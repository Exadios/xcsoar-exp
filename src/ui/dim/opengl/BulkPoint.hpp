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

#include "ui/opengl/Types.hpp"
#include "ui/dim/Point.hpp"
#include "Math/Point2D.hpp"

/**
 * A point structure to be used in arrays.
 */
struct BulkPixelPoint : Point2D<GLvalue, int> {
  BulkPixelPoint() = default;

  constexpr BulkPixelPoint(int _x, int _y) noexcept
    :Point2D(_x, _y) {}

  constexpr BulkPixelPoint(PixelPoint src)
    :Point2D(src.x, src.y) {}

  constexpr operator PixelPoint() const {
    return PixelPoint(x, y);
  }
};
