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
#include "Surveillance/TrivialArrayExtender.hpp"
#include "NMEA/Validity.hpp"

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

class TargetList;

/**
 * This is the base of the TargetListBase and TargetListBaseDecorator family.
 */
struct TargetListBase
  {
public:
  static constexpr size_t MAX_COUNT = 50;

  /**
   * The purpose of this is to indicate what type of data for processing.
   *
   * \todo This is ugly. Find a more elegant way to do this!
   */
  enum TargetType
    {
    MIXED,   // Target is unknown
    FLARM,   // Target is FLARM
    ADSB,    // Target is ADSB
    };

  /**
   * Ctor.
   */
  TargetListBase();

  /**
   * Copy ctor.
   * @param rhs The source object.
   */
  TargetListBase(const TargetListBase& rhs);

  /**
   * Virtual dtor for derived classes.
   */
  virtual ~TargetListBase()
    {
    }

  /**
   * Copy operator
   * @param rhs The source object.
   * @return This object.
   */
//  virtual TargetListBase& operator=(const TargetListBase& rhs) = 0;

  /**
   * Clear everything!
   */
  virtual void Clear() = 0;

  /**
   * Test whether this list is empty.
   */
  virtual bool IsEmpty() const = 0;

  /**
   * Adds data from the specified object, unless already present in this one.
   * @param add The object to add from.
   */
  #ifdef NDEBUG
  virtual void Complement(const TargetList& add);
  #else
  [[noreturn]] virtual void Complement(const TargetList& add);
  #endif

  /**
   * Delete stale targets.
   * @param clock Now.
   */
  virtual void Expire(TimeStamp clock) noexcept = 0;

  /**
   * Give the number of items in this list.
   * @return The number of elements in the list (whether they're valid or not).
   */
  virtual unsigned GetActiveTrafficCount() const = 0;

  /**
   * Looks up an item in the traffic list.
   * @param id The unique traffic id required.
   * @return The \ref RemoteTarget pointer, NULL if not found.
   */
  #ifdef NDEBUG
  virtual FlarmTarget* FindTraffic(TargetId id);
  #else
  [[noreturn]] virtual FlarmTarget* FindTraffic(TargetId id);
  #endif

  /**
   * Looks up an item in the traffic list.
   * @param id The unique traffic id required.
   * @return The \ref RemoteTarget pointer, NULL if not found.
   */
  #ifdef NDEBUG
  virtual const FlarmTarget* FindTraffic(TargetId id) const;
  #else
  [[noreturn]] virtual const FlarmTarget* FindTraffic(TargetId id) const;
  #endif

  /**
   * Looks up an item in the traffic list.
   * @param name The name of the traffid item required.
   * @return The \ref RemoteTarget pointer, NULL if not found. If more than
   *         instance of a named will be returned. Which one, who knows?
   */
  #ifdef NDEBUG
  virtual FlarmTarget* FindTraffic(const TCHAR* name);
  #else
  [[noreturn]] virtual FlarmTarget* FindTraffic(const TCHAR* name);
  #endif
   
  /**
   * Looks up an item in the traffic list.
   * @param name The name of the traffid item required.
   * @return The \ref RemoteTarget pointer, NULL if not found. If more than
   *         instance of a named will be returned. Which one, who knows?
   */
  #ifdef NDEBUG
  virtual const FlarmTarget* FindTraffic(const TCHAR* name) const;
  #else
  [[noreturn]] virtual const FlarmTarget* FindTraffic(const TCHAR* name) const;
  #endif

  /**
   * Allocates a new blank target object on the list.
   * @return If an object can be allocated on the this then a 
             \ref RemoteTarget pointer to that element. Otherwise nullptr. 
   */
  #ifdef NDEBUG
  virtual TargetPtr AllocateTraffic();
  #else
  [[noreturn]] virtual TargetPtr AllocateTraffic();
  #endif

  /**
   * Inserts a new \ref RemoteTarget object to the list.
   * @param target The target.
   * @return A pointer to the new element that has been added or nullptri
   *         if the list is full.
   */
  #ifdef NDEBUG
  virtual TargetPtr AddTarget(RemoteTarget& target);
  #else
  [[noreturn]] virtual TargetPtr AddTarget(RemoteTarget& target);
  #endif

  /**
   * Reference the previous traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry previous to t or null if is already at begin().
   */
  virtual const TargetPtr PreviousTraffic(size_t i) const = 0;

  /**
   * Reference the previous traffic in the ordered list.
   * @param target The reference target.
   * @return The entry previous to target or null if is already at begin().
   */
  virtual const TargetPtr PreviousTraffic(const TargetPtr target) const = 0;

  /**
   * Reference the next traffic in the ordered list.
   * @param i The reference entry.
   * @return The entry after i or nullptr if is already at end() - 1.
   */
  virtual const TargetPtr NextTraffic(size_t i) const = 0;

  /**
   * Reference the next traffic in the ordered list.
   * @param target The reference entry.
   * @return The entry after target or nullptr if is already at end() - 1.
   */
  virtual const TargetPtr NextTraffic(const TargetPtr target) const = 0;

  /**
   * Reference the first traffic in the ordered list.
   * @return The first target pointer or nullptr if the list is empty.
   */
  virtual const TargetPtr FirstTraffic() const = 0;

  /**
   * Reference the last traffic in the ordered list.
   * @return The last target pointer or nullptr if the list is empty.
   */
  virtual const TargetPtr LastTraffic() const = 0;

  /**
   * Finds the most critical alert.  Returns NULL if there is no
   * alert.
   */
  virtual const TargetPtr FindMaximumAlert() const = 0;

  /**
   * Convert a pointer to a element in the ordered list into an index into
   * the ordered list.
   * @param t The pointer to the element
   * @return The index corresponding to t.
   */
  virtual unsigned TrafficIndex(const TargetPtr* t) const = 0;

  TargetListBase::TargetType type;

  /**
   * Give a reference to the modified variable.
   * @return This reference may be used to manipulate the implementations 
   *         modified variable;
   */
  virtual Validity* Modified();
  virtual const Validity* Modified() const;

  /**
   * Give a reference to the new_traffic variable.
   * @return This reference may be used to manipulate the implementations 
   *         new_traffic variable;
   */
  virtual Validity* NewTraffic();
  virtual const Validity* NewTraffic() const;

  /**
   * Give a reference to the list variable.
   * @return This reference may be used to manipulate the implementations 
   *         List variable;
   */
#ifdef NDEBUG
  virtual TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List();
  virtual const TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List() const;
#else
  [[noreturn]]virtual TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List();
 [[noreturn]]virtual const TrivialArrayExtender< std::shared_ptr< RemoteTarget >, MAX_COUNT >* List() const;
#endif

  };

/**
 * /}
 */
