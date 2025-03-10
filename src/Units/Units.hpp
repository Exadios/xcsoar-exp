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

#include "Units/Settings.hpp"
#include "Units/System.hpp"

#include <tchar.h>

class AtmosphericPressure;

#define DEG "°"

/**
 * Namespace to manage unit conversions.
 * internal system units are (metric SI).
 */
namespace Units
{
  extern UnitSetting current;

  void SetConfig(const UnitSetting &new_config);

  /**
   * Returns the user-specified unit for a wing loading
   * @return The user-specified unit for a wing loading
   */
  [[gnu::pure]]
  Unit GetUserWingLoadingUnit();

  /**
   * Returns the user-specified unit for mass
   * @return The user-specified unit for mass
   */
  [[gnu::pure]]
  Unit GetUserMassUnit();

  /**
   * Returns the user-specified unit for a horizontal distance
   * @return The user-specified unit for a horizontal distance
   */
  [[gnu::pure]]
  Unit GetUserDistanceUnit();

  /**
   * Returns the user-specified unit for an altitude
   * @return The user-specified unit for an altitude
   */
  [[gnu::pure]]
  Unit GetUserAltitudeUnit();

  /**
   * Returns the user-specified unit for a temperature
   * @return The user-specified unit for a temperature
   */
  [[gnu::pure]]
  Unit GetUserTemperatureUnit();

  /**
   * Returns the user-specified unit for a horizontal speed
   * @return The user-specified unit for a horizontal speed
   */
  [[gnu::pure]]
  Unit GetUserSpeedUnit();

  /**
   * Returns the user-specified unit for a task speed
   * @return The user-specified unit for a task speed
   */
  [[gnu::pure]]
  Unit GetUserTaskSpeedUnit();

  /**
   * Returns the user-specified unit for a vertical speed
   * @return The user-specified unit for a vertical speed
   */
  [[gnu::pure]]
  Unit GetUserVerticalSpeedUnit();

  /**
   * Returns the user-specified unit for a wind speed
   * @return The user-specified unit for a wind speed
   */
  [[gnu::pure]]
  Unit GetUserWindSpeedUnit();

  /**
   * Returns the user-specified unit for a pressure
   * @return The user-specified unit for a pressure
   */
  [[gnu::pure]]
  Unit GetUserPressureUnit();

  [[gnu::pure]]
  Unit GetUserUnitByGroup(UnitGroup group);

  [[gnu::pure]]
  const TCHAR *GetSpeedName();

  [[gnu::pure]]
  const TCHAR *GetVerticalSpeedName();

  [[gnu::pure]]
  const TCHAR *GetWindSpeedName();

  [[gnu::pure]]
  const TCHAR *GetDistanceName();

  [[gnu::pure]]
  const TCHAR *GetAltitudeName();

  [[gnu::pure]]
  const TCHAR *GetTemperatureName();

  [[gnu::pure]]
  const TCHAR *GetTaskSpeedName();

  [[gnu::pure]]
  const TCHAR *GetPressureName();

  static inline double
  ToUserAltitude(double value)
  {
    return ToUserUnit(value, current.altitude_unit);
  }

  static inline double
  ToSysAltitude(double value)
  {
    return ToSysUnit(value, current.altitude_unit);
  }

  static inline double
  ToUserDistance(double value)
  {
    return ToUserUnit(value, current.distance_unit);
  }

  static inline double
  ToSysDistance(double value)
  {
    return ToSysUnit(value, current.distance_unit);
  }

  static inline double
  ToUserSpeed(double value)
  {
    return ToUserUnit(value, current.speed_unit);
  }

  static inline double
  ToSysSpeed(double value)
  {
    return ToSysUnit(value, current.speed_unit);
  }

  static inline double
  ToUserVSpeed(double value)
  {
    return ToUserUnit(value, current.vertical_speed_unit);
  }

  static inline double
  ToSysVSpeed(double value)
  {
    return ToSysUnit(value, current.vertical_speed_unit);
  }

  static inline double
  ToUserTaskSpeed(double value)
  {
    return ToUserUnit(value, current.task_speed_unit);
  }

  static inline double
  ToSysTaskSpeed(double value)
  {
    return ToSysUnit(value, current.task_speed_unit);
  }

  static inline double
  ToUserWindSpeed(double value)
  {
    return ToUserUnit(value, current.wind_speed_unit);
  }

  static inline double
  ToSysWindSpeed(double value)
  {
    return ToSysUnit(value, current.wind_speed_unit);
  }

  static inline double
  ToUserPressure(double Value)
  {
    return ToUserUnit(Value, current.pressure_unit);
  }
  
  [[gnu::const]]
  double
  ToUserPressure(AtmosphericPressure value);

  static inline double
  ToSysPressure(double Value)
  {
    return ToSysUnit(Value, current.pressure_unit);
  }

  /**
   * Convert a pressure value from the user unit to an
   * #AtmosphericPressure object.
   */
  [[gnu::const]]
  AtmosphericPressure
  FromUserPressure(double value);
  
  static inline double
  ToUserMass(double Value)
  {
    return ToUserUnit(Value, current.mass_unit);
  }
  
  static inline double
  ToSysMass(double Value)
  {
    return ToSysUnit(Value, current.mass_unit);
  }
    
};
