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

#ifndef XCSOAR_ADSB_DATA_HPP
#define XCSOAR_ADSB_DATA_HPP

#include "ADSB/Error.hpp"
#include "ADSB/List.hpp"

#include <type_traits>

/**
 * A container for all data received by a ADSB.
 */
struct AdsbData
  {

public:
  AdsbError error;

  AdsbTrafficList traffic;

  bool IsDetected() const
    {
    return !this->traffic.IsEmpty();
    }

  void Clear()
    {
    this->error.Clear();
    this->traffic.Clear();
    }

  void Complement(const AdsbData &add)
    {
    this->error.Complement(add.error);
    this->traffic.Complement(add.traffic);
    }

  void Expire(double clock)
    {
    this->error.Expire(clock);
    this->traffic.Expire(clock);
    }
  };

static_assert(std::is_trivial<AdsbData>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_DATA_HPP
