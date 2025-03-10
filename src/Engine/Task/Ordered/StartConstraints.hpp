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

#include "time/RoughTime.hpp"
#include "Geo/AltitudeReference.hpp"

struct AircraftState;
struct TaskStartMargins;

struct StartConstraints {
  /**
   * The time span during which the start gate is open.
   */
  RoughTimeSpan open_time_span;

  /** Maximum ground speed (m/s) allowed in start sector */
  double max_speed;

  /** Maximum height (m) allowed in start sector */
  unsigned max_height;

  /** Reference for max start height */
  AltitudeReference max_height_ref;

  /**
   * Does the start point need to be armed manually?
   *
   * This defaults to #TaskFactoryConstraints::start_requires_arm
   */
  bool require_arm;

  /**
   * If this is true, then exiting the observation zone is the goal,
   * not entering it.
   */
  bool score_exit;

  /**
   * This is a copy of FinishConstraints::fai_finish.  If true, then
   * the constraints defined in this class will be ignored.
   */
  bool fai_finish;

  /**
   * Wait time in minutes after Pilot Event (PEV) and start gate open time.
   */
  std::chrono::duration<unsigned> pev_start_wait_time;

  /**
   * Time in minutes start gate remains open after Pilot Event and PEV Wait
   * Time.
   */
  std::chrono::duration<unsigned> pev_start_window;

  void SetDefaults();

  /**
   * Check whether aircraft speed is within start speed limits
   *
   * @param ground_speed the aircraft's ground speed
   * @param margins if not nullptr, the given margins are allowed for
   * minor rule violation
   *
   * @return True if within limits
   */
  [[gnu::pure]]
  bool CheckSpeed(double ground_speed,
                  const TaskStartMargins *margins=nullptr) const;

  /**
   * Check whether aircraft height is within start height limit
   *
   * @param state Aircraft state
   * @param start_elevation start point elevation
   * @param margins if not nullptr, the given margins are allowed for
   * minor rule violation
   *
   * @return True if within limits
   */
  [[gnu::pure]]
  bool CheckHeight(const AircraftState &state,
                   double start_elevation,
                   const TaskStartMargins *margins=nullptr) const;
};
