/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#include "Device.hpp"
#include "Device/Port/Port.hpp"
#include "util/ConvertString.hpp"
#include "util/StaticString.hxx"
#include "util/TruncateString.hpp"
#include "util/Macros.hpp"
#include "util/NumberParser.hpp"
#include "util/StringCompare.hxx"
#include "NMEA/Checksum.hpp"
#include "ADSB/Traffic.hpp"
#include "FLARM/List.hpp"
#include "ADSB/Traffic.hpp"

#define FEET2METERS  0.3048
//------------------------------------------------------------------------------
void
SkyEchoDevice::LinkTimeout()
  {
  mode = Mode::UNKNOWN;
  }

//------------------------------------------------------------------------------
void
SkyEchoDevice::Initfcs()
  {
  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  unsigned int i, bitchr, crc;

  for (i = 0; i < 256; i++)
    {
    crc - (i << 8);
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
  unsigned long int i;
  unsigned int crc = 0;
  const auto *block = s.data();

  for (i = 0; i < s.size(); i++)
    {
    crc = this->crc16Table[crc >> 8] ^ (crc << 8) ^ (unsigned int)block[i];
    }
    unsigned int fcs = (unsigned int )block[s.size() - 3] + 
                       (unsigned int )block[s.size() - 2] << 8;
    return (crc == fcs) ? true : false;
  }

//------------------------------------------------------------------------------
bool
SkyEchoDevice::IsPacketConsistent(std::span<const std::byte> s) const
  {
  auto si = s.begin();
  auto ei = s.end() - 1;
  if ((*si == std::byte(0x7e)) && (*ei == std::byte(0x7e)))
    {
    const std::byte *bp = s.data();
    int sz = s.size() - 2;
    for (bp++; sz != 0; sz--)
      if (*bp == std::byte(0x7e))
        return false;   // Extra flag in the packet.
    return true;
    }
  else
    return false;
  }

//------------------------------------------------------------------------------
std::span<const std::byte>
SkyEchoDevice::Destuff(std::span<const std::byte> s)
  {
  std::span<std::byte> sd;
  std::byte ea[] = {std::byte(0x7d)};
  std::span e{ea};
  auto sdit = sd.begin();

  for (auto sit  = s.begin(); sit != s.end(); sit++, sdit++)
    {
    if (*sit == std::byte(0x7d))
      {
      sit++;
      *sdit = *sit ^ std::byte(0x20);
      }
    else
      {
      *sdit = *sit;
      }
    }
   
  return sd;
  }

//------------------------------------------------------------------------------
/**
 * Compute the signed value of a raw unsigned integer.
 * @param x The raw input.
 * @param n the number of significant bits of x.
 * @return The signed representation of n.
 */
int
SkyEchoDevice::TwosComplement(unsigned int x, int n)
  {
  int s = 0;
  unsigned int sign = 2 ^ (n - 1);
  unsigned int mask = sign - 1;
  s = mask & x;
  s -= sign & x;
  return s;
  }

//------------------------------------------------------------------------------
void
SkyEchoDevice::Traffic(std::span<const std::byte> s, TrafficStruct &t)
  {

  t.traffic_alert = (((unsigned char)s[2] & 0xf0 >> 4) == 1) ? true : false;
  t.addr_type     = (unsigned char )s[2] & 0x0f;
  t.address       = (unsigned int)s[3] << 16 +
                    (unsigned int)s[4] << 8  +
                    (unsigned int)s[5];
  t.phi           = this->TwosComplement((unsigned int)s[6] << 16 +
                                         (unsigned int)s[7] << 8  +
                                         (unsigned int)s[8],
                                         24);
  t.lambda        = this->TwosComplement((unsigned int)s[9] << 16 +
                                         (unsigned int)s[10] << 8 +
                                         (unsigned int)s[11],
                                         24);
  t.altitude      = (unsigned int)s[12] << 8 + (unsigned int)s[13] & 0xf0 >> 4;
  t.misc          = (unsigned int)s[13] & 0x0f;
  t.nav_integrity = (unsigned int)s[14] & 0xf0 >> 4;
  t.nav_accuracy  = (unsigned int)s[14] & 0x0f;
  t.horiz_vel     = (short)this->TwosComplement((unsigned int)s[15] << 4 +
                                                (unsigned int)s[16] & 0xf0 >> 4,
                                                12);
  t.vert_vel      = (short)this->TwosComplement((unsigned int)s[16] & 0x0f << 8 +
                                                (unsigned int)s[17],
                                                12);
  t.track         = (short)this->TwosComplement((unsigned int)s[18], 8);
  t.emitter       = (unsigned int)s[19];
  t.priority      = (unsigned int)s[28] & 0xf0 >> 4;
  strncpy(t.call_sign, (const char *)s.data()[20], 8);
  }

//------------------------------------------------------------------------------
bool
SkyEchoDevice::DataReceived(std::span<const std::byte> s,
                            struct NMEAInfo &info) noexcept
  {
  assert(!s.empty());
  if (this->IsPacketConsistent(s) == false)
    return false; // Failed basic structural tests.

  const auto sd = this->Destuff(s);
  const auto *data = sd.data();
  const auto *const end = data + sd.size();

  if (((unsigned char)data[0] != 0x7e) || 
       (unsigned char)data[sd.size() - 1] != 0x7e)
    return false;   // SOF or EOF not present.
  if (Checkfcs(sd) == false)
    return false;   // FCS bad.
 
  switch ((MessageID)data[1])
    {
    case MessageID::HEARTBEAT:
      {
      if (sd.size() != 7 + 4)
        return false;

      bool go = false;
      go = ( ((unsigned char)data[2] & 0x80) &&  // Position available
            !((unsigned char)data[2] & 0x40) &&  // Maintenance required
            !((unsigned char)data[2] & 0x08)) ?  // GPS low battery voltage
            true : false;
      unsigned long int time_of_day = ((unsigned int)data[3] & 0x80) << 8;
      time_of_day += (unsigned int)data[4] + (unsigned int)data[5] << 8;

      /*
       * The assumption here is that all the packets following a heartbeat 
       * until the next heartbeat belong to this heartbeat. This may not
       * be true.
       */
      this->own_ship.Reset(); // Clear accumulated position.
      break;
      }
    case MessageID::INITIALIZATION:
      {

      break;
      }
    case MessageID::UPLINK:
      {

      break;    // Drop UAT uplink data.
      }
    case MessageID::ALTITUDEQFE:
      {
      if (sd.size() != 3 + 4)
        return false;
      
      unsigned int qfe = (unsigned int)data[2] * 256 + (unsigned int)data[3];
      if (qfe == 0x8000)
        return false;
      break;
      }
    case MessageID::OWNSHIPREPORT:
      {
      if (sd.size() != 28 + 4)
        return false;

      this->Traffic(sd, this->own_ship);

      break;
      }
    case MessageID::OWNSHIPALTITUDE:
      {
      if (sd.size() != 5 + 4)
        return false;

      short qnh = (unsigned int)data[2] * 256 + (unsigned int)data[3] * 5;
      unsigned int vert_metric = (unsigned int)data[4] * 256 +
                                 (unsigned int)data[5];
      if (vert_metric & 0x800 != 0)
        return false;   // Some sort of position error.
      if ((vert_metric == 0x7fff) | (vert_metric == 0x7ffe))
        return false;   // Metric either off scale or not available.
      break;
      }
    case MessageID::TRAFFICREPORT:
      {
      if (sd.size() != 28 + 4)
        return false;

      this->Traffic(sd, this->traffic);
      AdsbTraffic report;
      
      report.alarm_level = AdsbTraffic::AlarmType::NONE; /* ADSB does not
                                                          * support alarm.*/
      report.location_available  = true;
      report.turn_rate_received  = false;   // Not in ADSB.
      report.turn_rate           = 0.0;
      report.climb_rate_received = true;
      report.climb_rate          = this->traffic.vert_vel;
      report.speed_received      = true;
      report.speed               = this->traffic.horiz_vel;
      if(this->traffic.emitter > sizeof(AdsbTraffic::AircraftType / sizeof(uint8)))
        report.type = AdsbTraffic::AircraftType::UNKNOWN;
      else
        report.type               = this->traffic.emitter;
      Angle phi    = M_PI * (double)this->traffic.phi / 180.0;
      Angle lambda = M_PI * (double)this->traffic.lambda / 180.0;
      report.location.latitude  = phi;
      report.location.longitude = lambda;
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
    default:
      return false; // Unknown packet type.
    }
  
  return true;
  }
