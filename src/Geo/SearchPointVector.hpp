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

#include "SearchPoint.hpp"

#include <vector>

class FlatRay;
struct FlatBoundingBox;
class GeoBounds;

class SearchPointVector: public std::vector<SearchPoint> {
public:
  template<typename... Args>
  SearchPointVector(Args&&... args) noexcept
    :std::vector<SearchPoint>(std::forward<Args>(args)...) {}

  bool PruneInterior() noexcept;

  /**
   * Apply convex pruning algorithm with increasing tolerance
   * until the trace is smaller than the given size
   *
   * @return True if input was modified
   */
  bool ThinToSize(const unsigned max_size) noexcept;

  void Project(const FlatProjection &tp) noexcept;

  [[gnu::pure]]
  FlatGeoPoint NearestPoint(const FlatGeoPoint &p) const noexcept;

  /** Find iterator of nearest point, assuming polygon is convex */
  [[gnu::pure]]
  const_iterator NearestIndexConvex(const FlatGeoPoint &p) const noexcept;

  [[gnu::pure]]
  bool IntersectsWith(const FlatRay &ray) const noexcept;

  [[gnu::pure]]
  FlatBoundingBox CalculateBoundingbox() const noexcept;

  [[gnu::pure]]
  GeoBounds CalculateGeoBounds() const noexcept;

  /** increment iterator, wrapping around to start if required */
  [[gnu::pure]]
  const_iterator NextCircular(const_iterator i) const noexcept;

  /** decreement iterator, wrapping around to last item if required */
  [[gnu::pure]]
  const_iterator PreviousCircular(const_iterator i) const noexcept;

  /** Is the given GeoPoint inside the polygon of SearchPoints? */
  [[gnu::pure]]
  bool IsInside(const GeoPoint &pt) const noexcept;

  /** Is the given FlatGeoPoint inside the polygon of SearchPoints? */
  [[gnu::pure]]
  bool IsInside(const FlatGeoPoint &pt) const noexcept;
};
