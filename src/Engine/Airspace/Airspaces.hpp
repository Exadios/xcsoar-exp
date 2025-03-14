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

#include "Predicate/AirspacePredicate.hpp"
#include "AirspacesInterface.hpp"
#include "AirspaceActivity.hpp"
#include "Ptr.hpp"
#include "util/Serial.hpp"
#include "Geo/Flat/TaskProjection.hpp"
#include "Atmosphere/Pressure.hpp"

#include <deque>

class RasterTerrain;
class AirspaceIntersectionVisitor;

/**
 * Container for airspaces using kd-tree representation internally for
 * fast geospatial lookups.
 */
class Airspaces : public AirspacesInterface {
  AtmosphericPressure qnh = AtmosphericPressure::Zero();
  AirspaceActivity activity_mask;

  AirspaceTree airspace_tree;
  TaskProjection task_projection;

  std::deque<AirspacePtr> tmp_as;

  /**
   * This attribute keeps track of changes to this project.  It is
   * used by the renderer cache.
   */
  Serial serial;

public:
  /**
   * Constructor.
   * Note this class can't safely be copied (yet)
   *
   * @return empty Airspaces class.
   */
  Airspaces() noexcept {}

  ~Airspaces() noexcept;

  Airspaces(const Airspaces &) = delete;
  Airspaces &operator=(const Airspaces &) = delete;

  const Serial &GetSerial() const noexcept {
    return serial;
  }

  /**
   * Add airspace to the internal airspace tree.
   * The airspace is not copied; ownership is transferred to this class if
   * m_owner is true
   *
   * @param asp New airspace to be added.
   */
  void Add(AirspacePtr airspace) noexcept;

  /**
   * Re-organise the internal airspace tree after inserting/deleting.
   * Should be called after inserting/deleting airspaces prior to performing
   * any searches, but can be done once after a batch insert/delete.
   */
  void Optimise() noexcept;

  /**
   * Clear the airspace store, deleting airspace objects if m_owner is true
   */
  void Clear() noexcept;

  /**
   * Size of airspace (in tree, not in temporary store) ---
   * must call optimise() before this for it to be accurate.
   *
   * @return Number of airspaces in tree
   */
  [[gnu::pure]]
  unsigned GetSize() const noexcept;

  /**
   * Whether airspace store is empty
   *
   * @return True if no airspace stored
   */
  [[gnu::pure]]
  bool IsEmpty() const noexcept;

  /**
   * Set terrain altitude for all AGL-referenced airspace altitudes
   *
   * @param terrain Terrain model for lookup
   */
  void SetGroundLevels(const RasterTerrain &terrain) noexcept;

  /**
   * Set QNH pressure for all FL-referenced airspace altitudes.
   * Doesn't do anything if QNH is unchanged
   *
   * @param press Atmospheric pressure model and QNH
   */
  void SetFlightLevels(AtmosphericPressure press) noexcept;

  /**
   * Set activity based on day mask
   *
   * @param days Mask of activity (a particular or range of days matching this day)
   */
  void SetActivity(const AirspaceActivity mask) noexcept;

  [[gnu::pure]]
  const_iterator_range QueryAll() const noexcept {
    auto predicate = boost::geometry::index::satisfies([](const Airspace &){
        return true;
      });
    return {airspace_tree.qbegin(predicate), airspace_tree.qend()};
  }

  /**
   * Query airspaces within range of location.
   *
   * @param loc location of origin of search
   * @param range distance in meters of search radius
   */
  [[gnu::pure]]
  const_iterator_range QueryWithinRange(const GeoPoint &location,
                                        double range) const noexcept;

  /**
   * Query airspaces intersecting the vector (bounding box check
   * only).  The result is in no specific order.
   */
  [[gnu::pure]]
  const_iterator_range QueryIntersecting(const GeoPoint &a,
                                         const GeoPoint &b) const noexcept;

  /**
   * Call visitor class on airspaces intersected by vector.
   * Note that the visitor is not instantiated separately for each match
   *
   * @param loc location of origin of search
   * @param end end of line along with to search for intersections
   * @param include_inside visit airspaces if the vector is completely
   * inside (i.e. no intersection with airspace outline)
   * @param visitor visitor class to call on airspaces intersected by line
   */
  void VisitIntersecting(const GeoPoint &location, const GeoPoint &end,
                         bool include_inside,
                         AirspaceIntersectionVisitor &visitor) const noexcept;

  void VisitIntersecting(const GeoPoint &location, const GeoPoint &end,
                         AirspaceIntersectionVisitor &visitor) const noexcept {
    VisitIntersecting(location, end, false, visitor);
  }

  /**
   * Query airspaces this location is inside.
   *
   * @param loc location of origin of search
   */
  [[gnu::pure]]
  const_iterator_range QueryInside(const GeoPoint &location) const noexcept;

  /**
   * Query airspaces the aircraft is inside (taking altitude into
   * account).
   *
   * @param loc location of origin of search
   */
  [[gnu::pure]]
  const_iterator_range QueryInside(const AircraftState &aircraft) const noexcept;

  const FlatProjection &GetProjection() const noexcept {
    return task_projection;
  }

  /**
   * Empty clearance polygons of all airspaces in this database
   */
  void ClearClearances() noexcept;

  /**
   * Copy/delete objects in this database based on query of master
   *
   * @param master Airspaces object to copy from
   * @param location location of aircraft, from which to search
   * @param range distance in meters of search radius
   * @param condition condition to be applied to matches
   *
   * @return True on change
   */
  bool SynchroniseInRange(const Airspaces &master,
                          const GeoPoint &location, double range,
                          AirspacePredicate condition) noexcept;

private:
  [[gnu::pure]]
  AirspaceVector AsVector() const noexcept;
};
