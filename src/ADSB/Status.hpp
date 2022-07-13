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

#ifndef XCSOAR_ADSB_STATUS_HPP
#define XCSOAR_ADSB_STATUS_HPP

#include "ADSB/Traffic.hpp"
#include "NMEA/Validity.hpp"

#include <type_traits>

/**
 * @file
 * The ADSB operation status.
 */
struct AdsbStatus
  {
  enum class GPSStatus: uint8_t 
    {
    NO_GO = 0,
    GO    = 1,
    };

  /** Number of received ADSB targets */
  unsigned short rx;

  /** Transmit status */
  bool tx;

  /** GPS status */
  GPSStatus gps;

  /** Is ADSB information available? */
  Validity available;

  /**
   * Clear available status.
   */
  void Clear()
    {
    this->available.Clear();
    }

  /**
   * Merge another status into this one.
   * @param rhs The status to merge with this one.
   */
  void Complement(const AdsbStatus &rhs)
    {
    if (!this->available && rhs.available)
      *this = rhs;
    }

  /**
   * Expire a stale entry.
   * @param clock Now.
   */
  void Expire(TimeStamp clock) noexcept
    {
    this->available.Expire(clock, std::chrono::seconds(10));
    }
  };

static_assert(std::is_trivial<AdsbStatus>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_STATUS_HPP
