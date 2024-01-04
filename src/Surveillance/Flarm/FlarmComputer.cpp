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

#include "Surveillance/Flarm/FlarmComputer.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Flarm/FlarmListDecorator.hpp"
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
  FlarmListDecorator fld(&flarm.traffic);
  TargetList flarm_targets = fld.FlarmTargets();
  for (auto &target : flarm.traffic.list)
    {
      FlarmPtr flarm_target = FlarmTarget::Cast2FlarmPtr(target);
    if (flarm_target == nullptr)
      continue; // This is not a FLARM target.

    // if we don't know the target's name yet
    if (!flarm_target->HasName())
      {
      // lookup the name of this target's id
      const TCHAR *fname = 
                     FlarmDetails::LookupCallsign(flarm_target->id);
      if (fname != NULL)
        flarm_target->name = fname;
      }

    // Calculate distance
    flarm_target->distance = hypot(flarm_target->relative_north,
                                   flarm_target->relative_east);

    // Calculate Location
    flarm_target->location_available = basic.location_available;
    if (flarm_target->location_available)
      {
      flarm_target->location.latitude =
        Angle::Degrees(flarm_target->relative_north * north_to_latitude) +
        basic.location.latitude;

      flarm_target->location.longitude =
        Angle::Degrees(flarm_target->relative_east * east_to_longitude) +
        basic.location.longitude;
      }

    // Calculate absolute altitude
    flarm_target->altitude_available = basic.gps_altitude_available;
    if (flarm_target->altitude_available)
      flarm_target->altitude = flarm_target->relative_altitude +
                        RoughAltitude(basic.gps_altitude);

    // Calculate average climb rate
    flarm_target->climb_rate_avg30s_available = 
      flarm_target->altitude_available;
    if (flarm_target->climb_rate_avg30s_available)
      flarm_target->climb_rate_avg30s =
        this->flarm_calculations.Average30s(flarm_target->id,
                                            basic.time,
                                            flarm_target->altitude);

    // The following calculations are only relevant for targets
    // where information is missing
    if (flarm_target->track_received     &&
        flarm_target->turn_rate_received &&
        flarm_target->speed_received     && 
        flarm_target->climb_rate_received)
      continue;

    // Check if the target has been seen before in the last seconds
    const FlarmPtr last_target = std::dynamic_pointer_cast<FlarmTarget>(
               last_flarm.traffic.FindTraffic(flarm_target->id));
    if (last_target == nullptr || !last_target->valid)
      continue; // last_target is either not FLARM or not valid.

    // Calculate the time difference between now and the last contact
    const auto dt = flarm_target->valid.GetTimeDifference(last_target->valid);
    if (dt.count() > 0)
      {
      // Calculate the immediate climb rate
      if (!flarm_target->climb_rate_received)
        flarm_target->climb_rate = (flarm_target->relative_altitude -
                                    last_target->relative_altitude) / 
                                   dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!flarm_target->climb_rate_received)
        flarm_target->climb_rate = last_target->climb_rate;
      }

    if (dt.count() > 0                   &&
        flarm_target->location_available &&
        last_target->location_available)
      {
      // Calculate the GeoVector between now and the last contact
      GeoVector vec =
        last_target->location.DistanceBearing(flarm_target->location);

      if (!flarm_target->track_received)
        flarm_target->track = vec.bearing;

      // Calculate the turn rate
      if (!flarm_target->turn_rate_received)
        {
        Angle turn_rate = flarm_target->track - last_target->track;
        flarm_target->turn_rate = turn_rate.AsDelta().Degrees() / dt.count();
        }

      // Calculate the speed [m/s]
      if (!flarm_target->speed_received)
        flarm_target->speed = vec.distance / dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!flarm_target->track_received)
        flarm_target->track = last_target->track;

      if (!flarm_target->turn_rate_received)
        flarm_target->turn_rate = last_target->turn_rate;

      if (!flarm_target->speed_received)
        flarm_target->speed = last_target->speed;
      }
    }
  }
