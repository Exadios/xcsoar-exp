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

#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/TargetId.hpp"
#include "NMEA/Validity.hpp"
#include "Surveillance/TrivialArrayExtender.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"

#include <type_traits>
#include <list>
#include <memory>

#ifndef NDEBUG
#include <stdio.h>
#include "thread/Thread.hpp"
#include <iostream>
#endif

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

/**
 * This class keeps track of the target objects received from a
 * remote sensing device.
 */
struct TargetList 
  {
public:
  static constexpr size_t MAX_COUNT = 50;

  /**
   * Ctor
   */
  TargetList()
    {
    this->Clear();
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
  TrivialArrayExtender<FlarmTarget, MAX_COUNT> flarm_list;
  TrivialArrayExtender<AdsbTarget,  MAX_COUNT> adsb_list;

  /**
   * Clear everything!
   */
  constexpr void Clear()
    {
    this->modified.Clear();
    this->new_traffic.Clear();
    this->flarm_list.clear();
    this->adsb_list.clear();
    }

  /**
   * Test whether this list is empty.
   * @return If empty then true.
   */
  constexpr bool IsEmpty() const
    {
    return ((this->flarm_list.empty() == true) && (this->adsb_list.empty()))
             ? true : false;
    }

  /**
   * Adds data from the specified object, unless already present in
   * this one.
   * @param add Source list.
   */

  constexpr void Complement(const TargetList& add) 
    {
    if (this->IsEmpty() && !add.IsEmpty())
      *this = add;  // Just initialize this list.
    this->modified.Complement(add.modified);  // Don't know where this is done
                                              // for the FLARM merge.
    // Add unique FLARM traffic from 'add' list
    for (auto &traffic : add.flarm_list)
      {
      if (this->FindFlarmTraffic(traffic.id) == nullptr)
        if (this->AddFlarmTarget(traffic) == nullptr)
          break;
      }
    // Add unique ADSB traffic from 'add' list.
    for (auto &traffic : add.adsb_list)
      {
      if ( this->FindAdsbTraffic(traffic.id) == nullptr)
        {
        if (this->AddAdsbTarget(traffic) == nullptr)
          break;
        }
      }
    }

  /**
   * Delete stale targets.
   * @param clock Time now.
   */
  constexpr void Expire(TimeStamp clock) noexcept
    {
    this->modified.Expire(clock, std::chrono::minutes(5));
    this->new_traffic.Expire(clock, std::chrono::minutes(1));

    // Do Flarm first ...
    for (unsigned i = this->flarm_list.size(); i-- > 0;)
      if (!this->flarm_list[i].Refresh(clock))
        this->flarm_list.quick_remove(i);

    // ... and then ADSB
    for (unsigned i = this->adsb_list.size(); i-- > 0;)
      if (!this->adsb_list[i].Refresh(clock))
        this->adsb_list.quick_remove(i);
    }

  /**
   * Give the number of Flarm items in this list.
   * @return Number of Flarm targets in the list.
   */
  constexpr unsigned GetActiveFlarmTrafficCount() const
    {
    return this->flarm_list.size();
    }

  /**
   * Give the number of Adsb items in this list.
   * @return Number of Adsb targets in the list.
   */
  constexpr unsigned GetActiveAdsbTrafficCount() const
    {
    return this->adsb_list.size();
    }

  /**
   * Give the total number of items in this list.
   * @return Number of targets in the list.
   */
  constexpr unsigned GetActiveTrafficCount() const
    {
    return this->GetActiveFlarmTrafficCount() +
           this->GetActiveAdsbTrafficCount();
    }

  /**
   * Looks up a Flarm item in the traffic list.
   *
   * @param id Target id
   * @return the \ref FlarmTarget pointer, nullptr if not found
   */
  constexpr FlarmTarget* FindFlarmTraffic(TargetId id) noexcept
    {
    for (auto& traffic : this->flarm_list)
      if (traffic.id == id)
        return &traffic;

    return nullptr;
    }

  /**
   * Looks up a Flarm item in the traffic list.
   *
   * @param id Target id
   * @return the \ref FlarmTarget pointer, nullptr if not found
   */
  constexpr const FlarmTarget* FindFlarmTraffic(TargetId id) const
    {
    for (const auto& traffic : this->flarm_list)
      if (traffic.id == id)
        return &traffic;
    return nullptr;
    }

  /**
   * Looks up a Flarm item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref FlarmTarget pointer, nullptr if not found
   */
  constexpr FlarmTarget* FindFlarmTraffic(const TCHAR* name)
    {
    for (auto& traffic : this->flarm_list)
      if (traffic.name.equals(name))
        return &traffic;
    return nullptr;
    }

  /**
   * Looks up a Flarm item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref FlarmTarget pointer, nullptr if not found
   */
  constexpr const FlarmTarget* FindFlarmTraffic(const TCHAR* name) const
    {
    for (const auto& traffic : this->flarm_list)
      if (traffic.name.equals(name))
        return &traffic;
    return nullptr;
    }

  /**
   * Looks up a Adsb item in the traffic list.
   *
   * @param id Target id
   * @return the \ref AdsbTarget pointer, nullptr if not found
   */
  constexpr AdsbTarget* FindAdsbTraffic(TargetId id) 
    {
    for (auto& traffic : this->adsb_list)
      if (traffic.id == id)
        return &traffic;

    return nullptr;
    }

  /**
   * Looks up a Adsb item in the traffic list.
   *
   * @param id Target id
   * @return the \ref AdsbTarget pointer, nullptr if not found
   */
  constexpr const AdsbTarget* FindAdsbTraffic(TargetId id) const
    {
    for (const auto& traffic : this->adsb_list)
      if (traffic.id == id)
        return &traffic;
    return nullptr;
    }

  /**
   * Looks up a Adsb item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, nullptr if not found
   */
  const AdsbTarget* FindAdsbTraffic(const TCHAR* name)
    {
    for (auto& traffic : this->adsb_list)
      if (traffic.name.equals(name))
        return &traffic;
    return nullptr;
    }

  /**
   * Looks up a Adsb item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, nullptr if not found
   */
  constexpr const AdsbTarget* FindAdsbTraffic(const TCHAR* name) const
    {
    for (const auto& traffic : this->adsb_list)
      if (traffic.name.equals(name))
        return &traffic;
    return nullptr;
    }

  /**
   * Allocates a new \ref FlarmTarget object on the list.
   * @return If an object can be allocated on the this then a 
   *         \ref FlarmTarget pointer to that element. Otherwise
   *         nullptr.
   */
  constexpr FlarmTarget* AllocateFlarmTraffic()
    {
    if (!this->flarm_list.full())
      return &flarm_list.append();
    else
      return nullptr;
    }

  /**
   * Allocates a new \ref AdsbTarget object on the list.
   * @return If an object can be allocated on the this then a 
   *         \ref AdsbTarget pointer to that element. Otherwise
   *         nullptr.
   */
  constexpr AdsbTarget* AllocateAdsbTraffic()
    {
    if (!this->adsb_list.full())
      return &adsb_list.append();
    else
      return nullptr;
    }

  /**
   * Inserts a new \ref FlarmTarget object to the list.
   * @param The target.
   * @return A pointer to the new element that has been added or nullptr
   *         if the list is full.
   */
  constexpr FlarmTarget* AddFlarmTarget(const FlarmTarget& target)
    {
    FlarmTarget* ptr = this->AllocateFlarmTraffic();
    if (ptr == nullptr)
      return ptr;

    *ptr = target;
    return ptr;
    }

  /**
   * Inserts a new \ref FlarmTarget object to the list.
   * @param The target.
   * @return A pointer to the new element that has been added or nullptr
   *         if the list is full.
   */
  constexpr AdsbTarget* AddAdsbTarget(const AdsbTarget& target)
    {
    AdsbTarget* ptr = this->AllocateAdsbTraffic();
    if (ptr == nullptr)
      return ptr;

    *ptr = target;
    return ptr;
    }

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or nullptr if is already at begin().
   */
  constexpr const FlarmTarget* PreviousFlarmTraffic(const FlarmTarget* target) const
    {
    return target > this->flarm_list.begin()
      ? target - 1
      : NULL;
    }

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  constexpr const FlarmTarget* NextFlarmTraffic(const FlarmTarget* target) const
    {
    return target + 1 < this->flarm_list.end()
      ? target + 1
      : nullptr;
    }

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  constexpr const FlarmTarget* FirstFlarmTraffic() const
    {
    return this->flarm_list.empty() ?
           nullptr :
           (this->flarm_list.begin());
    }

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  constexpr const FlarmTarget* LastFlarmTraffic() const
    {
    return this->flarm_list.empty() ?
           nullptr :
           (this->flarm_list.end() - 1);
    }

  /**
   * Finds the most critical alert.
   * @return Pointer to the maximum alert, nullptr if no alert.
   *
   * \bug
   * What stops the object pointed to from disappearing because of, for
   * instance, it exipres.
   */
  const FlarmTarget* FindMaximumAlert() const;

  /**
   * Convert a pointer to a element in the ordered list into an index into
   * the ordered list.
   * @param t The pointer to the element
   * @return The index corresponding to t.
   */
  constexpr unsigned FlarmTrafficIndex(const FlarmTarget* t) const
    {
    return t - this->flarm_list.begin();
    }

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to manipulate the implementations 
   *         modified variable;
   */
  constexpr Validity* Modified()
    {
    return &this->modified;
    }

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to read the implementations 
   *         modified variable;
   */
  constexpr const Validity* Modified() const
    {
    return &this->modified;
    }

  /**
   * Give a reference to the new_traffic variable.
   * @return This reference may be used to manipulate the implementations 
   *         new_traffic variable;
   */
  constexpr Validity* NewTraffic()
    {
    return &this->new_traffic;
    }

  /**
   * Give a reference to the new_traffic variable.
   * @return This reference may be used to read the implementations 
   *         new_traffic variable;
   */
  constexpr const Validity* NewTraffic() const
    {
    return &this->new_traffic;
    }

  /**
   * Give a reference to the Flarm list variable.
   * @return This reference may be used to manipulate the implementations 
   *         \ref flarm_list variable;
   */
  constexpr TrivialArrayExtender<FlarmTarget, MAX_COUNT >* FlarmList()
    {
    return &this->flarm_list;
    }

  /**
   * Give a reference to the ADSB list variable.
   * @return This reference may be used to manipulate the implementations 
   *         \ref adsb_list variable;
   */
  constexpr TrivialArrayExtender<AdsbTarget, MAX_COUNT >* AdsbList()
    {
    return &this->adsb_list;
    }

  /**
   * Give a reference to the Flarm list variable.
   * @return This reference may be used to read the implementations 
   *         \ref flarm_list variable;
   */
  constexpr const TrivialArrayExtender<FlarmTarget, MAX_COUNT>* FlarmList() const
    {
    return &this->flarm_list;
    }

  /**
   * Give a reference to the ADSB list variable.
   * @return This reference may be used to read the implementations 
   *         \ref adsb_list variable;
   */
  constexpr const TrivialArrayExtender<AdsbTarget, MAX_COUNT>* AdsbList() const
    {
    return &this->adsb_list;
    }

private:

  };

/**
 *\} 
 */
//static_assert(std::is_trivial<TargetList>::value, "type is not trivial");

