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

#include "util/NonCopyable.hpp"

struct WaypointRendererSettings;
struct WaypointLook;
class Canvas;
class LabelBlock;
class MapWindowProjection;
class Waypoints;
struct PolarSettings;
struct TaskBehaviour;
struct MoreData;
struct DerivedInfo;
class ProtectedTaskManager;
class ProtectedRoutePlanner;

/**
 * Renders way point icons and labels into a #Canvas.
 */
class WaypointRenderer : private NonCopyable {
  const Waypoints *way_points;

  const WaypointLook &look;

public:
  WaypointRenderer(const Waypoints *_way_points,
                   const WaypointLook &_look) noexcept
    :way_points(_way_points), look(_look) {}

  const WaypointLook &GetLook() const noexcept {
    return look;
  }

  void SetWaypoints(const Waypoints *_way_points) noexcept {
    way_points = _way_points;
  }

  void Render(Canvas &canvas, LabelBlock &label_block,
              const MapWindowProjection &projection,
              const WaypointRendererSettings &settings,
              const PolarSettings &polar_settings,
              const TaskBehaviour &task_behaviour,
              const MoreData &basic, const DerivedInfo &calculated,
              const ProtectedTaskManager *task,
              const ProtectedRoutePlanner *route_planner) noexcept;
};
