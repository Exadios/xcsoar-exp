/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2025 The XCSoar Project
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

#include "Device/Driver/FLARM/StaticParser.hpp"
#include "Surveillance/TargetStatus.hpp"
#include "Surveillance/TargetList.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Flarm/FlarmListDecorator.hpp"
#include "Device/Parser.hpp"
#include "Geo/Geoid.hpp"
#include "NMEA/Info.hpp"
#include "NMEA/Checksum.hpp"
#include "NMEA/InputLine.hpp"
#include "Units/System.hpp"
#include "util/CharUtil.hxx"
#include "NMEA/Info.hpp"

#include <string>

/**
 * The purpose of this test jig is to exercise the FLARM chain from 
 * \ref Device/Driver/FLARM/StaticParser.cpp to \ref FlarmComputer::Process.
 */

static NMEAInfo info;

static std::string record[] =
  {
    {"$GPRMC,070507.00,A,3257.74250,S,11704.84447,E,62.900,107.93,220919,,,A*49"},
    {"$GPVTG,107.93,T,,M,62.900,N,116.490,K,A*07"},
    {"$GPGGA,070507.00,3257.74250,S,11704.84447,E,1,09,0.76,1004.4,M,-29.7,M,,*53"},
    {"$GPGSA,A,3,24,12,19,15,06,02,25,29,32,,,,1.27,0.76,1.02*04"},
    {"$GPGSV,4,1,13,02,47,058,33,06,30,119,29,12,59,188,38,14,04,217,23*74"},
    {"$GPGSV,4,2,13,15,11,345,38,19,16,137,33,24,78,008,34,25,32,241,34*7A"},
    {"$GPGSV,4,3,13,29,25,311,36,32,17,236,30,40,15,286,,42,41,044,*7A"},
    {"$GPGSV,4,4,13,50,41,044,*4B"},
    {"$GPGLL,3257.74250,S,11704.84447,E,070507.00,A,A*7E"},
    {"PFLAU,1,1,2,1,0,-52,0,-41,7009,D000F2*41"},
    {"$GPRMC,070507.00,A,3257.74284,S,11704.84412,E,62.937,109.24,220919,,,A*46"},
    {"$PGRMZ,2860,F,2*06"},
    {"$GPGGA,070507.00,3257.74284,S,11704.84412,E,1,09,0.85,1001.2,M,-27.7,M,,*5B"},
    {"$PFLAA,0,4127,5448,-39,2,D000F2,273,,31,-0.8,1,0*4C"},
  };
//------------------------------------------------------------------------------
int main()
  {
  NMEAParser parser;

  for (int i = 0; i < 14; i++)
    {
    parser.ParseLine(record[i].c_str(), info);
    std::cout << "info.alive: " << info.alive.IsValid() << '\n';
    std::cout << "info.gps.fix_quality_available: " << info.gps.fix_quality_available.IsValid() << '\n';
//    std::cout << "info.gps.fix_quality: " << info.gps.fix_quality << '\n';
    std::cout << "info.gps.satellites_used: " << info.gps.satellites_used << '\n';
    std::cout << "info.location_available: " << info.location_available.IsValid() << '\n';
    std::cout << "info.location.IsValid(): " << info.location.IsValid() << '\n';
    std::cout << "info.location: " << info.location.latitude.Degrees() << ", "
                                   << info.location.longitude.Degrees()
                                   << '\n';
    std::cout << "info.target_data.traffic.IsEmpty(): "
              <<  info.target_data.traffic.IsEmpty() << '\n';
    if (info.target_data.traffic.IsEmpty() == false)
      {
      FlarmListDecorator fld(&info.target_data.traffic);
      for (TargetPtr tptr = info.target_data.traffic.FirstTraffic();
           tptr != nullptr;
           tptr = info.target_data.traffic.NextTraffic(tptr))
        {
        FlarmPtr fptr = FlarmListDecorator::FlarmCast(tptr);
        if (fptr == nullptr)
          std::cout << "Found traffic in info is not FLARM" << '\n';
        else
          std::cout << "Found traffic in info is FLARM" << '\n';
        std::cout << "fptr->location_available: " << fptr->location_available << '\n';
        std::cout << "fptr->valid: " << fptr->valid.IsValid() << '\n';
        std::cout << "fptr->id: " << std::hex << fptr->id.Arg() << std::dec << '\n';
        std::cout << "fptr->north, fptr->east: " 
                  << fptr->relative_north << ", "
                  << fptr->relative_east << '\n';
        }
      }
    NMEAInfo copy_a(info);
    if (copy_a.target_data.traffic.IsEmpty() == false)
      {
      FlarmListDecorator fld(&copy_a.target_data.traffic);
      for (TargetPtr tptr = copy_a.target_data.traffic.FirstTraffic();
           tptr != nullptr;
           tptr = copy_a.target_data.traffic.NextTraffic(tptr))
        {
        FlarmPtr aptr = FlarmListDecorator::FlarmCast(tptr);
        if (aptr == nullptr)
          std::cout << "Found traffic in copy_a is not FLARM" << '\n';
        else
          std::cout << "Found traffic in copy_a is FLARM" << '\n';
        std::cout << "aptr->location_available: " << aptr->location_available << '\n';
        std::cout << "aptr->valid: " << aptr->valid.IsValid() << '\n';
        std::cout << "aptr->id: " << std::hex << aptr->id.Arg() << std::dec << '\n';
        std::cout << "aptr->north, aptr->east: " 
                  << aptr->relative_north << ", "
                  << aptr->relative_east << '\n';
        }
      }
    NMEAInfo copy_b = info;
    if (copy_b.target_data.traffic.IsEmpty() == false)
      {
      FlarmListDecorator fld(&copy_b.target_data.traffic);
      for (TargetPtr tptr = copy_b.target_data.traffic.FirstTraffic();
           tptr != nullptr;
           tptr = copy_b.target_data.traffic.NextTraffic(tptr))
        {
        FlarmPtr bptr = FlarmListDecorator::FlarmCast(tptr);
        if (bptr == nullptr)
          std::cout << "Found traffic in copy_b is not FLARM" << '\n';
        else
          std::cout << "Found traffic in copy_b is FLARM" << '\n';
        std::cout << "bptr->location_available: " << bptr->location_available << '\n';
        std::cout << "bptr->valid: " << bptr->valid.IsValid() << '\n';
        std::cout << "bptr->id: " << std::hex << bptr->id.Arg() << std::dec << '\n';
        std::cout << "bptr->north, bptr->east: " 
                  << bptr->relative_north << ", "
                  << bptr->relative_east << '\n';
        }
      }
      
    }

  return 0;
  }
