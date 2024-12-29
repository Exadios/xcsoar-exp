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

/**
 * \cond NEVER
 */

#include "FLARM/Traffic.hpp"
#include "ADSB/Traffic.hpp"
#include <math.h>

static constexpr const TCHAR* acTypes[16] = {
  _T("Unknown"), _T("Glider"), _T("TowPlane"),
    _T("Helicopter"), _T("Parachute"), _T("DropPlane"), _T("HangGlider"),
    _T("ParaGlider"), _T("PoweredAircraft"), _T("JetAircraft"),
    _T("FlyingSaucer"), _T("Balloon"), _T("Airship"), _T("UAV"),
    _T("Unknown"), _T("StaticObject") };

const TCHAR *
FlarmTraffic::GetTypeString(AircraftType type)
{
  unsigned index = (unsigned)type;
  if (index < 16)
    return acTypes[index];

  return NULL;
}

void
FlarmTraffic::Update(const FlarmTraffic &other)
{
  alarm_level = other.alarm_level;
  relative_north = other.relative_north;
  relative_east = other.relative_east;
  relative_altitude = other.relative_altitude;
  track = other.track;
  track_received = other.track_received;
  turn_rate = other.turn_rate;
  turn_rate_received = other.turn_rate_received;
  speed = other.speed;
  speed_received = other.speed_received;
  climb_rate = other.climb_rate;
  climb_rate_received = other.climb_rate_received;
  stealth = other.stealth;
  type = other.type;
}

//-----------------------------------------------------------------------------
void
AdsbConvert(const AdsbTraffic &target, FlarmTraffic &eqv)
  {
  eqv.valid               = target.valid;
  eqv.alarm_level         = FlarmTraffic::AlarmType::NONE;
  eqv.relative_north      = target.relative_north;
  eqv.relative_east       = target.relative_east;
  eqv.relative_altitude   = target.relative_altitude;
  eqv.track               = target.track;
  eqv.track_received      = target.track_received;
  eqv.turn_rate           = 0.0;
  eqv.turn_rate_received  = false;
  eqv.speed               = target.speed;
  eqv.speed_received      = target.speed_received;
  eqv.climb_rate          = target.climb_rate;
  eqv.climb_rate_received = target.climb_rate_received;
  eqv.stealth             = false;
  eqv.distance            = sqrt(target.relative_east * target.relative_east +
                                 target.relative_north * target.relative_north);

  switch (target.type)
    {
    case AdsbTraffic::AircraftType::UNKNOWN:
    case AdsbTraffic::AircraftType::EIGHT:
    case AdsbTraffic::AircraftType::THIRTEEN:
    case AdsbTraffic::AircraftType::SV:
    case AdsbTraffic::AircraftType::SIXTEEN:
    case AdsbTraffic::AircraftType::POINT_OBSTACLE: 
    case AdsbTraffic::AircraftType::CLUSTER_OBSTACLE: 
    case AdsbTraffic::AircraftType::LINE_OBSTACLE:
    case AdsbTraffic::AircraftType::FENCE:
      {
      eqv.type = FlarmTraffic::AircraftType::UNKNOWN;
      break;
      }
    case AdsbTraffic::AircraftType::LIGHT:
    case AdsbTraffic::AircraftType::SMALL:
      {
      eqv.type = FlarmTraffic::AircraftType::POWERED_AIRCRAFT; 
      break;
      }
    case AdsbTraffic::AircraftType::LARGE:
    case AdsbTraffic::AircraftType::HIGH_VORTEX_LARGE:
    case AdsbTraffic::AircraftType::HEAVY:
    case AdsbTraffic::AircraftType::HIGHLY_MANEUVERABLE:
      {
      eqv.type = FlarmTraffic::AircraftType::JET_AIRCRAFT;
      break;
      }
    case AdsbTraffic::AircraftType::ROTORCRAFT:
      {
      eqv.type = FlarmTraffic::AircraftType::HELICOPTER;
      break;
      }
    case AdsbTraffic::AircraftType::GLIDER:
      {
      eqv.type = FlarmTraffic::AircraftType::GLIDER;
      break;
      }
    case AdsbTraffic::AircraftType::LIGHTER_THAN_AIR:
      {
      eqv.type = FlarmTraffic::AircraftType::BALLOON;
      break;
      }
    case AdsbTraffic::AircraftType::PARACHUTIST:
      {
      eqv.type = FlarmTraffic::AircraftType::PARACHUTE;
      break;
      }
    case AdsbTraffic::AircraftType::ULTRALIGHT:
      {
      eqv.type = FlarmTraffic::AircraftType::POWERED_AIRCRAFT;
      break;
      }
    case AdsbTraffic::AircraftType::UAV:
      {
      eqv.type = FlarmTraffic::AircraftType::FLYING_SAUCER;
      break;
      }
    case AdsbTraffic::AircraftType::EMERGENCY_SURFACE_VEHICLE:
    case AdsbTraffic::AircraftType::SERVICE_SURFACE_VEHICLE:
      {
      eqv.type = FlarmTraffic::AircraftType::UNKNOWN;
      break;
      }
    }
  }

/**
 * \endcond
 */

