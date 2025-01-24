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

#include "Surveillance/Adsb/AdsbAircraftType.hpp"
#include "Surveillance/RemoteTarget.hpp"

#include <map>
#include <array>

/**
 * A map to convert ADS-B aircraft types into generic representations suitable
 * for \ref AircraftType::AircraftType_t.
 */
static std::array<AircraftType::AircraftType_t, 22> adsb_map =
  {
  // AdsbAircraftType::AircraftType_t::UNKNOWN
  AircraftType::AircraftType_t::UNKNOWN,

  // AdsbAircraftType::AircraftType_t::LIGHT
  AircraftType::AircraftType_t::POWERED_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::SMALL
  AircraftType::AircraftType_t::POWERED_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::LARGE
  AircraftType::AircraftType_t::POWERED_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::HIGH_VORTEX_LARGE
  AircraftType::AircraftType_t::JET_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::HEAVY
  AircraftType::AircraftType_t::JET_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::HIGHLY_MANEUVERABLE
  AircraftType::AircraftType_t::JET_AIRCRAFT,

  // AdsbAircraftType::AircraftType_t::ROTORCRAFT
  AircraftType::AircraftType_t::HELICOPTER,

  // AdsbAircraftType::AircraftType_t::EIGHT
  AircraftType::AircraftType_t::UNKNOWN,

  // AdsbAircraftType::AircraftType_t::GLIDER
  AircraftType::AircraftType_t::GLIDER,

  // AdsbAircraftType::AircraftType_t::LIGHTER_THAN_AIR
  AircraftType::AircraftType_t::BALLOON,

  // AdsbAircraftType::AircraftType_t::PARACHUTIST
  AircraftType::AircraftType_t::PARACHUTE,

  // AdsbAircraftType::AircraftType_t::ULTRALIGHT
  AircraftType::AircraftType_t::POWERED_AIRCRAFT,

  //  AdsbAircraftType::AircraftType_t::THIRTEEN
  AircraftType::AircraftType_t::UNKNOWN,

  // AdsbAircraftType::AircraftType_t::UAV
  AircraftType::AircraftType_t::UAV,

  // AdsbAircraftType::AircraftType_t::SV
  AircraftType::AircraftType_t::FLYING_SAUCER,

  // AdsbAircraftType::AircraftType_t::SIXTEEN
  AircraftType::AircraftType_t::UNKNOWN,

  // AdsbAircraftType::AircraftType_t::EMERGENCY_SURFACE_VEHICLE
  AircraftType::AircraftType_t::EMERGENCY_SURFACE_VEHICLE,

  //  AdsbAircraftType::AircraftType_t::SERVICE_SURFACE_VEHICLE
  AircraftType::AircraftType_t::SURFACE_VEHICLE,

  // AdsbAircraftType::AircraftType_t::POINT_OBSTACLE
  AircraftType::AircraftType_t::STATIC_OBJECT,

  // AdsbAircraftType::AircraftType_t::CLUSTER_OBSTACLE
  AircraftType::AircraftType_t::STATIC_OBJECT,

  // AdsbAircraftType::AircraftType_t::LINE_OBSTACLE
  AircraftType::AircraftType_t::STATIC_OBJECT,
  };

//------------------------------------------------------------------------------
AdsbAircraftType::AdsbAircraftType()
  {
  this->type = AircraftType::AircraftType_t::UNKNOWN;
  }

//------------------------------------------------------------------------------
AdsbAircraftType::~AdsbAircraftType()
  {
  }

//------------------------------------------------------------------------------
bool
AdsbAircraftType::Type(unsigned int type) noexcept
  {
  try
    {
    this->type = adsb_map.at(type);
    }
  catch (std::out_of_range const&)
    {
    return false;
    }
  return true;
  }

//------------------------------------------------------------------------------
void
AdsbAircraftType::Type(AircraftType_t type) noexcept
  {
  this->type = adsb_map.at((unsigned int)type);
  }

//------------------------------------------------------------------------------
AircraftType::AircraftType_t
AdsbAircraftType::Type() const noexcept
  {
  return this->type;
  }

