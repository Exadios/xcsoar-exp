/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#ifndef XCSOAR_FLARM_DEVICE_HPP
#define XCSOAR_FLARM_DEVICE_HPP

#include "BinaryProtocol.hpp"
#include "Util/AllocatedArray.hxx"
#include "Compiler.h"
#include "tchar.h"
#include "Device/Driver.hpp"
#include "Device/SettingsMap.hpp"

#include <string>

#include <stdint.h>

class Port;
struct Declaration;
class OperationEnvironment;
class RecordedFlightList;
struct RecordedFlightInfo;
class NMEAInputLine;

class Dump1090Device: public AbstractDevice
{
private:
  Port &port;

  Mode mode;

  uint16_t sequence_number;

public:
  Dump1090Device(Port &port)
    : port(port),
      sequence_number(0)
    {
    }

protected:
  bool TextMode(OperationEnvironment &env);
  bool BinaryMode(OperationEnvironment &env);

  bool ParsePFLAC(NMEAInputLine &line);

public:
  /* virtual methods from class Device */
  void LinkTimeout() override;

  void Restart(OperationEnvironment &env);

private:
  bool Receive(const char *prefix, char *buffer, size_t length,
               OperationEnvironment &env, unsigned timeout_ms);
};

#endif
