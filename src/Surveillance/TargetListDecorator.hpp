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

#include "Surveillance/TargetList.hpp"
#include "Surveillance/TargetListBase.hpp"

/**
 * This is the decorator class for \ref TargetList.
 */
struct TargetListDecorator : public TargetListBase
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
  TargetListDecorator(TargetList* target_list)
    : target_list(target_list)
    {
    }

  /**
   * Virtual dtor for derived classes.
   */
  virtual ~TargetListDecorator()
    {
    }

  /**
   * Clear everything!
   */
  constexpr void Clear()
    {
    this->target_list->Clear();
    }

  /**
   * Check whether the list is empty.
   * @return If true thens it is empty.
   */
  constexpr bool IsEmpty() const
    {
    return this->target_list->IsEmpty();
    }

  /**
   * Delete stale targets.
   * @param clock Now.
   */
  void Expire(TimeStamp clock) noexcept
    {
    this->target_list->Expire(clock);
    }

  /**
   * Number of items currently in the list.
   * @return Number of targets in the list.
   */
  unsigned GetActiveTrafficCount() const
    {
    return this->target_list->GetActiveTrafficCount();
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  const TargetPtr PreviousTraffic(size_t i) const
    {
    return this->target_list->PreviousTraffic(i);
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or null if is already at begin().
   */
  const TargetPtr PreviousTraffic(const TargetPtr target) const
    {
    return this->target_list->PreviousTraffic(target);
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  const TargetPtr NextTraffic(size_t i) const
    {
    return this->target_list->NextTraffic(i);
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  const TargetPtr NextTraffic(const TargetPtr target) const
    {
    return this->target_list->NextTraffic(target);
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  const TargetPtr FirstTraffic() const
    {
    return this->target_list->FirstTraffic();
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  const TargetPtr LastTraffic() const
    {
    return this->target_list->LastTraffic();
    }

  /**
   * Finds the most critical alert irrespective of target type.
   * @return Returns the maximum alert target of NULL if there is no
   * alert.
   */
  const TargetPtr FindMaximumAlert() const
    {
    return this->target_list->FindMaximumAlert();
    }

  /**
   * Convert a pointer to a element in the ordered list into an index into
   * the ordered list.
   * @param t The pointer to the element
   * @return The index corresponding to t.
   */
  virtual unsigned TrafficIndex(const TargetPtr* t) const
    {
    return this->target_list->TrafficIndex(t);
    }

  /**
   * Give a reference to the list variable.
   * @return This reference may be used to manipulate the implementations 
   *         List variable;
   */
  virtual TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List() override
    {
    return this->target_list->List();
    }

  /**
   * Give a reference to the list variable.
   * @return This reference may not be used to manipulate the implementations 
   *         List variable;
   */

  virtual TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List() const override
    {    
    return this->target_list->List();
    }

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to manipulate the implementations 
   *         modified variable;
   */
  virtual Validity* Modified() override;
  virtual const Validity* Modified() const override;

protected:
  TargetList* target_list;
  };

  /**
   * \}
   */
