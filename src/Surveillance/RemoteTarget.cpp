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

#include "Surveillance/RemoteTarget.hpp"
#include <math.h>

#if 0
/**
 * Text representations of the \ref AircraftType enum. Plainly the number
 * entries in this array must equal the number of enum types.
 */
static constexpr const TCHAR* acTypes[18] =
  {
  _T("Unknown"),
  _T("Glider"),
  _T("TowPlane"),
  _T("Helicopter"),
  _T("Parachute"),
  _T("DropPlane"),
  _T("HangGlider"),
  _T("ParaGlider"),
  _T("PoweredAircraft"),
  _T("JetAircraft"),
  _T("FlyingSaucer"),
  _T("Balloon"),
  _T("Airship"),
  _T("UAV"),
  _T("Unknown"),
  _T("StaticObject")
  _T("Emergency Surface Vehicle"),
  _T("Service Surface Vehicle"),
  };
#endif

//------------------------------------------------------------------------------
void
RemoteTarget::Update(const RemoteTarget &other)
  {
  this->alarm_level         = other.alarm_level;
  this->track               = other.track;
  this->track_received      = other.track_received;
  this->speed               = other.speed;
  this->speed_received      = other.speed_received;
  this->climb_rate          = other.climb_rate;
  this->climb_rate_received = other.climb_rate_received;
  }

//------------------------------------------------------------------------------
unsigned int
RemoteTarget::RankScore() const
  {
  /* The rational is to compute a score which is mainly weighted by
   * the read status but influenced by distance (from our aircraft).
   */
  double dist_score;
  if ((double)this->distance > 1.0)
    dist_score  = 10000 / (this->distance * this->distance);
  else
    dist_score = 10000;
  double alarm_score = 5000 * (double)this->alarm_level *
                              (double)this->alarm_level;
  return (unsigned int)(dist_score + alarm_score);
  }

//------------------------------------------------------------------------------
RemoteTarget::RemoteTarget()
  : location_available(false),
    distance(-1)
  {
  }

//------------------------------------------------------------------------------
RemoteTarget::~RemoteTarget()
  {
  }
