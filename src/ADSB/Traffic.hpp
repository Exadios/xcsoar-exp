/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#include "IcaoId.hpp"
#include "NMEA/Validity.hpp"
#include "Geo/GeoPoint.hpp"
#include "Util/StaticString.hxx"
#include "Rough/RoughAltitude.hpp"
#include "Rough/RoughDistance.hpp"
#include "Rough/RoughSpeed.hpp"
#include "Rough/RoughAngle.hpp"

#include <type_traits>

#include <tchar.h>

struct AdsbTraffic
  {

public:
  /** Has the geographical location been calculated yet? */
  bool location_available;

  /** Was the direction of the target received from the ADSB or calculated? */
  bool track_received;

  /** Was the speed of the target received from the flarm or calculated? */
  bool speed_received;

  /** Was the climb_rate of the target received from the flarm or calculated? */
  bool climb_rate_received;

  /** Has the absolute altitude of the target been calculated yet? */
  bool altitude_available;

  /** Has the averaged climb rate of the target been calculated yet? */
  bool climb_rate_avg30s_available;

  /** Average climb rate over 30s */
  double climb_rate_avg30s;

  /** Was the direction of the target received from the ADSB or calculated? */
  bool track_valid;

  /** Was the speed of the target received from the ADSB or calculated? */
  bool speed_valid;

  /** Has the absolute altitude of the target been calculated yet? */
  bool altitude_valid;

  /** Is this object valid, or has it expired already? */
  Validity valid;

  /** Location of the ADSB target */
  GeoPoint location;

  /** Distance from our plane to the ADSB target */
  RoughDistance distance;

  /** TrackBearing of the ADSB target */
  RoughAngle track;

  /** Speed of the ADSB target */
  RoughSpeed speed;

  /** Altitude of the ADSB target */
  RoughAltitude altitude;

  /** Altitude-based distance of the ADSB target */
  RoughAltitude relative_altitude;

  /** Climbrate of the ADSB target */
  double climb_rate;

  /** Latitude-based distance of the ADSB target */
  double relative_north;

  /** Longitude-based distance of the ADSB target */
  double relative_east;

  /** ICAO id of the ADSB target */
  IcaoId id;

  /** (if exists) Name of the ADSB target */
  StaticString<10> name;

  /**
   * Is this aircraft valid?
   * @return If so, true.
   */
  bool IsDefined() const
    {
    return this->valid;
    }

  /**
   * Do we know the flight number?
   * @return If so, true
   */
  bool HasName() const
    {
    return !this->name.empty();
    }

  /**
   * Make this aircraft invalid and unknown.
   */
  void Clear() 
    {
    this->valid.Clear();
    this->name.clear();
    }

  /**
   * The bearing from us to them.
   * @return From us to them.
   */
  Angle Bearing() const
    {
    return Angle::FromXY(this->relative_north, this->relative_east);
    }

  /**
   * Is a powered aircraft.
   * @return Always true because the assumption is that ADSB aircraft are
   *         powered.
   */
  bool IsPowered() const
    {
    return true;
    }

  /**
   * Is this aircraft on the move?
   * @return If so, false.
   */
  bool IsPassive() const
    {
    return this->IsPowered() || this->speed < 4;
    }

  /**
   * Clear this object if its data has expired.
   *
   * @param Time Current time.
   * @return true if the object is still valid
   */
  bool Refresh(double Time) 
    {
    this->valid.Expire(Time, 2);
    return this->valid;
    }

  /**
   * Update the dynamic variables of this aircraft.
   * @param other The source update.
   */
  void Update(const AdsbTraffic &other);
  };

static_assert(std::is_trivial<AdsbTraffic>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_TRAFFIC_HPP
