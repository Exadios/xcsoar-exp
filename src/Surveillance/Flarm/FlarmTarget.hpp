/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2023 The XCSoar Project
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

#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/Flarm/FlarmAircraftType.hpp"

#include <type_traits>
#include <memory>

struct FlarmTarget : public RemoteTarget
  {
  /** FLARM aircraft type. */
  FlarmAircraftType type;
   
  /** Is the target in stealth mode? */
  bool stealth;

  /** Was the turn rate of the target received from the flarm or calculated? */
  bool turn_rate_received;

  /** Turnrate of the FLARM target */
  double turn_rate;

  /**
   * Is this target in stealth mode?
   * @return If in stealth mode then true.
   */
  bool Stealth() const override
    {
    return this->stealth;
    }

  /**
   * Set the stealth status.
   * @param stealth Either true or false.
   */
  void Stealth(bool stealth) override
    {
    this->stealth = stealth;
    }

  /**
   * Has the turn rate of the target received from the flarm or calculated?
   * @return If received then true.
   */
  bool TurnRateReceived() const override
    {
    return this->turn_rate_received;
    }

  /**
   * Set the turn rate received status.
   * @param turn_rate_received Either true or false.
   */
  void TurnRateReceived(bool turn_rate_received) override
    {
    this->turn_rate_received = turn_rate_received;
    }

  /**
   * Current turn rate.
   * @return The current turn rate in <units?>
   */
  double TurnRate() const override
    {
    return this->turn_rate;
    }

  /**
   * Set the turn rate.
   * @param turn_rate The current turn rate in <units?>
   */
  void TurnRate(double turn_rate) override
    {
    this->turn_rate = turn_rate;
    }

    /**
   * Set aircraft type.
   * @param type The FLARM aircraft type as reported by the driver.
   * @return If the value of type is legal then true.
   */
  bool Type(unsigned int type) override
    {
    return this->type.Type(type);
    }

  /**
   * Is definitely powered type.
   * @return If the target is definitely powered type then true.
   */
  bool IsPowered() const override
    {
    return type.IsPowered();
    }

  /**
   * Do an item by item update for this derived class.
   * @param other The source target.
   */
  void Update(const FlarmTarget& other);

  /**
   * The data from this target is from a collision avoidance system.
   * @return Always true.
   */
  bool CollisionAvoidance() const override
    {
    return true;
    }

  /**
   * The data from this target is not from a traffic awareness system.
   * @return Always false.
   */
  bool TrafficAwareness() const override
    {
    return false;
    }

#ifndef NDEBUG
  virtual int DebugType() const override
    {
    return 1;
    }
#endif

  };

/**
 * \}
 */
typedef std::shared_ptr<FlarmTarget> FlarmPtr;
