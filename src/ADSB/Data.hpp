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

#ifndef XCSOAR_ADSB_DATA_HPP
#define XCSOAR_ADSB_DATA_HPP

//#include "FLARM/Error.hpp"
//#include "FLARM/Version.hpp"
#include "ADSB/Status.hpp"
#include "ADSB/List.hpp"

#include <type_traits>

/**
 * A container for all data received by a ADSB receiver.
 */
struct AdsbData
  {
//  FlarmError error;

//  FlarmVersion version;

  AdsbStatus status;

  TrafficList traffic;

  bool IsDetected() const 
    {
    return this->status.available || !this->traffic.IsEmpty();
    }

  /**
   * Reset all state in this object.
   */
  void Clear()
    {
//    this->error.Clear();
//    this->version.Clear();
    this->status.Clear();
    this->traffic.Clear();
    }

  void Complement(const AdsbData &add)
    {
//    this->error.Complement(add.error);
//    this->version.Complement(add.version);
    this->status.Complement(add.status);
    this->traffic.Complement(add.traffic);
    }

  void Expire(TimeStamp clock) noexcept
    {
//    this->error.Expire(clock);
//    this->version.Expire(clock);
    this->status.Expire(clock);
    this->traffic.Expire(clock);
    }
};

static_assert(std::is_trivial<AdsbData>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_DATA_HPP
