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

#include "ui/dim/Rect.hpp"
#include "util/StaticArray.hxx"

/**
 * Simple code to prevent text writing over map city names.
 */
class LabelBlock {
  static constexpr unsigned SCREEN_HEIGHT = 4096;
  static constexpr unsigned BUCKET_SIZE = 64;
  static constexpr unsigned BUCKET_SHIFT = 7;
  static constexpr unsigned BUCKET_HEIGHT = 1 << BUCKET_SHIFT;
  static constexpr unsigned BUCKET_COUNT = SCREEN_HEIGHT / BUCKET_HEIGHT;

  /**
   * A bucket is responsible for hit tests in one horizontal section
   * of the screen.
   */
  class Bucket {
    typedef StaticArray<PixelRect, BUCKET_SIZE> BlockArray;
    BlockArray blocks;

  public:
    void Clear() noexcept {
      blocks.clear();
    }

    [[gnu::pure]]
    bool Check(const PixelRect rc) const noexcept;

    void Add(const PixelRect rc) noexcept {
      if (!blocks.full())
        blocks.append(rc);
    }
  };

  Bucket buckets[BUCKET_COUNT];

public:
  bool check(const PixelRect rc) noexcept;
  void reset() noexcept;
};
