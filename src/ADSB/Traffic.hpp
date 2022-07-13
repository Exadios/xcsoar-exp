/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#ifndef XCSOAR_ADSB_TRAFFIC_HPP
#define XCSOAR_ADSB_TRAFFIC_HPP

#include "FLARM/FlarmId.hpp"
#include "Geo/GeoPoint.hpp"
#include "NMEA/Validity.hpp"
#include "util/StaticString.hxx"
#include "Rough/RoughAltitude.hpp"
#include "Rough/RoughDistance.hpp"
#include "Rough/RoughSpeed.hpp"
#include "Rough/RoughAngle.hpp"

#include <type_traits>

#include <tchar.h>

/**
 * @file
 */

struct AdsbTraffic
  {
  enum class AlarmType: uint8_t
    {
    NONE = 0,
    LOW = 1,
    IMPORTANT = 2,
    URGENT = 3,
    INFO_ALERT = 4,
    };

  /**
   * ADSB aircraft types
   * @see GDL 90 Data Interface Specification table 11
   */
  enum class AircraftType: uint8_t
    {
    UNKNOWN                   = 0,
    LIGHT                     = 1,
    SMALL                     = 2,
    LARGE                     = 3,
    HIGH_VORTEX_LARGE         = 4,
    HEAVY                     = 5,
    HIGHLY_MANEUVERABLE       = 6,
    ROTORCRAFT                = 7,
    EIGHT                     = 8,  // Unassigned.
    GLIDER                    = 9,  // Sailplane.
    LIGHTER_THAN_AIR          = 10,
    PARACHUTIST               = 11, // Or sky diver.
    ULTRALIGHT                = 12, // Or para glider, hang glider.
    THIRTEEN                  = 13, // Unassigned.
    UAV                       = 14,
    SV                        = 15,
    SIXTEEN                   = 16, // Unassigned.
    EMERGENCY_SURFACE_VEHICLE = 17,
    SERVICE_SURFACE_VEHICLE   = 18,
    POINT_OBSTACLE            = 19,
    CLUSTER_OBSTACLE          = 20,
    LINE_OBSTACLE             = 21,
    FENCE,                          // Number of elements.
    };

  /** Has the geographical location been calculated yet? */
  bool location_available;

  /** Was the direction of the target received from the ADSB or calculated? */
  bool track_received;

  /** Was the speed of the target received from the ADSB or calculated? */
  bool speed_received;

  /** Has the absolute altitude of the target been calculated yet? */
  bool altitude_available;

  /** Was the turn rate of the target received from the ADSB or calculated? */
  bool turn_rate_received;

  /** Was the climb_rate of the target received from the ADSB or calculated? */
  bool climb_rate_received;

  /** Has the averaged climb rate of the target been calculated yet? */
  bool climb_rate_avg30s_available;

  /** Is this object valid, or has it expired already? */
  Validity valid;

  /** Location of the ADSB target */
  GeoPoint location;

  /** Distance from our aircraft to the ADSB target */
  RoughDistance distance;

  /** Bearing from our aircraft to the ADSB target */
  RoughAngle track;

  /** Speed of the ADSB target */
  RoughSpeed speed;

  /** Altitude of the ADSB target */
  RoughAltitude altitude;

  /** Altitude based distance of the ADSB target */
  RoughAltitude relative_altitude;

  /** Climbrate of the ADSB target */
  double climb_rate;

  /** Latitude-based distance of the ADSB target */
  double relative_north;

  /** Longitude-based distance of the ADSB target */
  double relative_east;

  /** ICAO id of the ADSB target */
  unsigned int id;

  /** Name of the ADSB target */
  StaticString<8> name;

  AlarmType alarm_level;

  /** Type of the aircraft */
  AircraftType type;

  /** Average climb rate over 30s */
  double climb_rate_avg30s;

  bool IsDefined() const
    {
    return valid;
    }

  bool HasAlarm() const
    {
    return alarm_level != AlarmType::NONE;
    }

  /**
   * Does the target have a name?
   * @return True if a name has been assigned to the target
   */
  bool HasName() const
    {
    return !name.empty();
    }

  void Clear()
    {
    valid.Clear();
    name.clear();
    }

  /**
   * Give the bearing from own ship to target.
   * @return Target bearing.
   */
  Angle Bearing() const
    {
    return Angle::FromXY(relative_north, relative_east);
    }

  /**
   * Is the target powered?
   * @return If powered then true.
   */
  bool IsPowered() const
    {
    return type != AircraftType::GLIDER && type != AircraftType::PARACHUTIST;
    }

  /**
   * Is this a powered but stationary target.
   * @return If powered and stationary then true.
   */
  bool IsPassive() const
    {
    return IsPowered() || speed < 4;
    }

  /**
   * Clear this object if its data has expired.
   * @return true if the object is still valid
   */
  bool Refresh(TimeStamp Time) noexcept
    {
    valid.Expire(Time, std::chrono::seconds(2));
    return valid;
    }

  static const TCHAR* GetTypeString(AircraftType type);

  void Update(const AdsbTraffic &other);
  };

static_assert(std::is_trivial<AdsbTraffic>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_TRAFFIC_HPP
