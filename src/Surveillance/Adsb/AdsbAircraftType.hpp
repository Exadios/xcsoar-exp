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

#include "Surveillance/AircraftType.hpp"

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

/**
 * A class to handle ADS-B aircraft types.
 */
class AdsbAircraftType final : public AircraftType
  {
public:
  /**
   * ADSB aircraft types
   * @see GDL 90 Data Interface Specification table 11
   */
  enum class AircraftType_t : uint8_t
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
    };

  /**
   * Default ctor.
   */
  AdsbAircraftType();

  /**
   * Dtor.
   */
  virtual ~AdsbAircraftType() override;

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

private:

  };

  /**
   * \}
   */

