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

#include "Airspace.hpp"
#include "Geo/Flat/BoostFlatBoundingBox.hpp"

#include <boost/geometry/index/rtree.hpp>
#include <boost/range/iterator_range_core.hpp>

/**
 * Abstract class for interface to #Airspaces database.
 * Allows references to the #Airspaces to be substituted for a
 * facade protected class where locking is required.
 */
class AirspacesInterface {
  struct AirspaceIndexable {
    using result_type = FlatBoundingBox;

    result_type operator()(const Airspace &airspace) const {
      return airspace;
    }
  };

public:
  using AirspaceVector = std::vector<Airspace>; /**< Vector of airspaces (used internally) */

  /**
   * Type of KD-tree data structure for airspace container
   */
  using AirspaceTree =
    boost::geometry::index::rtree<Airspace, boost::geometry::index::rstar<16>,
                                  AirspaceIndexable> ;

  using const_iterator = AirspaceTree::const_query_iterator;
  using const_iterator_range = boost::iterator_range<const_iterator>;
};
