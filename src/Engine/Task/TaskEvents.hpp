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

class TaskWaypoint;
struct Waypoint;

/**
 * Class used to provide feedback based on events that can be triggered
 * by the task system.  Typically this would be specialised by the client
 * to hook up to end-user code.
 */
class TaskEvents
{
public:
  /**
   * Called when the aircraft enters a turnpoint observation zone
   *
   * @param tp The turnpoint entered
   */
  virtual void EnterTransition([[maybe_unused]] const TaskWaypoint &tp) noexcept {}

  /**
   * Called when the aircraft exits a turnpoint observation zone
   *
   * @param tp The turnpoint the aircraft has exited
   */
  virtual void ExitTransition([[maybe_unused]] const TaskWaypoint &tp) noexcept {}

  /**
   * Called when auto-advance has changed the active
   * task point in an ordered task
   *
   * @param tp The turnpoint that is now active after auto-advance
   * @param i The task sequence number after auto-advance
   */
  virtual void ActiveAdvanced([[maybe_unused]] const TaskWaypoint &tp,
                              [[maybe_unused]] const int i) noexcept {}

  /**
   * Called when a task point can be advanced but the advance needs
   * to be armed
   *
   * @param tp The taskpoint waiting to be armed
   */
  virtual void RequestArm([[maybe_unused]] const TaskWaypoint &tp) noexcept {}

  /**
   * Called when orderd task has started
   */
  virtual void TaskStart() noexcept {}

  /**
   * Called when orderd task has finished
   */
  virtual void TaskFinish() noexcept {}
};
