/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

/**
 * This program reads a NMEA file and converts it to GDL90 for testing
 * purposes. Input and output is stdin and stdout.
 */

#include "DebugPort.hpp"
#include "GDL90Debug.hpp"
#include "Device/Port/Port.hpp"
#include "Device/Port/ConfiguredPort.hpp"
#include "Device/Config.hpp"
#include "Device/Parser.hpp"
#include "NMEA/Info.hpp"
#include "system/Args.hpp"
#include "system/Sleep.h"
#include "Operation/ConsoleOperationEnvironment.hpp"
#include "io/async/GlobalAsioThread.hpp"
#include "io/async/AsioThread.hpp"
#include "io/DataHandler.hpp"
#include "util/PrintException.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <bitset>
#include <string>
#include <array>
#include <vector>
#include <span>
#include <cmath>
#include <memory>
#include <map>
#include <fstream>

constexpr double meters2feet = 3.2808399;
constexpr double meters_per_second2knot = 1.94;

std::vector<std::byte> Packetize(std::vector<std::byte> s);
void DoHeartbeatMessage(std::unique_ptr<Port>& port, NMEAInfo& info);
void DoTrafficReportMessage(std::unique_ptr<Port>& port,
                            NMEAInfo& info,
                            FlarmTarget& target);
/*
 * These are the storage objects for the several messages.
 */
std::vector<std::byte>HeartbeatMessage;
std::vector<std::byte>OwnShipReportMessage;
std::vector<std::byte>TrafficReportMessage;
std::vector<std::byte>OwnShipGeoAltitudeMessage;

char idBuffer[1024];  /* A buffer, much bigger than needed,
                         to hold a id string. */

uint16_t crc16Table[256]; // Table for FCS.

std::map<unsigned long int, std::string> adsbNames;

//------------------------------------------------------------------------------
void
InitAircraftNames()
  {
  std::ifstream adsbNamesFile("AdsbNames.txt");
  if (!adsbNamesFile.is_open())
    return;   // No file so forget it.

  while (!adsbNamesFile.eof())
    {
    unsigned long int id_key;
    std::string call;

    adsbNamesFile >> std::hex >> id_key >> std::dec >> call;
//    std::cout << std::hex << id_key << std::dec << ", " << call << "\n";
    if (adsbNamesFile.eof())
      break;
    if (call.length() > 8)
      break;  // Callsign too big. ignore.
    while (call.length() < 8)
      call = call + " ";
    ::adsbNames.insert({id_key, call});
    }
//  for (const auto& pair : ::adsbNames)
//    std::cout << std::hex << pair.first << std::dec << ", " << pair.second << "\n";
  adsbNamesFile.close();
  }

//------------------------------------------------------------------------------
std::vector<std::byte>
Packetize(std::vector<std::byte> s)
  {
  std::vector<std::byte> op;

  for (unsigned int i = 0; i < s.size(); i++)
    {
    if ((s[i] == std::byte(0x7e)) || (s[i] == std::byte(0x7d)))
      {
      op.push_back(std::byte(0x7d));
      op.push_back(std::byte(s[i] ^ std::byte(0x20)));
      }
    else
      op.push_back(s[i]);
    }
  op.push_back(std::byte(0x7e));
  op.insert(op.begin(), std::byte(0x7e));

  return op;
  }

//------------------------------------------------------------------------------
constexpr void
Initfcs()
  {
  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  uint16_t i, bitchr, crc;

  for (i = 0; i < 256; i++)
    {
    crc = (i << 8);
    for (bitchr = 0; bitchr < 8; bitchr++)
      {
      crc = (crc << 1) ^ ((crc & 0x8000) ? 0x1021 : 0);
      }
    ::crc16Table[i] = crc;
    }
  }

