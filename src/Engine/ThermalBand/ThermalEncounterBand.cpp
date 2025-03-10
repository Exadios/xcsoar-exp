/* Copyright_License {

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

#include "ThermalEncounterBand.hpp"

#include <xcsoar-cassert>

void
ThermalEncounterBand::AddSample(const TimeStamp time,
                                const double height) noexcept
{
  const FloatDuration time_rel = time - time_start;

  // start condition, not initialised or descended through start, or gone back in time
  if (empty() || height <= h_min || time_rel.count() <= 0) {
    Start(time, height);
    return;
  }

  const unsigned next = ResizeToHeight(height);
  const unsigned prev = FindPenultimateFinished(next, time_rel);

  // forward predict, mixing in previous segment proportionally to
  // how far into this segment has been achieved
  const FloatDuration tstep = EstimateTimeStep(time_rel, height, prev);

  // fill previous relevant slices with interpolated time, and update stats
  for (unsigned i = prev + 1; i <= next; ++i) {
    slices[i].SetSample(slices[i - 1].time + tstep);
    Update(i - 1);
  }

  // calculate remainder part
  const double p = (height - GetSliceHeight(next - 1)) / dh;
  assert(p >= 0);
  assert(p <= 1);
  slices[next].n = p;
  Update(next);
}

inline unsigned
ThermalEncounterBand::ResizeToHeight(const double height) noexcept
{
  unsigned index = GetSliceIndex(height) + 1;
  // check to see if we need to resample
  while (index + 1 >= slices.capacity()) {
    // run out of height, must resample!
    Decimate(true);
    index = GetSliceIndex(height) + 1;
  }

  // handle size reduction or missing slices
  if (size() > index + 1) {
    slices.resize(index + 1);
  } else {
    while (size() < index + 1) {
      slices.append().Reset();
    }
  }
  return index;
}

inline unsigned
ThermalEncounterBand::FindPenultimateFinished(const unsigned index,
                                              const FloatDuration time) noexcept
{
  // find penultimate completed slice up to index
  unsigned prev = 0;
  while (prev + 2 < index && slices[prev + 2].n == 1. &&
         slices[prev+1].time < time) {
    prev++;
  }
  return prev;
}

inline FloatDuration
ThermalEncounterBand::EstimateTimeStep(const FloatDuration time,
                                       const double height,
                                       const unsigned index) noexcept
{
  // time since completed slice
  const auto dt_comp = time-slices[index].time;
  assert(dt_comp.count() > 0);

  // height since completed slice
  const double dh_comp = height-GetSliceHeight(index);
  assert(dh_comp > 0);

  // average time step across unfinished slices,
  // assuming constant climb rate since last completed
  return dt_comp * (dh / dh_comp);
}

inline void
ThermalEncounterBand::Start(const TimeStamp time,
                            const double height) noexcept
{
  Reset();
  time_start = time;
  ThermalSlice s;
  s.SetSample({});
  slices.append(s);
  h_min = height;
}
