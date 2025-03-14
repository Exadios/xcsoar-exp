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

#include "Computer/Wind/Computer.hpp"
#include "Computer/CirclingComputer.hpp"
#include "Computer/Settings.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "system/Args.hpp"
#include "DebugReplay.hpp"

#include <stdio.h>

int main(int argc, char **argv)
{
  Args args(argc, argv, "DRIVER FILE");
  DebugReplay *replay = CreateDebugReplay(args);
  if (replay == NULL)
    return EXIT_FAILURE;

  args.ExpectEnd();

  printf("# time wind_bearing (deg) wind_speed (m/s)\n");

  GlidePolar glide_polar(0);

  CirclingSettings circling_settings;

  WindSettings wind_settings;
  wind_settings.SetDefaults();

  CirclingComputer circling_computer;
  circling_computer.Reset();

  WindComputer wind_computer;
  wind_computer.Reset();

  Validity last;
  last.Clear();

  while (replay->Next()) {
    const MoreData &basic = replay->Basic();
    const DerivedInfo &calculated = replay->Calculated();

    circling_computer.TurnRate(replay->SetCalculated(),
                               basic, calculated.flight);
    circling_computer.Turning(replay->SetCalculated(),
                              basic,
                              calculated.flight,
                              circling_settings);

    wind_computer.Compute(wind_settings, glide_polar, basic,
                          replay->SetCalculated());

    if (calculated.estimated_wind_available.Modified(last)) {
      TCHAR time_buffer[32];
      FormatTime(time_buffer, replay->Basic().time);

      _tprintf(_T("%s %d %g\n"),
               time_buffer, (int)calculated.estimated_wind.bearing.Degrees(),
               (double)calculated.estimated_wind.norm);
    }

    last = calculated.estimated_wind_available;
  }

  delete replay;
}