//------------------------------------------------------------------------------
std::vector<std::byte>
AppendFCS(std::vector<std::byte>& s)
  {
  std::vector<std::byte> op = s;

  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  uint32_t i;
  uint16_t crc = 0;

  /** The data to be check summed start just after the initial flag
   * and terminates just before the packet check sum.
   */
  for (i = 0; i < s.size(); i++)
    {
    crc = ::crc16Table[crc >> 8] ^ (crc << 8) ^ (unsigned char)s[i];
    }
  op.push_back(std::byte(crc & 0x00ff));
  op.push_back(std::byte((crc & 0xff00) / 256));
  return op;
  }

//------------------------------------------------------------------------------
void
DoHeartbeatMessage(std::unique_ptr<Port>& port, NMEAInfo& info)
  {
  ::HeartbeatMessage.resize(7);

  if (info.alive == true)
    {
    FloatDuration time_here = info.clock.template Cast<FloatDuration>();
    std::cout << "Doing a Heartbeat at " 
              << (unsigned int)time_here.count()
              << std::hex << " (" << (unsigned int)time_here.count() << std::dec
              << ") seconds\n";

    unsigned int ticks = (unsigned int)time_here.count();
    unsigned int hour = ticks / (60 * 60);
    unsigned int minute = (ticks - (hour * 60 * 60)) / 60;
    unsigned int second = (ticks - (hour * 60 * 60) - minute * 60);
    info.date_time_utc = BrokenDateTime(hour, minute, second);

    ::HeartbeatMessage[0] = std::byte{0};

    ::HeartbeatMessage[1] = std::byte{0b00000001};      // UAT Initialized
    if (info.gps.fix_quality == FixQuality::GPS ||
        info.gps.fix_quality == FixQuality::DGPS)
      ::HeartbeatMessage[1] |= std::byte{0b10000000};
    
    ::HeartbeatMessage[2] = std::byte{0b00000001};      // UTC_OK
    unsigned int int_time = (unsigned int)info.time.ToDuration().count();
    if (int_time > ((0x10000) - 1))
      {
      ::HeartbeatMessage[2] |= std::byte{0b10000000};
      int_time -= 0x10000;
      }
    
    ::HeartbeatMessage[3] = ::HeartbeatMessage[4] = std::byte{0};
    ::HeartbeatMessage[4] = std::byte{(unsigned char )((int_time & 0xff00) >> 8)};
    ::HeartbeatMessage[3] = std::byte{(unsigned char )(int_time & 0xff)};

    std::vector<std::byte> op = Packetize(AppendFCS(HeartbeatMessage));
    if (port->Write(op.data(), op.size()) != op.size())
      {
      std::cerr << "Could not write HeartbeatMessage\n";
      }
    }
  else
    std::cout << "Not alive in DoHeartbeatMessage\n";
  }

/**
 *    offset    content
 *       27      p | x
 *       26        cc
 *       25        cc
 *       24        cc
 *       23        cc
 *       22        cc
 *       21        cc
 *       20        cc
 *       19        cc
 *       18        ee
 *       17        tt
 *       16        vv
 *       15      h | v
 *       14        hh
 *       13      i | a
 *       12      d | m
 *       11        dd
 *       10        nn
 *        9        nn
 *        8        nn
 *        7        ll
 *        6        ll
 *        5        ll
 *        4        aa
 *        3        aa
 *        2        aa
 *        1      s | t
 *        0        id
 *     
 */

