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

#pragma once

#include "AirspaceClass.hpp"

#include <xcsoar-cassert>
#include <chrono>

struct AirspaceWarningConfig
{
  using Duration = std::chrono::duration<unsigned>;

  /** Warning time before airspace entry */
  Duration warning_time;

  /** Enable repetitive sound */
  bool repetitive_sound;

  /** Time an acknowledgement will persist before a warning is reissued */
  Duration acknowledgement_time;

  /** Altitude margin (m) outside of which to not display airspace for auto mode */
  unsigned altitude_warning_margin;

  /** Class-specific warning flags */
  bool class_warnings[AIRSPACECLASSCOUNT];

  void SetDefaults();

  bool IsClassEnabled(AirspaceClass cls) const {
    assert((unsigned)cls < AIRSPACECLASSCOUNT);

    return class_warnings[(unsigned)cls];
  }
};
