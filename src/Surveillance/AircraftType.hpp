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

#include <memory>
#include <tchar.h>

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

/**
 * A class to hold aircraft types for each sensor class.
 */
class AircraftType
  {
public:
  /**
   * FLARM aircraft types:
   * @see http://www.flarm.com/support/manual/FLARM_DataportManual_v4.06E.pdf
   * Page 8.
   * ADSB aircraft types:
   * @see GDL 90 Data Interface Specification table 11
   *
   * This enum is a merge of the Flarm and Adsb aircraft types. It is probably
   * not worth implementing some sort of enum inheritance scheme.
   */
  enum class AircraftType_t: uint8_t
    {
    UNKNOWN                   = 0,   // unknown
    GLIDER                    = 1,   // glider / motor-glider
    TOW_PLANE                 = 2,   // tow / tug plane
    HELICOPTER                = 3,   // helicopter / rotorcraft
    PARACHUTE                 = 4,   // parachute
    DROP_PLANE                = 5,   // drop plane for parachutes
    HANG_GLIDER               = 6,   // hang-glider (hard)
    PARA_GLIDER               = 7,   // para-glider (soft)
    POWERED_AIRCRAFT          = 8,   // powered aircraft
    JET_AIRCRAFT              = 9,   // jet aircraft
    FLYING_SAUCER             = 10,  // flying saucer (UFO)
    BALLOON                   = 11,  // balloon
    AIRSHIP                   = 12,  // airship
    UAV                       = 13,  // unmanned aerial vehicle
    STATIC_OBJECT             = 15,  // static object
    EMERGENCY_SURFACE_VEHICLE = 16,  // vehicle on the ground
    SURFACE_VEHICLE           = 17,  // vehicle on the ground
    __FENCE__                 = 18,
    };

  /**
   * Default ctor.
   */
  AircraftType() = default;

  /**
   * Copy ctor.
   * @param rhs The reference object.
   */
  AircraftType(const AircraftType& rhs);

  /**
   * Dtor.
   */
  virtual ~AircraftType();

  /**
   * Copy operator.
   * @param rhs The reference object.
   * @return This object.
   */
  AircraftType& operator=(const AircraftType& rhs) noexcept;

  /**
   * The purpose of this function is to take the aircraft type, presumably
   * from the device driver, and set the internal state.
   * @param type The aircraft type specified by the driver. Presumably this
   *             is an unsigned int from the driver or can be cast to that.
   * @return If the type parameter is acceptable to this class then the
   *         state is set and true is returned. Otherwise there is no
   *         change to the state and false is returned.
   */
  virtual bool Type(unsigned int type) noexcept;

  /**
   * The aircraft type index.
   * @return The internal state.
   */
  virtual AircraftType_t Type() const noexcept;

  /**
   * The aircraft type as a string.
   * @return The aircraft type description.
   */
  virtual const TCHAR* TypeName() const noexcept;

  /**
   * The aircraft is definitely a powered type.
   * @return If a powered type then true.
   */
  virtual bool IsPowered() const noexcept;

  /**
   * Give the type name of a supplied type.
   * @param type The supplied type
   * @return The aircraft type description.
   */
  static const TCHAR* TypeName(AircraftType_t type) noexcept;

  /**
   * Give the type name of a supplied type.
   * @param type The supplied type
   * @return The aircraft type description.
   */
  static const TCHAR* TypeName(const AircraftType& rhs) noexcept;

protected:
  AircraftType_t type;
  };

/**
 * \}
 */
