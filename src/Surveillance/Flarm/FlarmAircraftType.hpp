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

#include "Surveillance/AircraftType.hpp"

#include <tchar.h>

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

/**
 * A class to handle FLARM aircraft types.
 */

class FlarmAircraftType final : public AircraftType
  {
public:
  enum class AircraftType_t : uint8_t
    {
    UNKNOWN          = 0,   // unknown
    GLIDER           = 1,   // glider / motor-glider
    TOW_PLANE        = 2,   // tow / tug plane
    HELICOPTER       = 3,   // helicopter / rotorcraft
    PARACHUTE        = 4,   // parachute
    DROP_PLANE       = 5,   // drop plane for parachutes
    HANG_GLIDER      = 6,   // hang-glider (hard)
    PARA_GLIDER      = 7,   // para-glider (soft)
    POWERED_AIRCRAFT = 8,   // powered aircraft
    JET_AIRCRAFT     = 9,   // jet aircraft
    FLYING_SAUCER    = 10,  // flying saucer (UFO)
    BALLOON          = 11,  // balloon
    AIRSHIP          = 12,  // airship
    UAV              = 13,  // unmanned aerial vehicle
    UNDEFINED        = 14,  // Who knows?
    STATIC_OBJECT    = 15,  // static object
    };

  /**
   * Default ctor.
   */
  FlarmAircraftType();

  /**
   * Dtor.
   */
  virtual ~FlarmAircraftType() override;

  /**
   * The purpose of this function is to take the aircraft type, presumably
   * from the device driver, and set the internal state.
   * @param type The aircraft type specified by the driver. Presumably this
   *             is an unsigned int from the driver or can be cast to that.
   * @return If the type parameter is within the range specified by
   *         AircraftType_t then true and the state id set. Otherwise false
   *         and the state is not set.
   */
  bool Type(unsigned int type) noexcept override;

  /**
   * The purpose of this function is to take the aircraft type, presumably
   * from the device driver, and set the internal state.
   * @param type The aircraft type specified by the driver. 
   */
  void Type(AircraftType_t type) noexcept;

  /**
   * The aircraft type index.
   * @return The internal state as cast.
   */
  AircraftType::AircraftType_t Type() const noexcept override;
  };

/**
 * \}
 */
