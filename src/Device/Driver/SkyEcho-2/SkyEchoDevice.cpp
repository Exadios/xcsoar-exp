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

#include "SkyEchoDevice.hpp"
#include "Device/Port/Port.hpp"
#include "util/ConvertString.hpp"
#include "util/StaticString.hxx"
#include "util/TruncateString.hpp"
#include "util/Macros.hpp"
#include "util/NumberParser.hpp"
#include "util/StringCompare.hxx"
#include "NMEA/Info.hpp"
#include "NMEA/Checksum.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"
#include "Surveillance/AircraftType.hpp"
#include "Units/System.hpp"
#include "time/BrokenTime.hpp"
#include "Components.hpp"
#include "Logger/GDL90Logger.hpp"

#include <vector>
#include <span>
#include <math.h>

#ifndef NDEBUG
#include "LogFile.hpp"
#endif

//------------------------------------------------------------------------------
void
SkyEchoDevice::LinkTimeout()
  {
  }

//------------------------------------------------------------------------------
constexpr void
SkyEchoDevice::Initfcs()
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
    this->crc16Table[i] = crc;
    }
  }

//------------------------------------------------------------------------------
bool
SkyEchoDevice::Checkfcs(std::span<const std::byte> s)
  {
  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  uint32_t i;
  uint16_t crc = 0;

  /** The data to be check summed start just after the initial flag
   * and terminates just before the packet check sum.
   */
  for (i = 1; i < s.size() - 3; i++)
    {
    crc = this->crc16Table[crc >> 8] ^ (crc << 8) ^ (unsigned char)s[i];
    }
  uint16_t fcs =  (uint16_t )s[s.size() - 3] + 
                 ((uint16_t )s[s.size() - 2] * 256);
  return (crc == fcs) ? true : false;
  }

//------------------------------------------------------------------------------
bool
SkyEchoDevice::IsPacketConsistent(std::span<const std::byte> s) const
  {
  if ((s.front() == std::byte(0x7e)) && (s.back() == std::byte(0x7e)))
    {
    const std::byte *bp = s.data();
    int sz = s.size() - 2;
    for (bp++; sz != 0; sz--)
      if (*bp == std::byte(0x7e))
        {
        return false;   // Extra flag in the packet.
        }
    return true;
    }
  else
    {
    return false;
    }
  }

//-----------------------------------------------------------------------------
std::vector<std::span<const std::byte> >
SkyEchoDevice::CutPacket(std::span<const std::byte> s) const
  {
  std::size_t sop, eop;
  std::vector<std::span<const std::byte> > r;

  r.clear();
  if (s[0] != std::byte(0x7e))
    return r; /* If not a start of packet then return zero ADSB packets */

  for (sop = 0, eop = 1; eop < s.size(); )
    {
    for ( ; (eop < s.size()) && (s[eop] != std::byte(0x7e)); eop++)
      ;
    if (s[eop] == std::byte(0x7e))
      {
      r.push_back(s.subspan(sop, eop - sop + 1));  // The ADSB packet is valid.
      sop = eop + 1;  // Advance to next packet if it exists.
      eop = sop + 1;
      }
    else
      return r;
    }
  return r;
  }

//------------------------------------------------------------------------------
int
SkyEchoDevice::TwosComplement(unsigned int x, int n)
  {
#if 0
  int s = 0;
  unsigned int sign = (1 << n);
  unsigned int mask = sign - 1;
  s = mask & x;
  s -= sign & x;
  return s;
#else
  unsigned int sign = 1 << (n - 1);
 
  if ((x & sign) != 0)
    {
    unsigned int mask = sign - 1;
    unsigned int acc  = ~mask;
    acc |= x;
    return (int)acc;
    }
  else
    return (int)x;
#endif
  }

