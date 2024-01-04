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

#include "Surveillance/TargetListBase.hpp"
#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/TargetId.hpp"
#include "NMEA/Validity.hpp"
//#include "util/TrivialArray.hxx"
#include "Surveillance/TrivialArrayExtender.hpp"

#include <type_traits>
#include <list>
#include <memory>

#ifndef NDEBUG
#include <stdio.h>
#include "thread/Thread.hpp"
#endif

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

/**
 *This class keeps track of the target objects received from a
 * remote sensing device.
 */
struct TargetList : public TargetListBase
  {
public:
  static constexpr size_t MAX_COUNT = 50;

  /**
   * Ctor
   */
  TargetList()
    {
    this->Clear();
    this->type = TargetType::MIXED;
    }

  /**
   * Allow object copies by copy ctor.
   * @param src The source object.
   */
  TargetList(const TargetList& src);

  /**
   * Allow object copies by copy operator.
   * @param rhs The source object.
   * @return This object.
   */
  TargetList& operator=(const TargetList& rhs);

  /**
   * Time stamp of the latest modification to this object.
   */
  Validity modified;

  /**
   * When was the last new traffic received?
   */
  Validity new_traffic;

  /** Target information */
  TrivialArrayExtender<std::shared_ptr<RemoteTarget>, MAX_COUNT> list;
//  std::list<std::shared_ptr<RemoteTarget>> list;

  /**
   * Clear everything!
   */
  constexpr void Clear()
    {
    this->modified.Clear();
    this->new_traffic.Clear();
    this->list.clear();
    }

  /**
   * Test whether this list is empty.
   * @return If empty then true.
   */
  constexpr bool IsEmpty() const
    {
    return this->list.empty();
    }

  /**
   * Adds data from the specified object, unless already present in
   * this one.
   * @param add Source list.
   *
   * \bug
   * Adding \ref traffic here should not happen because the object 
   * added is a generic \RemoteTarget - that is not a specific 
   * \ref FlarmTarget or \ref AdsbTarget. The processing done in the
   * merge and calculation threads requires a specific target because
   * the processing is invariably difference for each.
   */

  constexpr void Complement(const TargetList& add) override
    {
    this->modified.Complement(add.modified);  // Don't know where this is done
                                              // for the FLARM merge.
    // Add unique traffic from 'add' list
    for (auto &traffic : add.list)
      {
      if (this->FindTraffic(traffic->id) == nullptr)
        {
        /**
         * \bug
         * Adding \ref traffic here should not happen because the object 
         * added is a generic \RemoteTarget - that is not a specific 
         * \ref FlarmTarget or \ref AdsbTarget. The processing done in the
         * merge and calculation threads requires a specific target because
         * the processing is invariably difference for each.
         */
        if (this->AddTarget(*traffic) == nullptr)
          return;
        }
      }
    }

  /**
   * Delete stale targets.
   * @param clock Time now.
   */
  void Expire(TimeStamp clock) noexcept
    {
    this->modified.Expire(clock, std::chrono::minutes(5));
    this->new_traffic.Expire(clock, std::chrono::minutes(1));

    for (unsigned i = this->list.size(); i-- > 0;)
      if (!this->list[i]->Refresh(clock))
        this->list.quick_remove(i);
    }

  /**
   * Give the number of items in this list.
   * @return Number of targets in the list.
   */
  constexpr unsigned GetActiveTrafficCount() const
    {
    return this->list.size();
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return the \ref RemoteTarget pointer, NULL if not found
   */
  TargetPtr FindTraffic(TargetId id)
    {
    for (auto &traffic : this->list)
      if (traffic->id == id)
        {
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
        return traffic;
    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The RemoteTarget pointer, NULL if not found
   */
  TargetPtr FindTraffic(const TCHAR* name)
    {
    for (auto &traffic : this->list)
      if (traffic->name.equals(name))
        return traffic;
    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref RemoteTarget pointer, NULL if not found
   */
  const TargetPtr FindTraffic(const TCHAR* name) const
    {
    for (const auto &traffic : this->list)
      if (traffic->name.equals(name))
        return traffic;
    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Allocates a new target object on the list.
   *
   * @note This is probably not the function that you want to use. You
   *       probably want to attach one of the decorators and use the 
   *       equivalent function in that decorator.
   *
   * @return If an object can be allocated on the this then a 
   *         \ref RemoteTarget pointer to that element. Otherwise
   *         nullptr.
   */
  TargetPtr AllocateTraffic()
    {
    if (!this->list.full())
      {
      TargetPtr ptr = std::make_shared<RemoteTarget>();
      this->list.append(ptr);
      return ptr;
      }
    return std::shared_ptr<RemoteTarget>(nullptr);
    }

  /**
   * Inserts a new \ref RemoteTarget object to the list.
   *
   * @note This is probably not the function that you want to use. You
   *       probably want to attach one of the decorators and use the 
   *       equivalent function in that decorator.
   *
   * @param The target.
   * @return A pointer to the new element that has been added or nullptr
   *         if the list is full.
   */
  TargetPtr AddTarget(const RemoteTarget& target)
    {
    TargetPtr ptr = this->AllocateTraffic();
    if (ptr == nullptr)
      return ptr;

    *ptr = target;
    return ptr;
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  const TargetPtr PreviousTraffic(size_t i) const
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
  const TargetPtr PreviousTraffic(const TargetPtr target) const
    {
    return this->PreviousTraffic(this->FindIndex(target));
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  const TargetPtr NextTraffic(size_t i) const
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
  const TargetPtr NextTraffic(const TargetPtr target) const
    {
    return this->NextTraffic(this->FindIndex(target));
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  const TargetPtr FirstTraffic() const
    {
    return this->list.empty() ?
           nullptr :
           *this->list.begin();
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  const TargetPtr LastTraffic() const
    {
    return this->list.empty() ?
           nullptr :
           *(this->list.end() - 1);
    }

  /**
   * Finds the most critical alert.  Returns NULL if there is no
   * alert.
   */
  const TargetPtr FindMaximumAlert() const;

  /**
   * Convert a pointer to a element in the ordered list into an index into
   * the ordered list.
   * @param t The pointer to the element
   * @return The index corresponding to t.
   */
  unsigned TrafficIndex(const TargetPtr* t) const
    {
    return t - this->list.begin();
    }

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to manipulate the implementations 
   *         modified variable;
   */
  virtual Validity* Modified()
    {
    return &this->modified;
    }

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to read the implementations 
   *         modified variable;
   */
  virtual const Validity* Modified() const
    {
    return &this->modified;
    }

  /**
   * Give a reference to the new_traffic variable.
   * @return This reference may be used to manipulate the implementations 
   *         new_traffic variable;
   */
  virtual Validity* NewTraffic()
    {
    return &this->new_traffic;
    }

  /**
   * Give a reference to the new_traffic variable.
   * @return This reference may be used to read the implementations 
   *         new_traffic variable;
   */
  virtual const Validity* NewTraffic() const
    {
    return &this->new_traffic;
    }

  /**
   * Give a reference to the list variable.
   * @return This reference may be used to manipulate the implementations 
   *         List variable;
   */
  virtual TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List()
    {
    return &this->list;
    }

  /**
   * Give a reference to the list variable.
   * @return This reference may be used to read the implementations 
   *         List variable;
   */
  virtual const TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List() const
    {
    return &this->list;
    }

private:
  size_t FindIndex(const TargetPtr target) const
    {
    size_t i;

    // We don't need no fancy search algorithm!
    for (i = 0; i < this->list.size(); i++)
      if (list[i] == target)
        return i;
    return -1;
    }
  };

/**
 *\} 
 */
//static_assert(std::is_trivial<TargetList>::value, "type is not trivial");

