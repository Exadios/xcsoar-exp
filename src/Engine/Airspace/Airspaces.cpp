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

#include "Airspaces.hpp"
#include "AbstractAirspace.hpp"
#include "AirspaceIntersectionVisitor.hpp"
#include "Navigation/Aircraft.hpp"

#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/geometries/segment.hpp>

namespace bgi = boost::geometry::index;

Airspaces::~Airspaces() noexcept = default;

Airspaces::const_iterator_range
Airspaces::QueryWithinRange(const GeoPoint &location, double range) const noexcept
{
  if (IsEmpty())
    // nothing to do
    return {airspace_tree.qend(), airspace_tree.qend()};

  const FlatBoundingBox box = task_projection.ProjectSquare(location, range);
  return {airspace_tree.qbegin(bgi::intersects(box)), airspace_tree.qend()};
}

Airspaces::const_iterator_range
Airspaces::QueryIntersecting(const GeoPoint &a, const GeoPoint &b) const noexcept
{
  if (IsEmpty())
    // nothing to do
    return {airspace_tree.qend(), airspace_tree.qend()};

  const boost::geometry::model::segment line{
    task_projection.ProjectInteger(a),
    task_projection.ProjectInteger(b),
  };

  return {airspace_tree.qbegin(bgi::intersects(line)), airspace_tree.qend()};
}

void
Airspaces::VisitIntersecting(const GeoPoint &loc, const GeoPoint &end,
                             bool include_inside,
                             AirspaceIntersectionVisitor &visitor) const noexcept
{
  for (const auto &i : QueryIntersecting(loc, end))
    if (visitor.SetIntersections(i.Intersects(loc, end, task_projection)))
      visitor.Visit(i.GetAirspacePtr());

  if (include_inside) {
    for (const auto &i : QueryInside(loc)) {
      if (i.IsInside(end)) {
        /* the vector is completely inside the airspace, and thus does
           not intersect with airspace's outline: on caller's request,
           report an intersection */
        AirspaceIntersectionVector v;
        v.reserve(1);
        v.emplace_back(loc, end);
        visitor.SetIntersections(std::move(v));
        visitor.Visit(i.GetAirspacePtr());
      }
    }
  }
}

void
Airspaces::Optimise() noexcept
{
  if (IsEmpty())
    /* avoid assertion failure in uninitialised task_projection */
    return;

  if (task_projection.Update()) {
    // task projection changed, so need to push items back onto stack
    // to re-build airspace envelopes

    for (const auto &i : QueryAll())
      tmp_as.push_back(i.GetAirspacePtr());

    airspace_tree.clear();
  }

  for (auto &i : tmp_as) {
    Airspace as(std::move(i), task_projection);
    airspace_tree.insert(as);
  }

  tmp_as.clear();

  ++serial;
}

void
Airspaces::Add(AirspacePtr airspace) noexcept
{
  if (!airspace)
    // nothing to add
    return;

  // reset QNH to zero so set_pressure_levels will be triggered next update
  // this allows for airspaces to be add at any time
  qnh = AtmosphericPressure::Zero();

  // reset day to all so set_activity will be triggered next update
  // this allows for airspaces to be add at any time
  activity_mask.SetAll();

  if (IsEmpty())
    task_projection.Reset(airspace->GetReferenceLocation());

  task_projection.Scan(airspace->GetReferenceLocation());

  tmp_as.push_back(std::move(airspace));
}

void
Airspaces::Clear() noexcept
{
  // delete temporaries in case they were added without an optimise() call
  tmp_as.clear();

  // then delete the tree
  airspace_tree.clear();
}

unsigned
Airspaces::GetSize() const noexcept
{
  return airspace_tree.size();
}

bool
Airspaces::IsEmpty() const noexcept
{
  return airspace_tree.empty() && tmp_as.empty();
}

void
Airspaces::SetFlightLevels(const AtmosphericPressure press) noexcept
{
  if ((int)press.GetHectoPascal() != (int)qnh.GetHectoPascal()) {
    qnh = press;

    for (auto &v : QueryAll())
      v.SetFlightLevel(press);
  }
}

void
Airspaces::SetActivity(const AirspaceActivity mask) noexcept
{
  if (!mask.equals(activity_mask)) {
    activity_mask = mask;

    for (auto &v : QueryAll())
      v.SetActivity(mask);
  }
}

void
Airspaces::ClearClearances() noexcept
{
  for (auto &v : QueryAll())
    v.ClearClearance();
}

[[gnu::pure]]
static bool
AirspacePointersEquals(const Airspace &a, const Airspace &b) noexcept
{
  return &a.GetAirspace() == &b.GetAirspace();
}

[[gnu::pure]]
static bool
CompareAirspaceVectors(const AirspacesInterface::AirspaceVector &a,
                       const AirspacesInterface::AirspaceVector &b) noexcept
{
  return a.size() == b.size() &&
    std::is_permutation(a.begin(), a.end(), b.begin(), AirspacePointersEquals);
}

inline AirspacesInterface::AirspaceVector
Airspaces::AsVector() const noexcept
{
  AirspaceVector v;
  v.reserve(airspace_tree.size());

  for (const auto &i : QueryAll())
    v.push_back(i);

  return v;
}

bool
Airspaces::SynchroniseInRange(const Airspaces &master,
                              const GeoPoint &location,
                              const double range,
                              AirspacePredicate condition) noexcept
{
  qnh = master.qnh;
  activity_mask = master.activity_mask;
  task_projection = master.task_projection;

  AirspaceVector contents_master;
  for (const auto &i : master.QueryWithinRange(location, range))
    if (condition(i.GetAirspace()))
      contents_master.push_back(i);

  if (CompareAirspaceVectors(contents_master, AsVector()))
    return false;

  for (auto &i : QueryAll())
    i.ClearClearance();
  airspace_tree.clear();

  for (const auto &i : contents_master)
    airspace_tree.insert(i);

  ++serial;

  return true;
}

Airspaces::const_iterator_range
Airspaces::QueryInside(const GeoPoint &loc) const noexcept
{
  if (IsEmpty())
    // nothing to do
    return {airspace_tree.qend(), airspace_tree.qend()};

  const auto flat_location = task_projection.ProjectInteger(loc);
  const FlatBoundingBox box(flat_location, flat_location);

  const auto _begin =
    airspace_tree.qbegin(bgi::intersects(box) &&
                         bgi::satisfies([&loc](const Airspace &as){
                             return as.IsInside(loc);
                           }));

  return {_begin, airspace_tree.qend()};
}

Airspaces::const_iterator_range
Airspaces::QueryInside(const AircraftState &aircraft) const noexcept
{
  if (IsEmpty())
    // nothing to do
    return {airspace_tree.qend(), airspace_tree.qend()};

  const auto flat_location = task_projection.ProjectInteger(aircraft.location);
  const FlatBoundingBox box(flat_location, flat_location);

  const auto _begin =
    airspace_tree.qbegin(bgi::intersects(box) &&
                         bgi::satisfies([&aircraft](const Airspace &as){
                             return as.IsInside(aircraft);
                           }));

  return {_begin, airspace_tree.qend()};
}
