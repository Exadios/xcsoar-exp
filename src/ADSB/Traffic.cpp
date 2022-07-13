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

#include "ADSB/Traffic.hpp"

static constexpr const TCHAR* acTypes[22] =
  {
  _T("Unknown"),
  _T("Light"),
  _T("Small"),
  _T("Large"),
  _T("High Vortex"),
  _T("Heavy"),
  _T("Highly Maneuverable"),
  _T("Rotocraft"),
  _T("Unassigned"),
  _T("Glider"),
  _T("Balloon"),
  _T("Parachutist"),
  _T("Ultralight"),
  _T("Unassigned"),
  _T("UAV"),
  _T("SV"),
  _T("Unassigned"),
  _T("Emergency Surface Vehicle"),
  _T("Service Surface Vehicle"),
  _T("Point Obstacle"),
  _T("Cluster Obstacle"),
  _T("Line Obstacle"),
  };

//------------------------------------------------------------------------------
const TCHAR *
AdsbTraffic::GetTypeString(AircraftType type)
  {
  if (type < AircraftType::FENCE)
    {
    unsigned index = (unsigned)type;
    return acTypes[index];
    }

  return NULL;
  }

//------------------------------------------------------------------------------
void
AdsbTraffic::Update(const AdsbTraffic &other)
  {
  this->alarm_level = other.alarm_level;
  this->relative_north = other.relative_north;
  this->relative_east = other.relative_east;
  this->relative_altitude = other.relative_altitude;
  this->track = other.track;
  this->track_received = other.track_received;
  this->turn_rate_received = other.turn_rate_received;
  this->speed = other.speed;
  this->speed_received = other.speed_received;
  this->climb_rate = other.climb_rate;
  this->climb_rate_received = other.climb_rate_received;
  this->type = other.type;
  }
