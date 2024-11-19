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

#include "CutGDL90After.hpp"
#include "Utilities.hpp"

#include <iostream>
#include <cstdint>
#include <ctime>
#include <cstring>

//------------------------------------------------------------------------------
int
main(int argc, const char* argv[])
  {
  time_t seconds_since_midnight = 0;
  bool end_detected = false;

  if ((argc != 2) || (ParseTime(seconds_since_midnight, argv[1]) == false))
    {
    std::cerr << "Invocation:\n"
              << argv[0] << " <Time Spec>\n";
    return 255;
    }
  seconds_since_midnight += 2209104000;

   GDL90::RawBytes packed, unpacked;

  while ((std::cin.good() == true) && ((std::cin.eof() == false)))
    {
    do
      {
      packed.clear();
      char c;
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        return 0;
      packed.push_back(c);
      }
    while (packed[0] != 0x7e);

    char c = 0;
    do
      {
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        return 0;
      packed.push_back(c);
      }
    while (c != 0x7e);

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
        if (gdl90.heartbeat_decode(unpacked, 
                                   status,
                                   timestamp,
                                   msg_count_uplink,
                                   msg_count_basic_and_long) == true)
          {
          if (timestamp > (uint32_t)seconds_since_midnight)
            end_detected = true;
          else
            std::cout.write((const char*)packed.data(), packed.size());
          }
        else
          std::cerr << "heartbeat_decode error\n";
        break;
        }
      default:
        if (false == end_detected)
          {
          std::cout.write((const char*)packed.data(), packed.size());
          }
        break;
      }
    }

  return 0;
  }
