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

#include "Stamp.hpp"

/**
 * This class provides a synthetic clock value (for NMEAInfo::clock)
 * fed from a wall-clock time.  This is useful for providing good
 * clock sources for replayed computations.
 *
 * Call Reset() before first use.
 */
class ReplayClock {
  using Duration = FloatDuration;

  TimeStamp clock;

  TimeStamp last_time;

public:
  void Reset() {
    clock = {};
    last_time = TimeStamp::Undefined();
  }

  /**
   * Compute the next clock value.
   *
   * @param time the wallclock time; -1 means unknown
   */
  TimeStamp NextClock(TimeStamp time) noexcept {
    Duration offset;
    if (!time.IsDefined() || !last_time.IsDefined() || time < last_time)
      /* we have no (usable) wall clock time (yet): fully synthesised
         clock; increment by one second on each iteration */
      offset = std::chrono::seconds{1};
    else if (time > last_time)
      /* apply the delta between the two wall clock times to the clock
         value */
      offset = time - last_time;
    else
      /* wall clock time was not updated: apply only a very small
         offset */
      offset = std::chrono::milliseconds{10};

    last_time = time;
    return clock += offset;
  }
};
