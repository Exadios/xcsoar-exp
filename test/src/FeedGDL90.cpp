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
#include <fstream>

#include "DebugPort.hpp"
#include "Device/Port/Port.hpp"
#include "Device/Port/ConfiguredPort.hpp"
#include "Device/Config.hpp"
#include "system/Args.hpp"
#include "system/Sleep.h"
#include "Operation/ConsoleOperationEnvironment.hpp"
#include "io/async/GlobalAsioThread.hpp"
#include "io/async/AsioThread.hpp"
#include "io/DataHandler.hpp"
#include "util/PrintException.hxx"
#include "../tools/ADSB/GDL90/gdl90/GDL90.h"

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

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

  GDL90::RawBytes packed, unpacked;

  ScopeGlobalAsioThread global_asio_thread;

  MyHandler handler;
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

  uint32_t last_timestamp = 0;
  bool     timestamp_received = false;
  while ((std::cin.good() == true) && ((std::cin.eof() == false)))
    {
    do
      {
      packed.clear();
      char c;
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        {
        fprintf(stderr, "EOF in preamble\n");
        return 0;
        }
      packed.push_back(c);
      }
    while (packed[0] != 0x7e);

    unsigned char c = 0;
    int n = 0;
    do
      {
      if ((c = std::cin.get()) == std::istream::traits_type::eof())
        {
        fprintf(stderr, "EOF in data with %d byte read\n", n);
        return 0;
        }
      n++;
      packed.push_back(c);
      std::cerr << " 0x" << (unsigned int)c;
      }
    while (c != 0x7e);

    std::cerr << "\n";

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
          if (false == timestamp_received)
            {
            last_timestamp = timestamp;
            timestamp_received = true;
            }
          else
            {
            sleep(timestamp - last_timestamp);

            last_timestamp = timestamp;
            }
          if (port->Write(packed.data(), packed.size()) != packed.size())
            std::cerr << "Could not write Heartbeat\n";

          }
        else
          std::cout << "heartbeat_decode error\n";
        break;
        }
      default:
        if (true == timestamp_received)
          if (port->Write(packed.data(), packed.size()) != packed.size())
            std::cerr << "Could not write data\n";
        break;
      }
    }

  return EXIT_SUCCESS;
  }
catch (const std::exception &exception)
  {
  PrintException(exception);
  return EXIT_FAILURE;
  }
