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

#include "Weather/Features.hpp"
#include "net/http/Features.hpp"

#ifdef HAVE_PCMET

#include "PCMet/Settings.hpp"

#endif

struct WeatherSettings {
#ifdef HAVE_PCMET
  PCMetSettings pcmet;
#endif

#ifdef HAVE_HTTP
  /**
   * Enable Thermal Information Map?
   */
  bool enable_tim;
#endif

  void SetDefaults() {
#ifdef HAVE_PCMET
    pcmet.SetDefaults();
#endif

#ifdef HAVE_HTTP
    enable_tim = false;
#endif
  }
};
