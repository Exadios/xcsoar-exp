/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2023 The XCSoar Project
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

#include "FLARM/Error.hpp"
#include "FLARM/Version.hpp"

/**
 * A class to hold FLARM system status for \ref TargetData.
 */
class FlarmSystemStatus final
  {
public:
  /**
   * Ctor
   */
  FlarmSystemStatus() = default;

  constexpr bool IsDetected() const noexcept
    {
    return true;
    }

  constexpr void Clear() noexcept
    {
    this->error.Clear();
    this->version.Clear();
    }

  constexpr void Complement(const FlarmSystemStatus& rhs) noexcept
    {
    this->error.Complement(rhs.error);
    this->version.Complement(rhs.version);
    } 

  constexpr void Expire(TimeStamp clock) noexcept
    {
    this->error.Expire(clock);
    this->version.Expire(clock);
    }

  FlarmError error;
  FlarmVersion version;
  };

/**
 * \}
 */
static_assert(std::is_trivial<FlarmSystemStatus>::value, "type is not trivial");
