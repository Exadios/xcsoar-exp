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

#pragma once

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "Surveillance/RemoteTarget.hpp"
#include "NMEA/Validity.hpp"

#include <type_traits>

/**
 * The target status as reported by the device driver.
 */
struct TargetStatus
  {
  enum class GPSStatus: uint8_t 
    {
    NONE = 0,
    GPS_2D = 1,
    GPS_3D = 2,
    };

  /** Number of received surveillance devices */
  unsigned short rx;
  /** Transmit status */
  bool tx;

  /** GPS status */
  GPSStatus gps;

  /** Alarm level of the target */
  RemoteTarget::AlarmType alarm_level;

  /** Is target information available? */
  Validity available;

  constexpr void Clear() noexcept
    {
    available.Clear();
    }

  constexpr void Complement(const TargetStatus &add) noexcept 
    {
    if (!available && add.available)
      *this = add;
    }

  constexpr void Expire(TimeStamp clock) noexcept
    {
    available.Expire(clock, std::chrono::seconds(10));
    }
  };

/**
 * \}
 */
static_assert(std::is_trivial<TargetStatus>::value, "type is not trivial");
