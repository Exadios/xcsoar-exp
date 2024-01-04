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

#include "Surveillance/Adsb/AdsbComputer.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"
#include "Surveillance/TargetData.hpp"
#include "NMEA/Info.hpp"
#include "Surveillance/Adsb/AdsbCalculations.hpp"
#include "Geo/GeoVector.hpp"

//------------------------------------------------------------------------------
AdsbComputer::AdsbComputer()
  {
  }

//------------------------------------------------------------------------------
AdsbComputer::~AdsbComputer()
  {
  }

//------------------------------------------------------------------------------
void
AdsbComputer::Process(TargetData& adsb,
                      TargetData& last_adsb,
                      const NMEAInfo& basic)
  {
  if (basic.time_available)
    this->adsb_calculations.CleanUp(basic.time);

  if (!adsb.IsDetected())
    return;

  if (basic.location_available)
    {
    // Pre-calculate relative east and north projection to lat / lon
    // for Location calculations of each target
    constexpr Angle delta_lat = Angle::Degrees(0.01);
    constexpr Angle delta_lon = Angle::Degrees(0.01);

    /* Compute the "flat earth" vector of (0.01 deg, 0.01 deg) for our
     * present location. Then compute the phi and lambda distances that 
     * corresponds to this vector. */
    GeoPoint plat = basic.location;
    plat.latitude += delta_lat;
    GeoPoint plon = basic.location;
    plon.longitude += delta_lon;
    double dlat = basic.location.DistanceS(plat);
    double dlon = basic.location.DistanceS(plon);
    double latitude_to_north(0);
    double longitude_to_east(0);

    if (fabs(dlat) > 0 && fabs(dlon) > 0)
      {
      /* Meters per degree for phi, lambda at our current location. */
      latitude_to_north = dlat / delta_lat.Degrees();
      longitude_to_east = dlon / delta_lon.Degrees();
      }
    else
      {
      // for each item in traffic
      for (auto &target : adsb.traffic.list) 
        {
        AdsbPtr adsb_target = std::dynamic_pointer_cast<AdsbTarget>(target);
        if (adsb_target == nullptr)
          continue; // This is not an ADSB target.

        // Calculate distance
        Angle delta_phi    = target->location.latitude  -
                             basic.location.latitude;
        Angle delta_lambda = target->location.longitude -
                             basic.location.longitude;
        adsb_target->relative_north = delta_phi.Degrees()    * latitude_to_north;
        adsb_target->relative_east  = delta_lambda.Degrees() * longitude_to_east;
        adsb_target->distance = hypot(target->relative_north, target->relative_east);

        // Calculate absolute altitude
        adsb_target->altitude_available = basic.gps_altitude_available;
        if (adsb_target->altitude_available)
          adsb_target->altitude = target->relative_altitude +
                             RoughAltitude(basic.gps_altitude);

        // Calculate average climb rate
        adsb_target->climb_rate_avg30s_available = adsb_target->altitude_available;
        if (adsb_target->climb_rate_avg30s_available)
          adsb_target->climb_rate_avg30s =
              this->adsb_calculations.Average30s(adsb_target->id,
                                                 basic.time,
                                                 adsb_target->altitude);

        // The following calculations are only relevant for targets
        // where information is missing
        if (adsb_target->track_received && 
            adsb_target->speed_received &&
            adsb_target->climb_rate_received)
          continue;

        // Check if the target has been seen before in the last seconds
        const AdsbPtr last_target = std::dynamic_pointer_cast<AdsbTarget>(
                  last_adsb.traffic.FindTraffic(adsb_target->id));
        if (last_target == nullptr || !last_target->valid)
          continue; // last_target is either not ADSB or not valid.

        // Calculate the time difference between now and the last contact
        const auto dt = target->valid.GetTimeDifference(last_target->valid);
        if (dt.count() > 0)
          {
          // Calculate the immediate climb rate
          if (!adsb_target->climb_rate_received)
            adsb_target->climb_rate =
              (adsb_target->relative_altitude - last_target->relative_altitude) /
              dt.count();
          }
        else
          {
          // Since the time difference is zero (or negative)
          // we can just copy the old values
          if (!adsb_target->climb_rate_received)
            adsb_target->climb_rate = last_target->climb_rate;
          }

        if (dt.count() > 0             &&
            adsb_target->location_available &&
            last_target->location_available) 
          {
          // Calculate the GeoVector between now and the last contact
          GeoVector vec = last_target->location.DistanceBearing(target->location);

          if (!target->track_received)
            adsb_target->track = vec.bearing;

          // Calculate the speed [m/s]
          if (!target->speed_received)
            adsb_target->speed = vec.distance / dt.count();
          }
        else
          {
          // Since the time difference is zero (or negative)
          // we can just copy the old values
          if (!adsb_target->track_received)
            adsb_target->track = last_target->track;

          if (!adsb_target->speed_received)
            adsb_target->speed = last_target->speed;
          }
        }
      }
    }
  }
