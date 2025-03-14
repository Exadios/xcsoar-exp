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

#include "VegaParametersWidget.hpp"
#include "Language/Language.hpp"

static constexpr
VegaParametersWidget::StaticParameter limit_parameters[] = {
  { DataField::Type::INTEGER, "VelocityNeverExceed", N_("VNE"), NULL,
    NULL, 0, 2000, 10, _T("%d (0.1 m/s)") },
  { DataField::Type::INTEGER, "VelocitySafeTerrain", N_("V terrain"), NULL,
    NULL, 0, 2000, 10, _T("%d (0.1 m/s)") },
  { DataField::Type::INTEGER, "VelocitySafeTerrain", N_("Height terrain"),
    NULL, NULL, 0, 2000, 10, _T("%d m") },
  { DataField::Type::INTEGER, "VelocityManoeuvering", N_("V manoeuvering"),
    NULL, NULL, 0, 2000, 10, _T("%d (0.1 m/s)") },
  { DataField::Type::INTEGER, "VelocityAirBrake", N_("V airbrake"),
    NULL, NULL, 0, 2000, 10, _T("%d (0.1 m/s)") },
  { DataField::Type::INTEGER, "VelocityFlap", N_("V flap"),
    NULL, NULL, 0, 2000, 10, _T("%d (0.1 m/s)") },

  /* sentinel */
  { DataField::Type::BOOLEAN }
};