//------------------------------------------------------------------------------
void
SkyEchoDevice::Traffic(std::span<const std::byte> s, TrafficStruct &t)
  {
  assert(((MessageID)s[1] == MessageID::OWNSHIPREPORT) ||
         ((MessageID)s[1] == MessageID::TRAFFICREPORT));
  t.traffic_alert = (((unsigned char)s[2] & 0xf0 >> 4) == 1) ? true : false;
  t.addr_type     = (unsigned char )s[2] & 0x0f;
  t.address       = ((unsigned int)s[3] << 16) +
                    ((unsigned int)s[4] << 8)  +
                    ((unsigned int)s[5]);
  t.phi           = this->TwosComplement(((unsigned int)s[6] << 16) +
                                         ((unsigned int)s[7] << 8)  +
                                         ((unsigned int)s[8]),
                                         24);
  t.lambda        = this->TwosComplement(((unsigned int)s[9] << 16) +
                                         ((unsigned int)s[10] << 8) +
                                         ((unsigned int)s[11]),
                                         24);
  t.altitude      = ((unsigned int)s[12] << 8) +
                    (((unsigned int)s[13] & 0xf0) >> 4);
  t.misc          = (unsigned int)s[13] & 0x0f;
  t.nav_integrity = ((unsigned int)s[14] & 0xf0) >> 4;
  t.nav_accuracy  = (unsigned int)s[14] & 0x0f;
  t.horiz_vel     = (short)this->TwosComplement(((unsigned int)s[15] << 4) +
                                                (((unsigned int)s[16] & 0xf0) >> 4),
                                                12);
  t.vert_vel      = (short)this->TwosComplement((((unsigned int)s[16] & 0x0f) << 8) +
                                                ((unsigned int)s[17]),
                                                12);
  t.track         = (unsigned short)s[18];
  t.emitter       = (unsigned int)s[19];
  t.priority      = ((unsigned int)s[28] & 0xf0) >> 4;
  t.call_sign[0] = '\0';
  strncpy(t.call_sign, (const char *)&s[20], 8);
  }