//------------------------------------------------------------------------------
void
DoOwnShiptReport(std::unique_ptr<Port>& port, NMEAInfo& info)
  {
  ::OwnShipReportMessage.resize(28);

  ::OwnShipReportMessage[0] = std::byte{10};
  ::OwnShipReportMessage[1] = std::byte{0x01};

  /*
   * Our address. Why do we need this?
   */
  ::OwnShipReportMessage[2] = std::byte{0xe8};
  ::OwnShipReportMessage[3] = std::byte{0x20};
  ::OwnShipReportMessage[4] = std::byte{0x7c};

  std::byte quality_byte{0};    // Both i and a are zero unknown.
  if (info.gps.fix_quality_available == true)
    {
    if (((info.gps.fix_quality == FixQuality::GPS) ||
         (info.gps.fix_quality == FixQuality::DGPS)) &&
        (info.location_available == true))
      {
      // Encode i and a.

      // Encode lat and lon.
       unsigned int phi    = info.location.latitude.Degrees() / 180.0 * pow(2.0, 23);
       unsigned int lambda = info.location.longitude.Degrees() / 180.0 * pow(2.0, 23);
      ::OwnShipReportMessage[5]  = std::byte{(unsigned char )((phi & 0xff0000) >> 16)};
      ::OwnShipReportMessage[6]  = std::byte{(unsigned char )((phi & 0x00ff00) >> 8)};
      ::OwnShipReportMessage[7]  = std::byte{(unsigned char )((phi & 0x0000ff))};
      ::OwnShipReportMessage[8]  = std::byte{(unsigned char )((lambda & 0xff0000) >> 16)};
      ::OwnShipReportMessage[9]  = std::byte{(unsigned char )((lambda & 0x00ff00) >> 8)};
      ::OwnShipReportMessage[10] = std::byte{((unsigned char )(lambda & 0x0000ff))};
      ;
      }
    else
      {
      // Figure out what to do when no position is available.
      return;   // Return early because no position.
      ;
      }
    }
  else
    {
    // Figure out what to do when no fix quality is available.
    return;     // Return early because no fix quality available.
    ;
    }

  if (info.gps_altitude_available == true)
    {
    unsigned int fixed_altitude = (info.gps_altitude * meters2feet + 1000.0) / 25.0;
    ::OwnShipReportMessage[11] = std::byte{(unsigned char )((fixed_altitude & 0x0ff0) >> 4)};
    ::OwnShipReportMessage[12] = std::byte{(unsigned char )((fixed_altitude << 4) & 0x00f0)};
    }
  else
    return;     // Return early because no GPS alt.

  ::OwnShipReportMessage[12] &= std::byte{0xf0};
  if (info.track_available == true)
    ::OwnShipReportMessage[12] |= std::byte{0x01};      // True track.

  if (info.gps.pdop < 0.0)
    return;     // Return because unknown PDOP.
  if (info.gps.pdop < 7.5)
    quality_byte |= std::byte(0xb0);
  else if (info.gps.pdop < 25)
    quality_byte |= std::byte(0xa0);
  else if (info.gps.pdop < 75)
    quality_byte |= std::byte(0x90);
  else if (info.gps.pdop < 600)
    quality_byte |= std::byte(0x80);
  else if (info.gps.pdop < 1200)
    quality_byte |= std::byte(0x70);
  // Any higher then leave i at zero.

  if (info.gps.pdop < 3.0)
    quality_byte |= std::byte(0x0b);
  else if (info.gps.pdop < 15)
    quality_byte |= std::byte(0x0a);
  else if (info.gps.pdop < 45)
    quality_byte |= std::byte(0x09);
  else if (info.gps.pdop < 300)
    quality_byte | std::byte(0x08);
  else if (info.gps.pdop < 600)
    quality_byte |= std::byte(0x07);
  // Any higher then leave a at zero.

  ::OwnShipReportMessage[13] = quality_byte;

  if (info.ground_speed_available != true)
    return;     // Return early because no ground speed.
  unsigned int horizontal_velocity = info.ground_speed * meters_per_second2knot;
  ::OwnShipReportMessage[14]  = std::byte{(unsigned char )((horizontal_velocity & 0xff0) >> 4)};
  ::OwnShipReportMessage[15]  = std::byte{(unsigned char )((horizontal_velocity & 0x0f) << 4)};
  unsigned int vertical_velocity = 0;
  if (info.noncomp_vario_available == true)
    vertical_velocity = info.noncomp_vario * meters2feet / 64;
  ::OwnShipReportMessage[15] |= std::byte{(unsigned char )((vertical_velocity << 4) & 0xf0)};
  ::OwnShipReportMessage[16]  = std::byte{(unsigned char )((vertical_velocity >> 4) & 0xff)};
  unsigned char true_track = (unsigned char )(info.track.Degrees() * 256.0 / 360) & 0xff;
  ::OwnShipReportMessage[17] = std::byte{true_track};
  ::OwnShipReportMessage[18] = std::byte{9};    // Hard wire "Glider".
  ::OwnShipReportMessage[19] = std::byte{'V'};
  ::OwnShipReportMessage[20] = std::byte{'H'};
  ::OwnShipReportMessage[21] = std::byte{'-'};
  ::OwnShipReportMessage[22] = std::byte{'G'};
  ::OwnShipReportMessage[23] = std::byte{'S'};
  ::OwnShipReportMessage[24] = std::byte{'A'};
  ::OwnShipReportMessage[25] = std::byte{' '};
  ::OwnShipReportMessage[26] = std::byte{0};
  
  std::vector<std::byte> op = Packetize(AppendFCS(::OwnShipReportMessage));
  if (port->Write(op.data(), op.size()) != op.size())
    {
    std::cerr << "Could not write OwnShipReport\n";
    }
  }

