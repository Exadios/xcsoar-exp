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

#include "Bitmap.hpp"
#include "ui/dim/Point.hpp"
#include "ui/dim/Size.hpp"
#include "ResourceId.hpp"

struct PixelRect;
class Canvas;

/**
 * An icon with a mask which marks transparent pixels.
 */
class MaskedIcon {
protected:
  Bitmap bitmap;

  PixelSize size;

  PixelPoint origin;

public:
  const PixelSize &GetSize() const noexcept {
    return size;
  }

  bool IsDefined() const noexcept {
    return bitmap.IsDefined();
  }

  void LoadResource(ResourceId id, ResourceId big_id = ResourceId::Null(),
                    bool center=true);

  void Reset() noexcept {
    bitmap.Reset();
  }

  void Draw(Canvas &canvas, PixelPoint p) const noexcept;

  void Draw(Canvas &canvas, const PixelRect &rc, bool inverse) const noexcept;
};
