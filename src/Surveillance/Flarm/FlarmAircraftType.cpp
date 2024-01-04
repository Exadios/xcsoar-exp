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

#include "Surveillance/Flarm/FlarmAircraftType.hpp"
#include "Surveillance/RemoteTarget.hpp"

#include <map>
#include <array>

/**
 * A map to convert FLARM aircraft types into generic representations suitable
 * for \ref AircraftType::AircraftType_t.
 */
static std::array<AircraftType::AircraftType_t, 16> flarm_map =
  {
  // FlarmAircraftType::AircraftType_t::UNKNOWN
  AircraftType::AircraftType_t::UNKNOWN,

  // FlarmAircraftType::AircraftType_t::GLIDER
  AircraftType::AircraftType_t::GLIDER,

  // FlarmAircraftType::AircraftType_t::TOW_PLANE
  AircraftType::AircraftType_t::TOW_PLANE,

  // FlarmAircraftType::AircraftType_t::HELICOPTER
  AircraftType::AircraftType_t::HELICOPTER,

  // FlarmAircraftType::AircraftType_t::PARACHUTE
  AircraftType::AircraftType_t::PARACHUTE,

  // FlarmAircraftType::AircraftType_t::DROP_PLANE
  AircraftType::AircraftType_t::DROP_PLANE,

  // FlarmAircraftType::AircraftType_t::HANG_GLIDER
  AircraftType::AircraftType_t::HANG_GLIDER,

  // FlarmAircraftType::AircraftType_t::PARA_GLIDER
  AircraftType::AircraftType_t::PARA_GLIDER,

  //  FlarmAircraftType::AircraftType_t::POWERED_AIRCRAFT
  AircraftType::AircraftType_t::POWERED_AIRCRAFT,

  // FlarmAircraftType::AircraftType_t::JET_AIRCRAFT
  AircraftType::AircraftType_t::JET_AIRCRAFT,

  // FlarmAircraftType::AircraftType_t::FLYING_SAUCER
  AircraftType::AircraftType_t::FLYING_SAUCER,

  // FlarmAircraftType::AircraftType_t::BALLOON
  AircraftType::AircraftType_t::BALLOON,

  // FlarmAircraftType::AircraftType_t::AIRSHIP
  AircraftType::AircraftType_t::AIRSHIP,

  // FlarmAircraftType::AircraftType_t::UAV
  AircraftType::AircraftType_t::UAV,

  // FlarmAircraftType::AircraftType_t::UNDEFINED
  AircraftType::AircraftType_t::UNKNOWN,

  // FlarmAircraftType::AircraftType_t::STATIC_OBJECT
  AircraftType::AircraftType_t::STATIC_OBJECT,
  };

//------------------------------------------------------------------------------
FlarmAircraftType::FlarmAircraftType()
  {
  this->type = AircraftType::AircraftType_t::UNKNOWN;
  }

//------------------------------------------------------------------------------
FlarmAircraftType::~FlarmAircraftType()
  {
  }

//------------------------------------------------------------------------------
bool
FlarmAircraftType::Type(unsigned int type) noexcept
  {
  try
    {
    this->type = flarm_map.at(type);
    }
  catch (std::out_of_range const&)
    {
    return false;
    }
  return true;
  }

//------------------------------------------------------------------------------
void
FlarmAircraftType::Type(AircraftType_t type) noexcept
  {
  this->type = flarm_map.at((unsigned int)type);
  }

//------------------------------------------------------------------------------
AircraftType::AircraftType_t
FlarmAircraftType::Type() const noexcept
  {
  return this->type;
  }
