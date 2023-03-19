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
  FlarmListConstDecorator(const TargetList& target_list)
    : TargetListConstDecorator(target_list)
    {
    }

  /**
   * Looks up an item in the traffic list.
   * @param id Target id
   * @return the \ref FlarmTarget pointer, NULL if not found
   */
  FlarmPtr FindTraffic(TargetId id)
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return the \ref FlarmTarget pointer, NULL if not found
   */
  const FlarmPtr FindTraffic(TargetId id) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The RemoteTarget pointer, NULL if not found
   */
  FlarmPtr FindTraffic(const TCHAR *name)
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FindTraffic(name));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref RemoteTarget pointer, NULL if not found
   */
  const FlarmPtr FindTraffic(const TCHAR *name) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FindTraffic(name));
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  const FlarmPtr PreviousTraffic(size_t i) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.PreviousTraffic(i));
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or null if is already at begin().
   */
  const FlarmPtr PreviousTraffic(const TargetPtr target) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.PreviousTraffic(target));
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  FlarmPtr NextTraffic(size_t i) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.NextTraffic(i));
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  FlarmPtr NextTraffic(const TargetPtr target) const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.NextTraffic(target));
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  const FlarmPtr FirstTraffic() const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FirstTraffic());
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  const FlarmPtr LastTraffic() const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.LastTraffic());
    }

  /**
   * Finds the most critical alert.  Returns NULL if there is no
   * alert.
   */
  const FlarmPtr FindMaximumAlert() const
    {
    return FlarmListConstDecorator::FlarmCast(this->target_list.FindMaximumAlert());
    }

private:
  static const FlarmPtr FlarmCast(const TargetPtr t)
    {
    return std::dynamic_pointer_cast<FlarmTarget>(t);
    }
  };

/**
 * \}
 */
