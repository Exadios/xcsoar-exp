/*
Copyright_License {

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
#include <iostream>
#include <fstream>
#include "DumpGDL90.hpp"

//------------------------------------------------------------------------------
int
main(int, const char* argv[] )
  {
  GDL90::RawBytes packed, unpacked;
  bool heartbeat_only = false;

  std::string invocation(argv[0]);
  if (std::string::npos != invocation.find("heartbeat"))
    heartbeat_only = true;

  while ((std::cin.good() == true) && ((std::cin.eof() == false)))
    {
    do
      {
      packed.clear();
      int c;
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        {
        std::cerr << "EOF\n";
        return 0;
        }
      packed.push_back((char)c);
      }
    while (packed[0] != 0x7e);

    int c = 0;
    do
      {
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        {
        std::cerr << "EOF\n";
        return 0;
        }
      packed.push_back((char)c);
      }
    while ((char)c != 0x7e);

    GDL90 gdl90;

    if (!gdl90.unpack(packed, unpacked))
      {
      std::cerr << "Unpack problem\n";
      return 1;
      }

    GDL90::MESSAGE_ID mid;
    if (gdl90.id_decode(mid, unpacked) != true)
      {
      std::cerr << "Id Decode problem\n";
      return 1;
      }

    switch (mid)
      {
      case GDL90::MESSAGE_ID::HEARTBEAT:
        {
        uint32_t status, timestamp, msg_count_uplink, msg_count_basic_and_long;
        std::cout << "Heartbeat\n";
        if (gdl90.heartbeat_decode(unpacked, 
                                   status,
                                   timestamp,
                                   msg_count_uplink,
                                   msg_count_basic_and_long) == true)
          std::cout << "Status: " << status
                    << ", Timestamp: " << timestamp
                    << ", Message count uplink: " << msg_count_uplink
                    << ", Message count basic and long: " << msg_count_basic_and_long
                    << "\n";
        else
          std::cout << "heartbeat_decode error\n";
        break;
        }
      case GDL90::MESSAGE_ID::INITIALIZATION:
        if (false == heartbeat_only)
          std::cout << "Initialization\n";
        break;
      case GDL90::MESSAGE_ID::UPLINK_DATA:
        if (false == heartbeat_only)
          std::cout << "Uplink Data\n";
        break;
      case GDL90::MESSAGE_ID::OWNERSHIP_REPORT:
        {
        GDL90::ALERT_STATUS alert_status;
        GDL90::ADDR_TYPE addr_type;
        uint32_t participant_address, latitude, longitude, altitude, misc;
        GDL90::NIC nic;
        GDL90::NACP nacp;
        uint32_t horiz_velocity, vert_velocity, track_hdg;
        GDL90::EMITTER emitter;
        std::string call_sign;
        GDL90::EMERGENCY_PRIO emergency_prio_code;
        if (false == heartbeat_only)
          std::cout << "Ownship Report\n";
        if (gdl90.ownership_or_traffic_report_decode(unpacked,
                                                     true,
                                                     alert_status, 
                                                     addr_type,
                                                     participant_address, 
                                                     latitude,
                                                     longitude,
                                                     altitude,
                                                     misc, 
                                                     nic,
                                                     nacp,
                                                     horiz_velocity,
                                                     vert_velocity, 
                                                     track_hdg, 
                                                     emitter,
                                                     call_sign,
                                                     emergency_prio_code) == true)
          {
          if (false == heartbeat_only)
            std::cout << "Alert Status: " << (int)alert_status
                      << ", Addr Type: " << (int)addr_type
                      << ", Participant Address: " <<std::hex << participant_address << std::dec
                      << ", Latitude: " << latitude
                      << ", Longitude: " << longitude
                      << ", Altitude: " << altitude
                      << ", Misc: " << misc
                      << ", NIC: " << (int)nic
                      << ", NACP: " << (int)nacp
                      << ", Horiz Velocity: " << horiz_velocity
                      << ", Vert Velocity: " << vert_velocity
                      << ", Track Hdg: " << track_hdg
                      << ", Emitter: " << (int)emitter
                      << ", Call Sign: " << call_sign
                      << ", Emergency Prio Code: " << (int)emergency_prio_code
                      << "\n";
          }
        break;
        }
      case GDL90::MESSAGE_ID::OWNERSHIP_GEOMETRIC_ALTITUDE:
        uint32_t geo_altitude, vertical_figure_of_merit;
        bool vertical_warning;
        if (false == heartbeat_only)
          std::cout << "Ownship Geometric Altitude\n";
        if (gdl90.ownership_geometric_altitude_decode(unpacked,
                                                      geo_altitude,
                                                      vertical_warning,
                                                      vertical_figure_of_merit ) == true)
          {
          if (false == heartbeat_only)
            std::cout << "Geo Altitude: " << geo_altitude
                      << ", Vertical Warning: " << vertical_warning
                      << ", Vertical Figure of Merit: " << vertical_figure_of_merit
                      << "\n";
          }
        break;
      case GDL90::MESSAGE_ID::TRAFFIC_REPORT:
        {
        GDL90::ALERT_STATUS alert_status;
        GDL90::ADDR_TYPE addr_type;
        uint32_t participant_address, latitude, longitude, altitude, misc;
        GDL90::NIC nic;
        GDL90::NACP nacp;
        uint32_t horiz_velocity, vert_velocity, track_hdg;
        GDL90::EMITTER emitter;
        std::string call_sign;
        GDL90::EMERGENCY_PRIO emergency_prio_code;
        if (false == heartbeat_only)
          std::cout << "Traffic Report\n";
        if (gdl90.ownership_or_traffic_report_decode(unpacked,
                                                     false,
                                                     alert_status, 
                                                     addr_type,
                                                     participant_address, 
                                                     latitude,
                                                     longitude,
                                                     altitude,
                                                     misc, 
                                                     nic,
                                                     nacp,
                                                     horiz_velocity,
                                                     vert_velocity, 
                                                     track_hdg, 
                                                     emitter,
                                                     call_sign,
                                                     emergency_prio_code) == true)
          {
          if (false == heartbeat_only)
            std::cout << "Alert Status: " << (int)alert_status
                      << ", Addr Type: " << (int)addr_type
                      << ", Participant Address: " << std::hex << participant_address << std::dec
                      << ", Latitude: " << latitude
                      << ", Longitude: " << longitude
                      << ", Altitude: " << altitude
                      << ", Misc: " << misc
                      << ", NIC: " << (int)nic
                      << ", NACP: " << (int)nacp
                      << ", Horiz Velocity: " << horiz_velocity
                      << ", Vert Velocity: " << vert_velocity
                      << ", Track Hdg: " << track_hdg
                      << ", Emitter: " << (int)emitter
                      << ", Call Sign: " << call_sign
                      << ", Emergency Prio Code: " << (int)emergency_prio_code
                      << "\n";
          }
        break;
        }
      case GDL90::MESSAGE_ID::BASIC_UAT_REPORT:
        if (false == heartbeat_only)
          std::cout << "Basic UAT Report\n";
        break;
      case GDL90::MESSAGE_ID::LONG_UAT_REPORT:
        if (false == heartbeat_only)
          std::cout << "Long UAT Report\n";
        break;
      case GDL90::MESSAGE_ID::FOREFLIGHT:
        {
        GDL90::MESSAGE_FOREFLIGHT_SUBID subid;
        if (gdl90.foreflight_subid_decode(subid, unpacked) == true)
          {
          if (GDL90::MESSAGE_FOREFLIGHT_SUBID::ID == subid)
            {
            uint64_t device_serial_number;
            std::string device_name, device_long_name;
            uint32_t capabilities_mask;
            if (gdl90.foreflight_id_decode(unpacked,
                                           device_serial_number,
                                           device_name,
                                           device_long_name,
                                           capabilities_mask ) == true)
              {
              if (false == heartbeat_only)
                {
                std::cout << "Foreflight ID\n";
                std::cout << "Device Serial Number: " << std::hex << device_serial_number << std::dec
                          << ", Device Name: " << device_name
                          << ", Device Long Name: " << device_long_name
                          << ", Capabilities Mask: " << capabilities_mask
                          << "\n";
                }
              }
            }
          else
            {
            uint32_t roll, pitch, heading, ias, tas;
            if (gdl90.foreflight_ahrs_decode(unpacked,
                                             roll,
                                             pitch,
                                             heading,
                                             ias,
                                             tas)  == true)
              {
              if (false == heartbeat_only)
                {
                std::cout << "Foreflight AHRS\n";
                std::cout << ", Roll: " << roll
                          << ", Pitch: " << pitch
                          << ", Heading: " << heading
                          << ", IAS: " << ias
                          << ", TAS: " << tas
                          << "\n";
                }
              }
            }
          }
        break;
        }
      default:
        if (false == heartbeat_only)
          std::cout << "Unknown Packet Type\n";
        break;
      }
    }

  return 0;
  }
