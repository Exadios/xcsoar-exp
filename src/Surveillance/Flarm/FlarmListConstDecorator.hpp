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

#pragma once

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "Surveillance/TargetListConstDecorator.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"

/**
 * A decorator used to manage a \ref TargetList as a FlarmList.
 */
struct FlarmListConstDecorator : public TargetListConstDecorator
  {
  /**
   * Disallow default ctor.
   */
  FlarmListConstDecorator() = delete;

  /**
   * Ctor
   * @param target_list The \ref TargetList to decorate.
   */
  FlarmListConstDecorator(const TargetList* target_list)
    : TargetListConstDecorator(target_list)
    {
    }

  /**
   * Looks up an item in the traffic list.
   * @param id Target id
   * @return the \ref FlarmTarget pointer, NULL if not found
   */
  FlarmPtr FindFlarm(TargetId id)
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list->FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return the \ref FlarmTarget pointer, NULL if not found
   */
  const FlarmPtr FindFlarm(TargetId id) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list->FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref FlarmTarget pointer, NULL if not found
   */
  FlarmPtr FindFlarm(const TCHAR *name)
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list->FindTraffic(name));
    }

#if 1
  /**
   * Returns a \ref TargetList of FLARM targets.
   * @return The list of FLARM targets.
   */
  TargetList FlarmTargets() const;

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref FlarmTarget pointer, NULL if not found
   */
  const FlarmPtr FindFlarm(const TCHAR *name) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list->FindTraffic(name));
    }
#endif

private:
  static const FlarmPtr FlarmCast(const TargetPtr t)
    {
    return std::dynamic_pointer_cast<FlarmTarget>(t);
    }
  
#if 0
  /**
   * Inserts a new \ref FlarmTarget object to the list.
   * @param target A pointer to the target.
   * @return A pointer to the new element that has been added or nullptr
   *         if the list is full.
   */
  FlarmPtr AddFlarm(FlarmPtr target)
    {
    if (this->target_list->List()->full())
      return std::shared_ptr<FlarmTarget>(nullptr);   // Too bad.

    this->target_list->List()->append(target);
    return target;
    }
#endif
  };

/**
 * \}
 */
