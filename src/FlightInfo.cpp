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

#include "FlightInfo.hpp"
#include "time/BrokenDateTime.hpp"

std::chrono::system_clock::duration
FlightInfo::Duration() const noexcept
{
  if (!date.IsPlausible() ||
      !start_time.IsPlausible() || !end_time.IsPlausible())
    return std::chrono::seconds{-1};

  auto duration = BrokenDateTime(date, end_time) - BrokenDateTime(date, start_time);

  // adjust for possible date advance between start and end (add one day)
  if (duration.count() < 0)
    duration += std::chrono::hours(24);

  // if still not a plausible duration return invalid duration
  if (duration.count() < 0 || duration > std::chrono::hours(14))
    return std::chrono::seconds{-1};

  return duration;
}
