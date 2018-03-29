/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#include "StaticParser.hpp"
#include "NMEA/InputLine.hpp"
#include "NMEA/Info.hpp"
#include "FLARM/Error.hpp"
#include "FLARM/Version.hpp"
#include "FLARM/Status.hpp"
#include "FLARM/List.hpp"
#include "Util/Macros.hpp"
#include "Util/StringAPI.hxx"
#include "Geo/GeoVector.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>

namespace pt = boost::property_tree;

//------------------------------------------------------------------------------
void
ParseDump1090(const char *line, NMEAInfo &info, double clock)
  {
  TrafficList &flarm = info.flarm.traffic;
  FlarmTraffic traffic;

  flarm.modified.Update(clock);

  if ((line[0] != '[') || (line[1] != '{'))
    return;   // Its not a dump1090 JSON, must be something else.

  pt::ptree root;
  std::stringstream json;

  /*
   * The data contained in 'line' is as follows:
   *
   * * - now: the time this file was generated, in seconds since
   *     Jan 1 1970 00:00:00 GMT (the Unix epoch).
   * * - messages: the total number of Mode S messages processed since
   *     dump1090 started.
   * * - aircraft: an array of JSON objects, one per known aircraft. Each
   *     aircraft has the following keys. Keys will be omitted if data is
   *     not available.
   *   * - hex: the 24-bit ICAO identifier of the aircraft, as 6 hex digits.
   *       The identifier may start with '~', this means that the address is
   *       a non-ICAO address (e.g. from TIS-B).
   *   * - squawk: the 4-digit squawk (octal representation)
   *   * - flight: the flight name / callsign
   *   * - lat, lon: the aircraft position in decimal degrees
   *   * - nucp: the NUCp (navigational uncertainty category) reported for
   *       the position
   *   * - seen_pos: how long ago (in seconds before "now") the position was
   *       last updated
   *   * - altitude: the aircraft altitude in feet, or ¿ground¿ if it is
   *       reporting it is on the ground
   *   * - vert_rate: vertical rate in feet/minute
   *   * - track: true track over ground in degrees (0-359)
   *   * - speed: reported speed in kt. This is usually speed over ground,
   *       but might be IAS - you can't tell the difference here, sorry!
   *   * - messages: total number of Mode S messages received from this aircraft
   *   * - seen: how long ago (in seconds before ¿now¿) a message was last
   *       received from this aircraft
   *   * - rssi: recent average RSSI (signal power), in dbFS; this will always
   *       be negative.
   *
   */
  json << line;

  pt::read_json(json, root);

  if ((root.get_optional<std::string>("now")      == boost::none ||
      (root.get_optional<std::string>("messages") == boost::none)))
    return;   // This is probably not the JSON from dump1090!

  for (auto it = root.get_child("aircraft").begin();
       it != root.get_child("aircraft").end();
       ++it)
    {
    pt::ptree ac = it->second;

    // ICAO hex id.
    boost::optional<std::string> id = ac.get_optional<std::string>("hex");
    if (id == boost::none)
      return;   // Whoops!
    traffic.id = FlarmId::Parse(id->c_str(), nullptr);

    // The distance {n, e} in meters.
    boost::optional<double> phi    = ac.get_optional<double>("lat");
    boost::optional<double> lambda = ac.get_optional<double>("lon");
    if ((phi == boost::none) || (lambda == boost::none))
      return;   // We need a position!
    GeoPoint n, e;

    n.latitude  = Angle::Degrees(*phi);
    n.longitude = info.location.longitude;
    e.latitude  = info.location.latitude;
    e.longitude = Angle::Degrees(*lambda);
    GeoVector rn = info.location.DistanceBearing(n);
    GeoVector re = info.location.DistanceBearing(e);
    if ((rn.bearing.Degrees() > 90) && (rn.bearing.Degrees() < 270))
      traffic.relative_north = -rn.distance;
    else
      traffic.relative_north =  rn.distance;
    if ((re.bearing.Degrees() > 180) && (re.bearing.Degrees() < 360))
      traffic.relative_east = -re.distance;
    else
      traffic.relative_east =  re.distance;

    // Call sign.
    boost::optional<std::string> callsign = ac.get_optional<std::string>("flight");
    if (callsign != boost::none)
      {
      ;   // TODO /todo Do something here, but what?
      }

    // The height difference in meters.
    traffic.relative_altitude = 0;  // TODO /todo find our height and compute.

    // Track.
    boost::optional<double> t = ac.get_optional<double>("track");
    if (t == boost::none)
      traffic.track = Angle::Zero();
    else
      traffic.track = Angle::Degrees(*t);

    // Turn rate is not known for dump1090.
    traffic.turn_rate = 0;

    // Speed in meters per sec.
    boost::optional<double> s = ac.get_optional<double>("speed");
    if (t == boost::none)
      traffic.speed = 0;
    else
      traffic.speed = (*s) * 0.514444;

    // Vert rate in meters / sec.
    boost::optional<double> v = ac.get_optional<double>("vert_rate");
    if (v == boost::none)
      traffic.climb_rate = 0;
    else
      traffic.climb_rate = (*v) * 0.00508;

    // Aircraft type is not known.
    traffic.type = FlarmTraffic::AircraftType::POWERED_AIRCRAFT;
    }
  FlarmTraffic *flarm_slot = flarm.FindTraffic(traffic.id);
  if (flarm_slot == nullptr)
    {
    flarm_slot = flarm.AllocateTraffic();
    if (flarm_slot == nullptr)
      // no more slots available
      return;

    flarm_slot->Clear();
    flarm_slot->id = traffic.id;

    flarm.new_traffic.Update(clock);
    }

  // set time of fix to current time
  flarm_slot->valid.Update(clock);

  flarm_slot->Update(traffic);
  }
