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

#include "Geo/SpeedVector.hpp"
#include "Geo/GeoVector.hpp"
#include "time/FloatDuration.hxx"

struct AircraftState;
class TaskPoint;

/**
 * Class used to define a glide/navigation task
 */
struct GlideState {
  /** Distance/bearing of task */
  GeoVector vector;

  /**
   * The minimum altitude for arrival at the target (i.e. target
   * altitude plus safety margin).
   */
  double min_arrival_altitude;

  /** Wind vector (deg True) */
  SpeedVector wind;
  /** Aircraft height less target height */
  double altitude_difference;

  /** (internal use) */
  Angle effective_wind_angle;
  /** headwind component (m/s) in cruise */
  double head_wind;

private:
  /** (internal use) */
  double wind_speed_squared;

public:
  /**
   * Dummy task constructor.  Typically used for synthetic glide
   * tasks.  Where there are real targets, the other constructors should
   * be used instead.
   *
   * @param vector Specified vector for task
   * @param htarget Height of target (m above MSL)
   * @param altitude the altitude of the aircraft
   * @param wind the wind vector
   *
   * @return Initialised glide task
   */
  GlideState(const GeoVector &vector, const double htarget,
             double altitude, const SpeedVector wind);

  [[gnu::pure]]
  static GlideState Remaining(const TaskPoint &tp,
                              const AircraftState &aircraft,
                              double min_h);

  /**
   * Calculate internal quantities to reduce computation time
   * by clients of this class
   *
   * @param wind the wind vector
   */
  void CalcSpeedups(const SpeedVector wind);

  /**
   * Calculates average cross-country speed from effective
   * cross-country speed (accounting for wind)
   *
   * @param Veff Effective cruise speed (m/s)
   *
   * @return Average cross-country speed (m/s)
   */
  [[gnu::pure]]
  double CalcAverageSpeed(double v_eff) const;

  /**
   * Calculate distance a circling aircraft will drift
   * in a given time
   *
   * @param t_climb Time spent in climb (s)
   *
   * @return Distance (m) of drift
   */
  [[gnu::pure]]
  double DriftedDistance(FloatDuration climb_time) const;
};
