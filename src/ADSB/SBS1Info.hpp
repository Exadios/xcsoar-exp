/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2018 The XCSoar Project
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

#ifndef XCSOAR_SBS1INFO_H
#define XCSOAR_SBS1INFO_H

#include "ADSB/AdsbData.hpp"
#include "NMEA/Validity.hpp"
#include "Geo/SpeedVector.hpp"

/**
 * A structure that holds all the parsed data read from the connected ADSB
 * SBS1 device.
 */
struct SBS1Info 
  {

public:

  /**
   * Default ctor.
   */
  SBS1Info() = default;

  /**
   * A monotonic wall clock time, in seconds, in an defined
   * epoch.  This may get updated even if the device doesn't send
   * any data.  It is used to update and check the #Validity
   * attributes in this struct.
   */
  double clock;

  /**
   * Is the device alive?  This attribute gets updated each time a
   * SBS1 line was successfully parsed.
   */
  Validity alive;

  /** A location is available for this aircraft. */
  Validity location_available;

  /** location of this aircraft */
  GeoPoint location;

  /** A track is available for this aircraft. */
  Validity track_available;

  /** track angle of this aircraft in degrees true */
  Angle track;

  //##############
  //   Altitude
  //##############

  Validity altitude_available;

  /** Altitude AMSL (m) */
  double altitude;

  //##########
  //   Time
  //##########

  /**
   * Did the device provide a time value?
   */
  Validity time_available;

  /**
   * Global time (seconds after UTC midnight)
   */
  double time;

  //###########
  //   Vario
  //###########

  /**
   * Is an external gross, non-compensated, vertical speed vario signal present?
   * @see VerticalRate
   */
  Validity vertical_rate_available;

    /**
   * Gross, non-compensated aircraft vertical speed (m/s, up positive)
   * @see VerticalRateAvailable
   */
  double vertical_rate;

  //###########
  // Id
  //###########

  /**
   * The ICAO id.
   */
  unsigned int icao;

  /**
   * A call sign has been received.
   */
  Validity call_sign_valid;

  /**
   * The call sign.
   */
  TCHAR *call_sign;

  AdsbData adsb;

  void UpdateClock();

  /**
   * Returns the altitude if available.
   * @return If an altitude is available the <true, altitude>, otherwise
   *         <false, 0.0>.
   */
  gcc_pure
  std::pair<bool, double> GetAnyAltitude() const
    {
    return altitude_available ? std::make_pair(true, altitude) :
                                std::make_pair(false, 0.);
    }

  /**
   * Set the gross, non-compensated, plain-old vertical speed vario value [m/s].
   */
  void ProvideNoncompVario(double value)
    {
    vertical_rate = value;
    vertical_rate_available.Update(clock);
    }

  /**
   * Clears all information, start with tabula rasa.
   */
  void Reset();

  /**
   * Check the expiry time of the device connection with the wall
   * clock time.  This should be called from a periodic timer.  The
   * GPS time cannot be used here, because a disconnected device would
   * not update its GPS time.
   */
  void ExpireWallClock();

  /**
   * Check expiry times of all attributes which have a time stamp
   * associated with them.  This should be called after the GPS time
   * stamp has been updated.
   */
  void Expire();

  /**
   * Adds data from the specified object, unless already present in
   * this one.
   *
   * Note that this does not copy calculated values which are managed
   * outside of the NMEA parser.
   */
  void Complement(const SBS1Info &add);
  };

static_assert(std::is_trivial<SBS1Info>::value, "type is not trivial");

#endif  // XCSOAR_SBS1INFO_H
