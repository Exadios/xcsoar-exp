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

#include "Surveillance/Flarm/FlarmComputer.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/TargetData.hpp"
#include "NMEA/Info.hpp"
#include "Surveillance/Flarm/FlarmCalculations.hpp"
#include "Surveillance/Flarm/FlarmDetails.hpp"
#include "Geo/GeoVector.hpp"

//------------------------------------------------------------------------------
FlarmComputer::FlarmComputer()
  {
  }

//------------------------------------------------------------------------------
FlarmComputer::~FlarmComputer()
  {
  }

//------------------------------------------------------------------------------
void
FlarmComputer::Process(TargetData& flarm,
                       TargetData& last_flarm,
                       const NMEAInfo& basic)
  {

  // Cleanup old calculation instances
  if (basic.time_available)
    flarm_calculations.CleanUp(basic.time);

  // if (target data is available)
  if (!flarm.IsDetected())
    return;

  double north_to_latitude(0);
  double east_to_longitude(0);

  if (basic.location_available)
    {
    // Precalculate relative east and north projection to lat/lon
    // for Location calculations of each target
    constexpr Angle delta_lat = Angle::Degrees(0.01);
    constexpr Angle delta_lon = Angle::Degrees(0.01);

    GeoPoint plat = basic.location;
    plat.latitude += delta_lat;
    GeoPoint plon = basic.location;
    plon.longitude += delta_lon;

    double dlat = basic.location.DistanceS(plat);
    double dlon = basic.location.DistanceS(plon);

    if (fabs(dlat) > 0 && fabs(dlon) > 0)
      {
      north_to_latitude = delta_lat.Degrees() / dlat;
      east_to_longitude = delta_lon.Degrees() / dlon;
      }
    }

  // for each item in traffic
  for (auto& target : flarm.traffic.flarm_list)
    {
    // if we don't know the target's name yet
    if (!target.HasName())
      {

      // lookup the name of this target's id
      const TCHAR* fname = 
                     FlarmDetails::LookupCallsign(target.id);
      if (fname != NULL)
        target.name = fname;
      }

    // Calculate distance
    target.distance = hypot(target.relative_north, target.relative_east);

    // Calculate Location
    target.location_available = basic.location_available;
    if (target.location_available)
      {
      target.location.latitude =
        Angle::Degrees(target.relative_north * north_to_latitude) +
        basic.location.latitude;

      target.location.longitude =
        Angle::Degrees(target.relative_east * east_to_longitude) +
        basic.location.longitude;
      }

    // Calculate absolute altitude
    target.altitude_available = basic.gps_altitude_available;
    if (target.altitude_available)
      target.altitude = target.relative_altitude +
                        RoughAltitude(basic.gps_altitude);

    // Calculate average climb rate
    target.climb_rate_avg30s_available = target.altitude_available;
    if (target.climb_rate_avg30s_available)
      target.climb_rate_avg30s =
        flarm_calculations.Average30s(target.id, basic.time, target.altitude);

    // The following calculations are only relevant for targets
    // where information is missing
    if (target.track_received     &&
        target.turn_rate_received &&
        target.speed_received     && 
        target.climb_rate_received)
      continue;

    // Check if the target has been seen before in the last seconds
    const FlarmTarget* last_target = 
               last_flarm.traffic.FindFlarmTraffic(target.id);
    if (last_target == nullptr || !last_target->valid)
      continue; // last_target is either not FLARM or not valid.

    // Calculate the time difference between now and the last contact
    const auto dt = target.valid.GetTimeDifference(last_target->valid);
    if (dt.count() > 0)
      {
      // Calculate the immediate climb rate
      if (!target.climb_rate_received)
        target.climb_rate = (target.relative_altitude -
                             last_target->relative_altitude) / 
                             dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!target.climb_rate_received)
        target.climb_rate = last_target->climb_rate;
      }

    if (dt.count() > 0                   &&
        target.location_available        &&
        last_target->location_available)
      {
      // Calculate the GeoVector between now and the last contact
      GeoVector vec =
        last_target->location.DistanceBearing(target.location);

      if (!target.track_received)
        target.track = vec.bearing;

      // Calculate the turn rate
      if (!target.turn_rate_received)
        {
        Angle turn_rate = target.track - last_target->track;
        target.turn_rate = turn_rate.AsDelta().Degrees() / dt.count();
        }

      // Calculate the speed [m/s]
      if (!target.speed_received)
        target.speed = vec.distance / dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!target.track_received)
        target.track = last_target->track;

      if (!target.turn_rate_received)
        target.turn_rate = last_target->turn_rate;

      if (!target.speed_received)
        target.speed = last_target->speed;
      }
    }
  }
