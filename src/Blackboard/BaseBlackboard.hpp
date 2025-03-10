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

#pragma once

#include "NMEA/MoreData.hpp"
#include "NMEA/Derived.hpp"

/**
 * Base class for blackboards, providing read access to \ref NMEAInfo and
 * \ref DerivedInfo
 */
class BaseBlackboard
{
protected:
  MoreData gps_info;
  DerivedInfo calculated_info;

public:
  /**
   * Give a reference to this blackboard's GPS data (i.e. input data)
   * @return A const reference to the input data.
   */
  constexpr const MoreData &Basic() const noexcept {
    return gps_info;
  }

  /**
   * Give a reference to this blackboard's calculated data.
   * @return A const reference to the calculated_data.
   */
  constexpr const DerivedInfo& Calculated() const noexcept {
    return calculated_info;
  }
};
