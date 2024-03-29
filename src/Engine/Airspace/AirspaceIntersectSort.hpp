// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "Geo/GeoPoint.hpp"

#include <optional>
#include <queue>

class AbstractAirspace;
class AirspaceIntersectionVector;

/**
 * Utility class to sort airspaces in ascending order of vector parameter (0,1)
 */
class AirspaceIntersectSort {
  using Intersection = std::pair<double, GeoPoint>;

  /**
   * Function object used to rank intercepts by vector parameter t(0,1)
   */
  struct Rank {
    bool operator()(const Intersection &x,
                    const Intersection &y) const noexcept {
      return x.first > y.first;
    }
  };

  std::priority_queue<Intersection, std::vector<Intersection>, Rank> m_q;

  const GeoPoint& m_start;
  const AbstractAirspace &airspace;

public:
  /**
   * Constructor
   *
   * @param start Location of start point
   * @param the_airspace Airspace to test for intersections
   */
  AirspaceIntersectSort(const GeoPoint &start,
                        const AbstractAirspace &the_airspace) noexcept
    :m_start(start), airspace(the_airspace) {}

  /**
   * Add point to queue
   *
   * @param t Ray parameter [0,1]
   * @param p Point of intersection
   */
  void add(const double t, const GeoPoint &p) noexcept;

  /**
   * Determine if no points are found
   *
   * @return True if no points added
   */
  bool empty() const noexcept {
    return m_q.empty();
  }

  /**
   * Return closest intercept point (or location if inside)
   */
  [[gnu::pure]]
  std::optional<GeoPoint> top() const noexcept;

  /**
   * Return vector of pairs of enter/exit intersections.
   *
   * @return vector
   */
  AirspaceIntersectionVector all() noexcept;
};
