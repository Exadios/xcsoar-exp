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

#include "Surveillance/AircraftType.hpp"

#include <map>
#include <array>
#include <string>
#include <cassert>

/**
 * Text associated with each aircraft type.
 */
static std::map<AircraftType::AircraftType_t, std::string> Types =
  {
  {AircraftType::AircraftType_t::UNKNOWN,                   "Unknown"},
  {AircraftType::AircraftType_t::GLIDER,                    "Glider"},
  {AircraftType::AircraftType_t::TOW_PLANE,                 "Tow Plane"},
  {AircraftType::AircraftType_t::HELICOPTER,                "Helicopter"},
  {AircraftType::AircraftType_t::PARACHUTE,                 "Parachute"},
  {AircraftType::AircraftType_t::DROP_PLANE,                "Drop Plane"},
  {AircraftType::AircraftType_t::HANG_GLIDER,               "Hang Glider"},
  {AircraftType::AircraftType_t::PARA_GLIDER,               "Para Glider"},
  {AircraftType::AircraftType_t::POWERED_AIRCRAFT,          "Powered Aircraft"},
  {AircraftType::AircraftType_t::JET_AIRCRAFT,              "Jet Aircraft"},
  {AircraftType::AircraftType_t::FLYING_SAUCER,             "UFO"},
  {AircraftType::AircraftType_t::BALLOON,                   "Balloon"},
  {AircraftType::AircraftType_t::AIRSHIP,                   "Airship"},
  {AircraftType::AircraftType_t::UAV,                       "UAV"},
  {AircraftType::AircraftType_t::STATIC_OBJECT,             "Static Object"},
  {AircraftType::AircraftType_t::EMERGENCY_SURFACE_VEHICLE, "Ground Emergency Vehicle"},
  {AircraftType::AircraftType_t::SURFACE_VEHICLE,           "Ground Vehicle"},
  };

//------------------------------------------------------------------------------
AircraftType::AircraftType(const AircraftType& rhs)
  {
  this->type = rhs.type;
  }

//------------------------------------------------------------------------------
AircraftType::~AircraftType()
  {
  }

//------------------------------------------------------------------------------
AircraftType&
AircraftType::operator=(const AircraftType& rhs) noexcept
  {
  this->type = rhs.type;
  return *this;
  }

//------------------------------------------------------------------------------
bool
AircraftType::Type(unsigned int type) noexcept
  {
  assert(type >= (unsigned int)AircraftType_t::UNKNOWN);
  assert(type <  (unsigned int)AircraftType_t::__FENCE__);

  if (type >= (unsigned int)AircraftType_t::UNKNOWN &&
      type <  (unsigned int)AircraftType_t::__FENCE__)
    {
    this->type = (AircraftType_t)type;
    return true;
    }
  else
    return false;
  }

//------------------------------------------------------------------------------
AircraftType::AircraftType_t
AircraftType::Type() const noexcept
  {
  return this->type;
  }

//-----------------------------------------------------------------------------
const TCHAR*
AircraftType::TypeName() const noexcept
  {
  return Types.at(this->type).c_str();
  }

//------------------------------------------------------------------------------
bool
AircraftType::IsPowered() const noexcept
  {
  if ((this->type == AircraftType_t::GLIDER)      ||
      (this->type == AircraftType_t::PARACHUTE)   ||
      (this->type == AircraftType_t::HANG_GLIDER) ||
      (this->type == AircraftType_t::PARA_GLIDER) ||
      (this->type == AircraftType_t::BALLOON))
    return false;
  return true;
  }

//------------------------------------------------------------------------------
const TCHAR*
AircraftType::TypeName(AircraftType_t type) noexcept
  {
  if (type >= AircraftType_t::UNKNOWN &&
      type <  AircraftType_t::__FENCE__)
    return Types.at(type).c_str();
  else
    return Types.at(AircraftType_t::UNKNOWN).c_str();
  }

//------------------------------------------------------------------------------
const TCHAR*
AircraftType::TypeName(const AircraftType& rhs) noexcept
  {
  return Types.at(rhs.type).c_str();
  }