//------------------------------------------------------------------------------
void
DoTrafficReportMessage(std::unique_ptr<Port>& port,
                       NMEAInfo& info,
                       FlarmTarget& target)
  {
  std::cout << "Traffic Report for " << std::hex << target.id.Arg()
            << std::dec << "\n";
  ::TrafficReportMessage.resize(28);

  ::TrafficReportMessage[0] = std::byte{20};
  ::TrafficReportMessage[1] = std::byte{0x01};

  ::TrafficReportMessage[4] = std::byte{(unsigned char)(target.id.Arg() & 0xff)};
  ::TrafficReportMessage[3] = std::byte{(unsigned char)((target.id.Arg() & 0xff00) >> 8)};
  ::TrafficReportMessage[2] = std::byte{(unsigned char)((target.id.Arg() & 0xff0000) >> 16)};

  if (info.location_available.IsValid() == true)
    {
    // Precalculate relative east and north projection to lat/lon
    // for Location calculations of each target
    constexpr Angle delta_lat = Angle::Degrees(0.01);
    constexpr Angle delta_lon = Angle::Degrees(0.01);

    GeoPoint plat = info.location;
    plat.latitude += delta_lat;
    GeoPoint plon = info.location;
    plon.longitude += delta_lon;

    double dlat = info.location.DistanceS(plat);
    double dlon = info.location.DistanceS(plon);

    if (fabs(dlat) > 0 && fabs(dlon) > 0)
      {
      double north_to_latitude = delta_lat.Degrees() / dlat;
      double east_to_longitude = delta_lon.Degrees() / dlon;

      target.location.latitude = Angle::Degrees(target.relative_north *
                                                north_to_latitude) +
                                 info.location.latitude;
      target.location.longitude = Angle::Degrees(target.relative_east *
                                                 east_to_longitude) +
                                  info.location.longitude;
      target.location_available = info.location_available;
      }
    }

  if (target.location_available == true)
    {
    // Encode phi and lambda.
     unsigned int phi    = target.location.latitude.Degrees() / 180.0 * pow(2.0, 23);
     unsigned int lambda = target.location.longitude.Degrees() / 180.0 * pow(2.0, 23);
    ::TrafficReportMessage[5]  = std::byte{(unsigned char )((phi & 0xff0000) >> 16)};
    ::TrafficReportMessage[6]  = std::byte{(unsigned char )((phi & 0x00ff00) >> 8)};
    ::TrafficReportMessage[7]  = std::byte{(unsigned char )((phi & 0x0000ff))};
    ::TrafficReportMessage[8]  = std::byte{(unsigned char )((lambda & 0xff0000) >> 16)};
    ::TrafficReportMessage[9]  = std::byte{(unsigned char )((lambda & 0x00ff00) >> 8)};
    ::TrafficReportMessage[10] = std::byte{((unsigned char )(lambda & 0x0000ff))};
    }
  else
    {
    // Figure out what to do when no fix is available.
    std::cerr << "No GPS fix\n";
    return;     // Return early because no fix available.
    ;
    }

  if (info.gps_altitude_available)
    {
    target.altitude = target.relative_altitude +
                      RoughAltitude(info.gps_altitude);
    target.altitude_available = info.gps_altitude_available;
    }

  if (target.altitude_available == true)
    {
    unsigned int fixed_altitude = (target.altitude * meters2feet +
                                   RoughAltitude(1000.0)) / 25.0;
    ::TrafficReportMessage[11] = std::byte{(unsigned char)(((unsigned short int)fixed_altitude & 0x0ff0) >> 4)};
    ::TrafficReportMessage[12] = std::byte{(unsigned char)(((unsigned short int)fixed_altitude << 4) & 0x00f0)};
    }
  else
    {
    std::cerr << "No GPS alt.\n";
    return;     // Return early because no GPS alt.
    }

  ::TrafficReportMessage[12] &= std::byte{0xf0};
  if (info.track_available == true)
    ::TrafficReportMessage[12] |= std::byte{0x01};      // True track.

  ::TrafficReportMessage[13] = std::byte{0x68}; // Arbitrarily set i = 6, a = 8

  if (target.speed_received != true)
    {
    std::cerr << "Speed not received\n";
    return;     // Return early because no ground speed.
    }

  unsigned int horizontal_velocity = target.speed * meters_per_second2knot;
  ::TrafficReportMessage[14]  = std::byte{(unsigned char )((horizontal_velocity & 0xff0) >> 4)};
  ::TrafficReportMessage[15]  = std::byte{(unsigned char )((horizontal_velocity & 0x0f) << 4)};
  unsigned int vertical_velocity = 0;
  if (target.climb_rate_received == true)
    vertical_velocity = target.climb_rate * meters2feet / 64;
  ::TrafficReportMessage[15] |= std::byte{(unsigned char )((vertical_velocity << 4) & 0xf0)};
  ::TrafficReportMessage[16]  = std::byte{(unsigned char )((vertical_velocity >> 4) & 0xff)};
  if (target.track_received == true)
    {
    unsigned char true_track = (unsigned char )(Angle(target.track).Degrees() * 256.0 / 360) & 0xff;
  ::TrafficReportMessage[17] = std::byte{true_track};
    }
  else
    {
    std::cerr << "Track not received\n";
    return; // Return early because track not received.
    }

  ::TrafficReportMessage[18] = std::byte{9};    // Hard wire "Glider".

  for (size_t i = 19; i < 27; i++)
    ::TrafficReportMessage[i] = std::byte{'\0'};

  if (auto s = adsbNames.find((long unsigned int)target.id.Arg());
           s != adsbNames.end())
    {
    for (size_t i = 0; i < 7; i++)
      {
      ::TrafficReportMessage[i + 19] = std::byte{(unsigned char)s->second[i]};
      }
    }
  else
    {
    target.id.Format(idBuffer);
    for (size_t i = 0; i < 7; i++)
      {
      ::TrafficReportMessage[i + 19] = std::byte{(unsigned char)idBuffer[i]};
      if (idBuffer[i] == '\0')
        break;
      }
    }
  
  ::TrafficReportMessage[27] = std::byte{0x00}; // No emergency.

  std::vector<std::byte> op = Packetize(AppendFCS(::TrafficReportMessage));
  if (port->Write(op.data(), op.size()) != op.size())
    {
    std::cerr << "Could not write TrafficReport\n";
    }
  }

