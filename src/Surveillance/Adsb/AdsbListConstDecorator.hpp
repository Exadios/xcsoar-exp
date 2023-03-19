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
#include "Surveillance/Adsb/AdsbTarget.hpp"

/**
 * A decorator used to manage a \ref TargetList as a FlarmList.
 */
struct AdsbListConstDecorator : public TargetListConstDecorator
  {
  /**
   * Disallow default ctor.
   */
  AdsbListConstDecorator() = delete;

  /**
   * Ctor
   * @param target_list The \ref TargetList to decorate.
   */
  AdsbListConstDecorate(const TargetList& target_list)
    : TargetListConstDecorator(target_list)
    {
    }

  /**
   * Looks up an item in the traffic list.
   * @param id Target id
   * @return the \ref RemoteTarget pointer, NULL if not found
   */
  AdsbPtr FindTraffic(TargetId id)
    {
    for (auto &traffic : this->list)
      if (traffic->id == id)
        {
        traffic = std::make_shared<RemoteTarget>(*traffic);
        return traffic;
        }

    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return the \ref RemoteTarget pointer, NULL if not found
   */
  const TargetPtr FindTraffic(TargetId id) const
    {
    for (const auto &traffic : this->list)
      if (traffic->id == id)
        return std::make_shared<RemoteTarget>(*traffic);

    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The RemoteTarget pointer, NULL if not found
   */
  AdsbPtr FindTraffic(const TCHAR *name)
    {
    for (auto &traffic : this->list)
      if (traffic->name.equals(name))
        return std::make_shared<RemoteTarget>(*traffic);

    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref RemoteTarget pointer, NULL if not found
   */
  const AdsbPtr FindTraffic(const TCHAR *name) const
    {
    for (const auto &traffic : this->list)
      if (traffic->name.equals(name))
        return std::make_shared<RemoteTarget>(*traffic);

    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  const AdsbPtr PreviousTraffic(size_t i) const
    {
    return i > 0 ?
           this->list[i - 1] :
           nullptr;
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or null if is already at begin().
   */
  const AdsbPtr PreviousTraffic(const TargetPtr target) const
    {
    return this->PreviousTraffic(this->FindIndex(target));
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  AdsbPtr NextTraffic(size_t i) const
    {
    return (i + 1) < this->list.size() ?
           this->list[i + 1] :
           nullptr;
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  AdsbPtr NextTraffic(const TargetPtr target) const
    {
    return this->NextTraffic(this->FindIndex(target));
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  const AdsbPtr FirstTraffic() const
    {
    return this->list.empty() ?
           nullptr :
           *this->list.begin();
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  const AdsbPtr LastTraffic() const
    {
    return this->list.empty() ?
           nullptr :
           *(this->list.end() - 1);
    }

  /**
   * Finds the most critical alert.  Returns NULL if there is no
   * alert.
   */
  const AdsbPtr FindMaximumAlert() const;
  };

typedef std::dynamic_pointer_cast<AdsbList> AdsbCast;

/**
 * \}
 */
