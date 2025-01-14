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

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "Surveillance/TargetStatus.hpp"
#include "Surveillance/TargetList.hpp"
#include "Surveillance/Flarm/FlarmSystemStatus.hpp"
#include "Surveillance/Adsb/AdsbSystemStatus.hpp"
#include "FLARM/Error.hpp"

#include <type_traits>

/**
 * A container for all data received by a target tracking device.
 */
struct TargetData
  {
  FlarmError flarm_error;

  TargetStatus status;

  TargetList traffic;

  FlarmSystemStatus flarm_status;
  
  AdsbSystemStatus  adsb_status;

#ifndef NDEBUG
  /**
   * Temporary ctor.
   */
  TargetData()
    {
    }

#endif

  /**
   * Copy an object of this type via an assignment operator.
   * @param rhs The RHS of the equal operator.
   * @return This object as copied from rhs.
   */
  TargetData& operator=(const TargetData& rhs)
    {
    this->status       = rhs.status;
    this->traffic      = rhs.traffic;
    this->flarm_status = rhs.flarm_status;
    this->adsb_status  = rhs.adsb_status;
    return *this;
    }

  constexpr bool IsDetected() const noexcept
    {
    return this->status.available || !this->traffic.IsEmpty();
    }

  /**
   * This is a NOP. Do not clear the status and traffic variables here
   * because a "GO" status can represent the status of more than one
   * device. Instead allow any GO status to expire as a function of time.
   */
  /* constexpr */ void Clear() noexcept
    {
    this->flarm_error.Clear();
    this->flarm_status.Clear();
    this->adsb_status.Clear();
    this->traffic.Clear();
    }

  /* constexpr */ void Complement(const TargetData& add) noexcept
    {
    this->status.Complement(add.status);

    this->traffic.Complement(add.traffic);
    }

  /**
   * Expire the GO status of this object and the target list.
   * @param clock Now.
   */
  void Expire(TimeStamp clock) noexcept
    {
    this->status.Expire(clock);
    this->traffic.Expire(clock);
    }
  };

/**
 * \}
 */
//static_assert(std::is_trivial<TargetData>::value, "type is not trivial");