//------------------------------------------------------------------------------
void
DoOwnShipGeoAltitude(std::unique_ptr<Port>& port, NMEAInfo& info)
  {
  ::OwnShipGeoAltitudeMessage.resize(5);

  ::OwnShipGeoAltitudeMessage[0] = std::byte{11};

  if (info.gps_altitude_available == true)
    {
    unsigned int alt = (info.gps_altitude * 3.28084) / 5.0;
    ::OwnShipGeoAltitudeMessage[1] = std::byte{(unsigned char)((alt >> 8) & 0x00ff)};
    ::OwnShipGeoAltitudeMessage[2] = std::byte{(unsigned char)((alt & 0x00ff))};
    // Vertical figure of merit is not available.
    ::OwnShipGeoAltitudeMessage[4] = std::byte{0x7f};
    ::OwnShipGeoAltitudeMessage[3] = std::byte{0xff};

    std::vector<std::byte> op = Packetize(AppendFCS(::OwnShipGeoAltitudeMessage));
    if (port->Write(op.data(), op.size()) != op.size())
      {
      std::cerr << "Could not write OwnShipAltitude\n";
      }
    }
  }


//------------------------------------------------------------------------------
class MyHandler : public DataHandler
  {
public:
  bool DataReceived(std::span<const std::byte> s) noexcept override
    {
    return fwrite(s.data(), 1, s.size(), stdout) == s.size();
    }
  };