//------------------------------------------------------------------------------
bool
SkyEchoDevice::DataReceived(std::span<const std::byte> s,
                            struct NMEAInfo &info) noexcept
  {
  /* SkyEcho 2 (with transceiver firmware version 2.6.13 and wifi 
   * firmware version 0.2.30) packages a number of GDL 90 packets in a
   * single UDP packet in various ways. These observations made on wireshark
   * dumps.
   *
   * The SkyEcho 2 packs one, or more, GDL 90 packets into each single UDP
   * packet. There are no observed instances of a GDL 90 packet being split
   * across UDP packets.
   *
   * In addition there appear to be a type of AHRS packet not detailed in
   * the GDL 90 documentation. These have a message id of 0x65. See 
   * https://www.foreflight.com/connect/spec/ for details of these types.
   * This id type have two sub types, 0x00 and 0x01. At this time only the
   * 0x00 sub type have been observed. Again it appears that these GDL 90
   * packets are not split across UDP packets.
   */

  if (s.empty() == true)
    {
#ifndef NDEBUG
    std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif
    return false;
    }
 
  if (::gdl90_logger != nullptr)
    ::gdl90_logger->Log(s);

  auto pkts = this->CutPacket(s);

  bool rtn = false;
  for (auto pkt = pkts.begin(); pkt != pkts.end(); pkt++)
    {
    std::span<const std::byte> cs = *pkt;

    if (this->IsPacketConsistent(cs) == false)
      {
#ifndef NDEBUG
      std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif
      break; // Failed basic structural tests.
      }

    /*
     Destuff the packet.
     */
    std::vector<std::byte> sd;

    for (unsigned int i = 0; i < cs.size(); i++)
      {
      if (cs[i] == std::byte(0x7d))
        {
        i++;
        assert(i < cs.size());
        sd.push_back(cs[i] ^ std::byte(0x20));
        }
      else
        {
        sd.push_back(cs[i]);
        }
      }

    if (this->Checkfcs(sd) == false)
      {
#ifndef NDEBUG
      std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif
      return false;   // FCS bad.
      }
 
    info.alive.Update(info.clock);
    switch ((MessageID)sd[1])
      {
      case MessageID::HEARTBEAT:
        {
        if (sd.size() != 7 + 4)
          {
#ifndef NDEBUG
          std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif
          break;
          }

        assert(info.clock.IsDefined());

        auto data = sd.data();
        bool go = false;
        /* Position available & maintenance not required and battery not low
           is 'go'. */
        go = (((data[2] & std::byte(0x80)) == std::byte(0x80)) && 
              ((data[2] & std::byte(0x40)) != std::byte(0x40)) &&  
              ((data[2] & std::byte(0x08)) != std::byte(0x08))) ?  
              true : false;
        this->time_of_day = ((unsigned int)data[3] & 0x80) << 8;
        this->time_of_day += (unsigned int)data[4] + ((unsigned int)data[5] << 8);

        /**
         * \todo pfb: Find a way to compute a broken down UTC date and time.
         *       Util this is done SkyEcho will not be able to provide
         *       a real position source because the Glide Computer needs
         *       a broken down UTC date and time set in info (Issue 5).
         * Note: GDL90 does not transmit date so this will have to be
         *       synthesized from some other source.
         */
#if 0
        int hour = this->time_of_day / (60 * 60);
        int min  = this->time_of_day / 60 - hour * 60;
        int sec  = this->time_of_day % 60;
                  
        (BrokenTime )(info.date_time_utc) = BrokenTime(hour, min, (unsigned int)sec);
        info.time_available.Update(info.clock);
#endif
        info.time = TimeStamp{FloatDuration(this->time_of_day)};

        info.gps.satellite_ids_available.Clear();   // Nope.
        info.gps.satellites_used_available.Clear(); // Likewise
        info.gps.fix_quality_available.Clear();
        /**
         * This 'go' status does not represent the status of the actual GPS.
         * Use the status derived in the OWNSHIP instead.
        info.adsb.status.gps = (go == true) ?
                               AdsbStatus::GPSStatus::GO :
                               AdsbStatus::GPSStatus::NO_GO;
         */
        if (go == false)
          this->own_ship.Reset(); // Clear accumulated position.
        info.target_data.status.available.Update(info.clock);
         
        rtn = true;
        break;
        }
      case MessageID::INITIALIZATION:
        {
        break;
        }
      case MessageID::UPLINK:
        {
        break;
        }
      case MessageID::ALTITUDEQFE:
        {
        if (sd.size() != 3 + 4)
          {
          break;
          }
      
        auto data = sd.data();
        unsigned int qfe = (unsigned int)data[2] * 256 + (unsigned int)data[3];
        if (qfe == 0x8000)
          {
#ifndef NDEBUG
          LogFormat("%s, %d\n", __FILE__, __LINE__);
#endif
          break;
          }
        break;
        }
      case MessageID::OWNSHIPREPORT:
        {
        if (sd.size() != 28 + 4)
          {
#ifndef NDEBUG
          std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif
          break;
          }

        this->Traffic(sd, this->own_ship);
  
        FloatDuration t((double)this->time_of_day);

        if (this->own_ship.nav_accuracy > 6) 
          {
          /**
           * this->own_ship.nav_accuracy > 6 seems to be a reliable indication
           * of whether the SkyEcho has a GPS position.
           */
          GeoPoint p(Angle::Degrees((double)this->own_ship.lambda * 180.0 / pow(2, 23)),
                     Angle::Degrees((double)this->own_ship.phi * 180.0 / pow(2, 23)));
          info.location = p;
          info.gps.real = true;
          info.track = Angle::Degrees((double)(this->own_ship.track) * 360.0 / 256.0);
          info.ground_speed = Units::ToSysUnit((double)this->own_ship.horiz_vel,
                                               Unit::KNOTS);
          info.track_available.Update(info.clock);
          info.ground_speed_available.Update(info.clock);

          info.location_available.Update(info.clock);
          info.target_data.status.gps = TargetStatus::GPSStatus::GPS_3D;
          info.target_data.status.available.Update(info.clock);
          }
        else
          info.target_data.status.gps = TargetStatus::GPSStatus::NONE;
        break;
        }
      case MessageID::OWNSHIPALTITUDE:
        {
        if (sd.size() != 5 + 4)
          {
          break;
          }

        auto data = sd.data();
        /**
         * I have observed that, on rare occasions, the SkyEcho-2 puts out
         * an altitude corresponding to {data[2], data[3]} equal to
         * {0x7f, 0xff}. This is plainly some status indicator which is
         * meant to report some state.
         */
        if (((unsigned int)data[2] != 0x7f) && ((unsigned int)data[3] != 0xff))
          {
          info.gps_altitude = Units::ToSysUnit((double)((unsigned int)data[2]  * 256 +
                                                        (unsigned int)data[3]) * 5,
                                                Unit::FEET);
          info.gps_altitude_available.Update(info.clock);
          }

        /* \todo pfb: Figure out what "vertical warning indicator" and
                 "vertical figure of merit" in table 16 of the GDL 90
                 document are all about.
        */
        break;
        }
      case MessageID::TRAFFICREPORT:
        {
        if (sd.size() != 28 + 4)
          break;

        this->Traffic(sd, this->target);
        AdsbTarget report;
      
        report.alarm_level = RemoteTarget::AlarmType::NONE; /* ADSB does not
                                                             * support alarm.*/
        report.location_available  = true;
        report.climb_rate_received = true;
        /* \todo pfb: Find the XCSoar wide conversion from feet / second to
                 meters / second.  */
        report.climb_rate          = (this->target.vert_vel * 64 / 3.2808) / 60;
        report.speed_received      = true;
        report.speed               = Units::ToSysUnit((double )this->target.horiz_vel,
                                                      Unit::KNOTS);
        report.track = Angle::Degrees((double)(this->target.track) * 360.0 / 256.0);
        report.type.Type(this->target.emitter);
        Angle phi    = Angle::Degrees((double)this->target.phi * 180.0 / pow(2, 23));
        Angle lambda = Angle::Degrees((double)this->target.lambda * 180.0 / pow(2, 23));
        report.location.latitude  = phi;
        report.location.longitude = lambda;
        report.altitude = Units::ToSysUnit((double)this->target.altitude * 25 - 1000,                                  // See section 3.5.1.4 of GDL 90 document.
                                          Unit::FEET);
        report.id.Arg(this->target.address);
        // There's got to be a better way to do this.
        for (unsigned i = 0;
             i < sizeof(TrafficStruct::call_sign) / sizeof (char);
             i++)
          report.name.push_back(target.call_sign[i]);

        info.location_available.Update(info.clock);

        /*
         * Update the traffic list. Make a new entry if required.
         */
        TargetList& target_list = info.target_data.traffic;
        target_list.modified.Update(info.clock);
        AdsbTarget* slot = target_list.FindAdsbTraffic(report.id);
        if (slot == nullptr)
          {
          slot = target_list.AllocateAdsbTraffic();
          if (slot == nullptr)
            return true;  // No space left in the list.
          slot->Clear();
          slot->id = report.id;
          target_list.new_traffic.Update(info.clock);
          }
        slot->valid.Update(info.clock); // The target is valid now.
        slot->Update(report); // Add or update this target to the list.
        break;
        }
      case MessageID::BASICREPORT:
        {
        break;
        }
      case MessageID::LONGREPORT:
        {
        break;
        }
      case MessageID::AHRS:
        {
        break;
        }
      default:  // Unknown packet type.
        {
        break;
        }
      }
    }
  
  return rtn;
  }

//------------------------------------------------------------------------------
static Device *
SkyEchoCreateOnPort([[maybe_unused]] const DeviceConfig &config,
                                     Port &comPort)
  {
  Device *dev = new SkyEchoDevice(comPort);

  return dev;
  }

//------------------------------------------------------------------------------
const struct DeviceRegister skyecho_driver =
  {
  _T("SkyEcho_2"),
  _T("SkyEcho 2"),
  DeviceRegister::RAW_GPS_DATA,
  SkyEchoCreateOnPort,
  };
