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

#include <type_traits>
#include <array>
#include <xcsoar-cassert>
#include <type_traits>

/**
 * Derived climb rate history
 * 
 */
class ClimbHistory {
  /**
   * Store vario history from 0 to 360 kph.
   */
  static constexpr std::size_t SIZE = 100;

  /** Average climb rate for each episode */
  std::array<double, SIZE> vario;

  /** Number of samples in each episode */
  std::array<unsigned short, SIZE> count;

public:
  void Clear();

  void Add(unsigned speed, double vario);

  /**
   * Do we have data for the specified speed?
   */
  bool Check(unsigned speed) const {
    return speed < count.size() && count[speed] > 0;
  }

  /**
   * Returns the average climb rate for the specified speed.
   */
  double Get(unsigned speed) const {
    assert(speed < vario.size());
    assert(count[speed] > 0);

    return vario[speed] / count[speed];
  }
};

static_assert(std::is_trivial<ClimbHistory>::value, "type is not trivial");