//------------------------------------------------------------------------------
int
main(int argc, char **argv)
try
  {
  Args args(argc, argv, "PORT BAUD");
  DebugPort debug_port(args);
  args.ExpectEnd();

  InitAircraftNames();

  bool done_heartbeat = false;

  ScopeGlobalAsioThread global_asio_thread;

  MyHandler handler;

  ::Initfcs();

  auto port = debug_port.Open(*asio_thread, *global_cares_channel, handler);
  if (port == NULL)
    {
    fprintf(stderr, "Failed to open COM port\n");
    return EXIT_FAILURE;
    }

  ConsoleOperationEnvironment env;

  if (!port->WaitConnected(env)) 
    {
    fprintf(stderr, "Failed to connect the port\n");
    return EXIT_FAILURE;
    }

  if (!port->StartRxThread())
    {
    fprintf(stderr, "Failed to start the port thread\n");
    return EXIT_FAILURE;
    }

  NMEAInfo info;
  NMEAParser parser;
  info.Reset();
  int line_no = 0;

  char stamp[6] = "";

  char line[1024];
  while (fgets(line, sizeof(line), stdin) != NULL)
    {
    char *eos = NULL;

    if ((eos = strchr(line, '\n')) != NULL)
      *eos = '\0';  // Trim off a '\n' if it is there.

    line_no++;
    if ((parser.ParseLine(line, info)) == false)
      {
      continue;
      }

    info.alive.Update(info.clock);
    if (info.location_available &&
        info.gps.fix_quality_available &&
        info.gps_altitude_available &&
        done_heartbeat)
      {
      done_heartbeat = false;
      DoOwnShiptReport(port, info);
      }

    if (memcmp(line, "$GP", 3) == 0 &&
        (memcmp(line + 3, "GGA", 3) == 0 ||
         memcmp(line + 3, "RMC", 3) == 0) &&
        line[6] == ',' &&
        !StringIsEqual(stamp, line + 7, sizeof(stamp)))
      {
      for (size_t i = 0;
           i < info.target_data.traffic.GetActiveTrafficCount();
           i++)
        DoTrafficReportMessage(port,
                               info,
                               info.target_data.traffic.flarm_list[i]);

      /* the time stamp has changed - sleep for one second */
      Sleep(1000);
      strncpy(stamp, line + 7, sizeof(stamp));
      info.UpdateClock();
      info.Expire();
      DoHeartbeatMessage(port, info);
      DoOwnShipGeoAltitude(port, info);
      done_heartbeat = true;
      }

/*    port->FullWriteString(strcat(line, "\n"), env, std::chrono::seconds(1)); */
    }

  return EXIT_SUCCESS;
  }
catch (const std::exception &exception)
  {
  PrintException(exception);
  return EXIT_FAILURE;
  }
