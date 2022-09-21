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

#include "ADSB/AdsbComputer.hpp"
#include "NMEA/Info.hpp"
#include "Geo/GeoVector.hpp"

#ifndef NDEBUG
#include "LogFile.hpp"
#endif

//------------------------------------------------------------------------------
void
AdsbComputer::Process(AdsbData &adsb,
                      const AdsbData &last_adsb,
                      const NMEAInfo &basic)
  {
  // Cleanup old calculation instances
  if (basic.time_available)
    this->adsb_calculations.CleanUp(basic.time);

  // if (ADSB data is available)
  if (!adsb.IsDetected())
    return;

  double latitude_to_north(0);
  double longitude_to_east(0);

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

    if (fabs(dlat) > 0 && fabs(dlon) > 0)
      {
      /* Meters per degree for phi, lambda at our current location. */
      latitude_to_north = dlat / delta_lat.Degrees();
      longitude_to_east = dlon / delta_lon.Degrees();
      }
    }

  // for each item in traffic
  for (auto &traffic : adsb.traffic.list) 
    {

#ifndef NDEBUG
//    LogFormat("%s, %d: %lf, %lf", __FILE__, __LINE__,
//                                  basic.location.latitude.Degrees(),
//                                  traffic.location.latitude.Degrees());
//    LogFormat("%s, %d: %lf, %lf", __FILE__, __LINE__,
//                                  basic.location.longitude.Degrees(),
//                                  traffic.location.longitude.Degrees());
#endif
    // Calculate distance
    Angle delta_phi    = traffic.location.latitude  - basic.location.latitude;
    Angle delta_lambda = traffic.location.longitude - basic.location.longitude;
    traffic.relative_north = delta_phi.Degrees()    * latitude_to_north;
    traffic.relative_east  = delta_lambda.Degrees() * longitude_to_east;
    traffic.distance = hypot(traffic.relative_north, traffic.relative_east);

#ifndef NDEBUG
//    LogFormat("%s, %d: %lf, %lf", __FILE__, __LINE__,
//                                 delta_phi.Degrees(), delta_lambda.Degrees());
    double s = traffic.distance;
    LogFormat("%s, %d: %lf", __FILE__, __LINE__, s);
#endif
    // Calculate absolute altitude
    traffic.altitude_available = basic.gps_altitude_available;
    if (traffic.altitude_available)
      traffic.altitude = traffic.relative_altitude + RoughAltitude(basic.gps_altitude);

    // Calculate average climb rate
    traffic.climb_rate_avg30s_available = traffic.altitude_available;
    if (traffic.climb_rate_avg30s_available)
      traffic.climb_rate_avg30s =
        this->adsb_calculations.Average30s(traffic.id,
                                           basic.time,
                                           traffic.altitude);

    // The following calculations are only relevant for targets
    // where information is missing
    if (traffic.track_received && traffic.turn_rate_received &&
        traffic.speed_received && traffic.climb_rate_received)
      continue;

    // Check if the target has been seen before in the last seconds
    const AdsbTraffic *last_traffic =
      last_adsb.traffic.FindTraffic(traffic.id);
    if (last_traffic == NULL || !last_traffic->valid)
      continue;

    // Calculate the time difference between now and the last contact
    const auto dt = traffic.valid.GetTimeDifference(last_traffic->valid);
    if (dt.count() > 0)
      {
      // Calculate the immediate climb rate
      if (!traffic.climb_rate_received)
        traffic.climb_rate =
          (traffic.relative_altitude - last_traffic->relative_altitude) / dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!traffic.climb_rate_received)
        traffic.climb_rate = last_traffic->climb_rate;
      }

    if (dt.count() > 0 &&
        traffic.location_available &&
        last_traffic->location_available) 
      {
      // Calculate the GeoVector between now and the last contact
      GeoVector vec = last_traffic->location.DistanceBearing(traffic.location);

      if (!traffic.track_received)
        traffic.track = vec.bearing;

      // Calculate the speed [m/s]
      if (!traffic.speed_received)
        traffic.speed = vec.distance / dt.count();
      }
    else
      {
      // Since the time difference is zero (or negative)
      // we can just copy the old values
      if (!traffic.track_received)
        traffic.track = last_traffic->track;

      if (!traffic.speed_received)
        traffic.speed = last_traffic->speed;
      }
    }
  }
