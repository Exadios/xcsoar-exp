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

#include "Surveillance/TargetError.hpp"
#include "util/Macros.hpp"
#include "Language/Language.hpp"

static const TCHAR* const severity_strings[] =
  {
  N_("No error"),
  N_("Information"),
  N_("Reduced functionality"),
  N_("Fatal problem"),
  };

const TCHAR*
TargetError::ToString(Severity severity) noexcept
  {
  unsigned i = (unsigned)severity;
  return i < ARRAY_SIZE(severity_strings)
           ? severity_strings[i]
           : N_("Unknown");
  }

static constexpr struct
  {
  TargetError::Code code;
  const TCHAR* string;
  } error_strings[] =
    {
    { TargetError::Code::FIRMWARE_TIMEOUT, N_("Firmware timeout") },
    { TargetError::Code::POWER, N_("Power") },
    { TargetError::Code::GPS_COMMUNICATION, N_("GPS communication") },
    { TargetError::Code::GPS_CONFIGURATION, N_("GPS configuration") },
    { TargetError::Code::RF_COMMUNICATION, N_("RF communication") },
    { TargetError::Code::COMMUNICATION, N_("Communication") },
    { TargetError::Code::FLASH_MEMORY, N_("Flash memory") },
    { TargetError::Code::PRESSURE_SENSOR, N_("Pressure sensor") },
    { TargetError::Code::OBSTACLE_DATABASE, N_("Obstacle database") },
    { TargetError::Code::FLIGHT_RECORDER, N_("Flight recorder") },
    { TargetError::Code::TRANSPONDER_RECEIVER, N_("Transponder receiver") },
    { TargetError::Code::OTHER, N_("Other") },
    { TargetError::Code::OTHER, nullptr }
    };

const TCHAR*
TargetError::ToString(Code code) noexcept
  {
  for (auto i = error_strings; i->string != nullptr; ++i)
    if (i->code == code)
      return i->string;

  return N_("Unknown");
  }