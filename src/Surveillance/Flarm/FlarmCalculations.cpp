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

#include "Surveillance/Flarm/FlarmCalculations.hpp"

//------------------------------------------------------------------------------
double
FlarmCalculations::Average30s(const TargetId& id,
                              TimeStamp time,
                              double altitude) noexcept
  {
  ClimbAverageCalculator& item = averageCalculatorMap[id];
  return item.GetAverage(time, altitude, std::chrono::seconds{30});
  }

//------------------------------------------------------------------------------
void
FlarmCalculations::CleanUp(TimeStamp now) noexcept
  {
  constexpr FloatDuration MAX_AGE = std::chrono::minutes{1};

  // Iterate through ClimbAverageCalculators and remove expired ones
  for (auto it = averageCalculatorMap.begin(),
         it_end = averageCalculatorMap.end();
       it != it_end; )
    if (it->second.Expired(now, MAX_AGE))
      it = averageCalculatorMap.erase(it);
    else
      ++it;
  }
