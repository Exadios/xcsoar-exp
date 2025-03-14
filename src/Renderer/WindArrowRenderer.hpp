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

#include <cstdint>

class Canvas;
class Angle;
struct PixelPoint;
struct PixelRect;
struct WindArrowLook;
struct SpeedVector;
struct DerivedInfo;
struct MapSettings;
enum class WindArrowStyle : uint8_t;

class WindArrowRenderer {
  const WindArrowLook &look;

public:
  explicit WindArrowRenderer(const WindArrowLook &_look) noexcept
    :look(_look) {}

  void Draw(Canvas &canvas, Angle screen_angle, SpeedVector wind,
            PixelPoint pos, const PixelRect &rc, WindArrowStyle arrow_style) noexcept;

  void Draw(Canvas &canvas, Angle screen_angle, PixelPoint pos,
            const PixelRect &rc, const DerivedInfo &calculated,
            const MapSettings &settings) noexcept;

  void DrawArrow(Canvas &canvas, PixelPoint pos, Angle angle,
                 unsigned width, unsigned length, unsigned tail_length,
                 WindArrowStyle arrow_style,
                 int offset,
                 unsigned scale) noexcept;
};
