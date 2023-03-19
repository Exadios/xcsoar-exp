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

#pragma once

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/Adsb/AdsbAircraftType.hpp"

#include <type_traits>
#include <memory>

struct AdsbTarget : public RemoteTarget
  {

#if 0
  /**
   * Set aircraft type.
   * @param type The ADSB aircraft type as reported by the driver.
   * @return If the value type is legal then true.
   */
  bool Type(unsigned int type) override
   {
   return this->type.Type(type);
   }
#endif

  /**
   * Is definitely a powered type.
   * @return If the target definitely a powered type then true.
   */
  bool IsPowered() const override
    {
    return this->type.IsPowered();
    }

  /**
   * The data from this target is not from a collision avoidance system.
   * @return Always false.
   */
  bool CollisionAvoidance() const override
    {
    return false;
    }

  /**
   * The data from this target is from a traffic awareness system.
   * @return Always true.
   */
  bool TrafficAwareness() const override
    {
    return true;
    }
  };

typedef std::shared_ptr<AdsbTarget> AdsbPtr;

/**
 * \}
 */
