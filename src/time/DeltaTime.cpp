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

#include "DeltaTime.hpp"

#include <xcsoar-cassert>

FloatDuration
DeltaTime::Update(TimeStamp current_time, FloatDuration min_delta,
                  FloatDuration warp_tolerance) noexcept
{

  assert(current_time.IsDefined());
  assert(min_delta.count() >= 0);
  assert(warp_tolerance.count() >= 0);

  if (!IsDefined()) {
    /* first call */
    last_time = current_time;
    return {};
  }

  if (current_time < last_time) {
    /* time warp */

    const auto delta = last_time - current_time;
    last_time = current_time;
    return delta < warp_tolerance ? FloatDuration{0} : FloatDuration{-1};
  }

  const auto delta = current_time - last_time;
  if (delta < min_delta)
    /* difference too small, don't update "last" time stamp to let
       small differences add up eventually */
    return {};

  last_time = current_time;

  if (delta > std::chrono::hours{4})
    /* after several hours without a signal, we can assume there was
       a time warp */
    return FloatDuration{-1};

  return delta;
}
