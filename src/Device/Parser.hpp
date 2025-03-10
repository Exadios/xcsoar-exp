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

#pragma once

#include "time/Stamp.hpp"

struct NMEAInfo;
class NMEAInputLine;
struct GeoPoint;
struct BrokenDate;
struct BrokenTime;

/**
 * Objects of this class parse NMEA records. The generic record types parsed 
 * are "GSA", "GLL", "RMC", "GGA", "HDM", "MVW". The proprietary record types
 * parsed are "PTAS1", "PFLAE", "PFLAV", "PFLAA", "PFLAU", "PGRMZ"
 */
class NMEAParser
{
  TimeStamp last_time;

public:
  bool real;

  bool use_geoid;

public:
  NMEAParser();

  /**
   * Resets the NMEAParser. The parser is set to use real input data and 
   * apply locally calculated geoidal separation (aka "geoidal height")
   * to GPS altitude.
   */
  void Reset();

  /**
   * Set the NMEAInfo.gps.real to argument.
   * @param real The argument to NMEAinfo.gps.real
   */
  void SetReal(bool _real) {
    real = _real;
  }

  /**
   * Disable application of locally calculated geoidal seperation to GPS
   * altitude (because, presumably, it has already been applied by the GPS
   * device.
   */
  void DisableGeoid() {
    use_geoid = false;
  }

  /**
   * Parses a provided NMEA String into a NMEA_INFO struct
   * @param line NMEA string
   * @param info NMEA_INFO output struct
   * @return Parsing success
   */
  bool ParseLine(const char *line, NMEAInfo &info);

public:
  /**
   * Calculates the checksum of the provided NMEA string and
   * compares it to the provided checksum
   * @param String NMEA string
   * @return True if checksum correct
   */
  static bool NMEAChecksum(const char *string);

  /**
   * Checks whether time has advanced since last call and
   * updates the last_time reference if necessary
   * @return True if time has advanced since last call
   */
  static bool TimeHasAdvanced(TimeStamp this_time,
                              TimeStamp &last_time,
                              NMEAInfo &info);

  static bool ReadGeoPoint(NMEAInputLine &line, GeoPoint &value_r);

  static bool ReadDate(NMEAInputLine &line, BrokenDate &date);

  /**
   * Read and parse a time stamp in the form "HHMMSS.SSS".
   */
  static bool ReadTime(NMEAInputLine &line, BrokenTime &broken_time,
                       TimeStamp &time_of_day_s) noexcept;

private:
  /**
   * Checks whether time has advanced since last call and
   * updates the GPS_info if necessary
   * @param ThisTime Current time
   * @param info NMEA_INFO struct to update
   * @return True if time has advanced since last call
   */
  bool TimeHasAdvanced(TimeStamp this_time, NMEAInfo &info) noexcept;

  /**
   * Parses a GLL sentence
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  bool GLL(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a GGA sentence
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  bool GGA(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a GSA sentence
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  bool GSA(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a RMC sentence
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  bool RMC(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a HDM sentence
   */
  bool HDM(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a PGRMZ sentence (Garmin proprietary).
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  bool RMZ(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a PTAS1 sentence (Tasman Instruments proprietary).
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  static bool PTAS1(NMEAInputLine &line, NMEAInfo &info);

  /**
   * Parses a MWV sentence (NMEA Wind information).
   *
   * @param line A NMEAInputLine instance that can be used for parsing
   * @param info NMEA_INFO struct to parse into
   * @return Parsing success
   */
  static bool MWV(NMEAInputLine &line, NMEAInfo &info);
};
