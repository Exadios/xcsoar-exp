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

#include "Surveillance/TargetList.hpp"

/**
 * This is the decorator class for \ref TargetList.
 */
struct TargetListDecorator : public TargetList
  {
public:
  /**
   * Disallow default ctor.
   */
  TargetListDecorator() = delete;

  /**
   * Ctor.
   * @param target_list The \ref TargetList to decorate.
   */
  TargetListDecorator(TargetList& target_list)
    : target_list(target_list)
    {
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  const TargetPtr PreviousTraffic(size_t i) const
    {
    return this->target_list.PreviousTraffic(i);
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or null if is already at begin().
   */
  const TargetPtr PreviousTraffic(const TargetPtr target) const
    {
    return this->target_list.PreviousTraffic(target);
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  TargetPtr NextTraffic(size_t i) const
    {
    return this->target_list.NextTraffic(i);
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  TargetPtr NextTraffic(const TargetPtr target) const
    {
    return this->target_list.NextTraffic(target);
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  const TargetPtr FirstTraffic() const
    {
    return this->target_list.FirstTraffic();
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  const TargetPtr LastTraffic() const
    {
    return this->target_list.LastTraffic();
    }

  /**
   * Finds the most critical alert irrespective of target type.
   * @return Returns the maximum alert target of NULL if there is no
   * alert.
   */
  const TargetPtr FindMaximumAlert() const
    {
    return this->target_list.FindMaximumAlert();
    }

protected:
  TargetList& target_list;
  };

  /**
   * \}
   */
