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

    /* Meters per degree for phi, lambda at our current location. */
    latitude_to_north = dlat / delta_lat.Degrees();
    longitude_to_east = dlon / delta_lon.Degrees();
    // for each item in traffic
    for (size_t i = 0; i < adsb.traffic.adsb_list.size(); i++)
      {
      AdsbTarget& target = adsb.traffic.adsb_list[i];
//    for (auto& target : adsb.traffic.adsb_list) 
//      {
      // Calculate distance
      Angle delta_phi    = target.location.latitude  -
                           basic.location.latitude;
      Angle delta_lambda = target.location.longitude -
                           basic.location.longitude;
      target.relative_north = delta_phi.Degrees()    * latitude_to_north;
      target.relative_east  = delta_lambda.Degrees() * longitude_to_east;
      target.distance = hypot(target.relative_north, target.relative_east);

      /* Cut out this target if the range is too great.
       *
       * Too great a distance is arbitrarily defined here as twice the 
       * maximum range of the surveillance radar.
       */
      /**
       * \todo Somehow more closely connect this check here to the
       *       maximum range on the radar so that this value is a function
       *       of that code. (Issue 5)
      */
      if (target.distance > RoughDistance(20000))  // 20kM
        {
        adsb.traffic.adsb_list.quick_remove(i);
        continue;
        }

      // Calculate absolute altitude
      target.altitude_available = basic.gps_altitude_available;
      if (target.altitude_available)
        {
        target.relative_altitude = target.altitude -
                                   RoughAltitude(basic.gps_altitude);
        }

      // Calculate average climb rate
#ifdef ADSB_CLIMB_RATE
      /* When ready compute a climb rate for ADSB. The code below does not do
       * that.
       */
      /**
       * \todo pfb: Implement ADSB climb rate. (Issue 5)
       */
      target.climb_rate_avg30s_available = target.altitude_available;
      if (target.climb_rate_avg30s_available)
        target.climb_rate_avg30s =
            this->adsb_calculations.Average30s(target.id,
                                               basic.time,
                                               target.altitude);
#else
      target.climb_rate_avg30s_available = false;
#endif

      // The following calculations are only relevant for targets
      // where information is missing
      if (target.track_received && 
          target.speed_received &&
          target.climb_rate_received)
        continue;

      // Check if the target has been seen before in the last seconds
      const AdsbTarget* last_target = 
                last_adsb.traffic.FindAdsbTraffic(target.id);
      if (last_target == nullptr || !last_target->valid)
        continue; // last_target is either not ADSB or not valid.

      // Calculate the time difference between now and the last contact
      const auto dt = target.valid.GetTimeDifference(last_target->valid);
      if (dt.count() > 0)
        {
        // Calculate the immediate climb rate
        if (!target.climb_rate_received)
          target.climb_rate =
            (target.relative_altitude - last_target->relative_altitude) /
            dt.count();
        }
      else
        {
        // Since the time difference is zero (or negative)
        // we can just copy the old values
        if (!target.climb_rate_received)
          target.climb_rate = last_target->climb_rate;
        }

      if (dt.count() > 0            &&
          target.location_available &&
          last_target->location_available) 
        {
        // Calculate the GeoVector between now and the last contact
        GeoVector vec = last_target->location.DistanceBearing(target.location);

        if (!target.track_received)
          target.track = vec.bearing;

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

        if (!target.speed_received)
          target.speed = last_target->speed;
        }
      }
    }
  }
