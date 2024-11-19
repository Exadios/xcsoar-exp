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

#include <iostream>
#include "../tools/ADSB/GDL90/gdl90/GDL90.h"

//------------------------------------------------------------------------------
bool
DoHeartbeatTest(const GDL90::RawBytes& unpacked)
  {
  GDL90 gdl90;
  GDL90::MESSAGE_ID mid;

  std::cout << "Unpacked: " << std::hex;
  for (long unsigned int i = 0; i < unpacked.size(); i++)
    std::cout << "0x" << (int)unpacked[i] << ", ";
  std::cout << std::dec << "\n";

  uint32_t status, timestamp, msg_count_uplink, msg_count_basic_and_long;

  status = timestamp = msg_count_uplink = msg_count_basic_and_long = 0;

  if (gdl90.id_decode(mid, unpacked) != true)
    {
    std::cerr << "Id Decode problem\n";
    return false;
    }
  if (GDL90::MESSAGE_ID::HEARTBEAT != mid)
    {
    std::cerr << "Id Decode wrong type\n";
    return false;
    }

  if (gdl90.heartbeat_decode(unpacked, 
                             status,
                             timestamp,
                             msg_count_uplink,
                             msg_count_basic_and_long) == true)
    {
    std::cout << "Status: 0x" << std::hex << status << std::dec << "\n";
    std::cout << "Time: " << timestamp << 
                 ", 0x" << std::hex << timestamp << std::dec << "\n";
    std::cout << "Uplink: " << msg_count_uplink << "\n";
    std::cout << "Basic: " << msg_count_basic_and_long << "\n\n";
    }
  else
    {
    std::cout << "heartbeat_decode error\n";
    return false;
    }

  return true;
  }

//------------------------------------------------------------------------------
int main(int, const char*[])
  {
    {
    GDL90::RawBytes unpacked = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (DoHeartbeatTest(unpacked) == false)
      return 1;
    }

    {
    GDL90::RawBytes unpacked = {0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (DoHeartbeatTest(unpacked) == false)
      return 1;
    }

    {
    GDL90::RawBytes unpacked = {0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00};
    if (DoHeartbeatTest(unpacked) == false)
      return 1;
    }

    {
    GDL90::RawBytes unpacked = {0x00, 0x81, 0x00, 0xfa, 0x48, 0x00, 0x00};
    if(DoHeartbeatTest(unpacked) == false)
      return 1;
    }

    {
    GDL90::RawBytes unpacked = {0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x37};
    if (DoHeartbeatTest(unpacked) == false)
      return 1;
    }

  return 0;
  }
