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

#include "util/StaticString.hxx"
#include "Polar/Shape.hpp"

struct Plane
{
  StaticString<32> registration;
  StaticString<6> competition_id;
  StaticString<32> type;

  StaticString<32> polar_name;

  PolarShape polar_shape;

  double empty_mass;
  double dry_mass_obsolete; // unused entry for plane file compatibility. to be removed 2023..
  double max_ballast;
  double max_speed;
  double wing_area;

  /** Time to drain full ballast (s) */
  unsigned dump_time;

  unsigned handicap;

  /**
   * Type of glider from a list, published by WeGlide server to select
   * the correct glider id for the flight to upload.  The list is
   * published on https://raw.githubusercontent.com/ the data of the
   * selected glider you can find on
   * https://api.weglide.org/v1/aircraft/$(ID)
   */
  unsigned weglide_glider_type;
};
