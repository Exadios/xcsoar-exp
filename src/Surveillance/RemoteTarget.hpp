/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
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

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "TargetId.hpp"
#include "AircraftType.hpp"
#include "Geo/GeoPoint.hpp"
#include "NMEA/Validity.hpp"
#include "util/StaticString.hxx"
#include "Rough/RoughAltitude.hpp"
#include "Rough/RoughDistance.hpp"
#include "Rough/RoughSpeed.hpp"
#include "Rough/RoughAngle.hpp"

#include <type_traits>
#include <tchar.h>
#include <memory>

#ifndef NDEBUG
#include "LogFile.hpp"
#endif

/**
 * The purpose of this struct is to define the total domain of remote targets.
 * Methods on the domain may be implemented in this struct or derived
 * structs, or both, as appropriate. Of course it may become necessary to
 * expand this domain to suit a currently unsupported device.
 */
struct RemoteTarget
  {
public:
  enum class AlarmType: uint8_t
    {
    NONE = 0,
    LOW = 1,
    IMPORTANT = 2,
    URGENT = 3,
    INFO_ALERT = 4,
    };

  /** Type of the target. */
//  AircraftType type;

  /** Has the geographical location been calculated yet? */
  bool location_available;

  /** Was the direction of the target received from the target or calculated? */
  bool track_received;

  /** Was the speed of the target received from the target or calculated? */
  bool speed_received;

  /** Has the absolute altitude of the target been calculated yet? */
  bool altitude_available;

  /** Was the climb_rate of the target received from the target or calculated? */
  bool climb_rate_received;

  /** Has the averaged climb rate of the target been calculated yet? */
  bool climb_rate_avg30s_available;

  /** Is this object valid, or has it expired already? */
  Validity valid;

  /** Location of the target */
  GeoPoint location;

  /** Distance from our plane to the target */
  RoughDistance distance;

  /** TrackBearing of the target */
  RoughAngle track;

  /** Speed of the target */
  RoughSpeed speed;

  /** Altitude of the target */
  RoughAltitude altitude;

  /** Difference between the target altitude and our own altitude. */
  RoughAltitude relative_altitude;

  /** Turnrate of the target */
  double turn_rate;

  /** Climbrate of the target */
  double climb_rate;

  /** Latitude-based distance of the target */
  double relative_north;

  /** Longitude-based distance of the target */
  double relative_east;

  /** Hex id of the  target */
  TargetId id;

  /** (if exists) Name of the target */
  StaticString<10> name;

  AlarmType alarm_level;

  /** Average climb rate over 30s */
  double climb_rate_avg30s;

  bool IsDefined() const
    {
    return this->valid;
    }

  bool HasAlarm() const
    {
    return this->alarm_level != AlarmType::NONE;
    }

  /**
   * Does the target have a name?
   * @return True if a name has been assigned to the target
   */
  bool HasName() const
    {
    return !this->name.empty();
    }

  void Clear()
    {
    valid.Clear();
    name.clear();
    }

  Angle Bearing() const
    {
    return Angle::FromXY(relative_north, relative_east);
    }

  /**
   * Set aircraft type.
   * @param type The aircraft type as given by the driver.
   * @return If the value of type is legal then true.
   */
  virtual bool Type([[maybe_unused]]unsigned int type)
    {
    return false;
    }

  /**
   * Is definitely powered.
   * @return If the target is definitely powered type then true.
   */
  virtual bool IsPowered() const
    {
    return false;
    }

  /**
   * Is stationary.
   * @return If the target is powered and stationary.
   */
  bool IsPassive() const
    {
    return this->IsPowered() || speed < 4;
    }

  /**
   * Clear this object if its data has expired.
   * @param time Current seconds since midnight * 64.
   * @return true if the object is still valid
   */
  bool Refresh(TimeStamp time) noexcept
    {
    this->valid.Expire(time, std::chrono::seconds(2));
    return this->valid;
    }

  /**
   * Do an item for item copy to this object.
   * @param other The source target.
   */
  void Update(const RemoteTarget& other);

  /**
   * Ctor
   */
  RemoteTarget();

  /**
   * Dtor. Destruction may occur at any moment via a delete. Very important
   * that the dtor is virtual!
   */
  virtual ~RemoteTarget();

  /**
   * Is this target in stealth mode?
   * @return Always false.
   */
  virtual bool Stealth() const
    {
    return false;
    }

  /**
   * Set the stealth status. This function is a no-op in this base struct.
   * @param stealth Who cares.
   */
  virtual void Stealth([[maybe_unused]] bool stealth) 
    {
    }

  /**
   * Has the turn rate of the target received from the flarm or calculated?
   * @return Always false.
   */
  virtual bool TurnRateReceived() const
    {
    return false;
    }

  /**
   * Set the turn rate received status. This function is a no-op in this
   * base struct.
   * @param turn_rate_received Who cares.
   */
  virtual void TurnRateReceived([[maybe_unused]] bool turn_rate_received) 
    {
    }

  /**
   * Current turn rate.
   * @return Always 0.0
   */
  virtual double TurnRate() const
    {
    return 0.0;
    }

  /**
   * Set the turn rate. This function is a no-op in this base struct.
   * @param turn_rate Who cares.
   */
  virtual void TurnRate([[maybe_unused]] double turn_rate) 
    {
    }

  /**
   * Compute a metric that can be used to sort the list for pruning etc.
   * @return A metric that signifies the relative importance of this
   * particular target. Higher score means more important.
   */
  unsigned int RankScore() const;

  /**
   * Is this target data derived from a collision avoidance system?
   * @return The data is from a collision avoidance system (e.g. Flarm,
   *         TCAS, or some other ACAS system).
   */
  virtual bool CollisionAvoidance() const
    {
    return false;
    }
  
  /**
   * Is this target data derived from a traffic awareness system?
   * @return The data is from a traffic awareness system (e.g. ADS-B).
   */
  virtual bool TrafficAwareness() const
    {
    return false;
    }

#ifndef NDEBUG
  virtual int DebugType() const
    {
    return 0;
    }
#endif

};

/**
 * The shared_ptr to use.
 */
typedef std::shared_ptr<RemoteTarget> TargetPtr;

/** \} */

//static_assert(std::is_trivial<RemoteTarget>::value, "type is not trivial");
