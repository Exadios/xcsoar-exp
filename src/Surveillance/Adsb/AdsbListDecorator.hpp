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

#include "Surveillance/TargetListDecorator.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"

/**
 * A decorator used to manage a \ref TargetList as a FlarmList.
 */
struct AdsbListDecorator : public TargetListDecorator
  {
  /**
   * Disallow default ctor.
   */
  AdsbListDecorator() = delete;

  /**
   * Ctor
   * @param target_list The \ref TargetList to decorate.
   */
  AdsbListDecorator(TargetList& target_list)
    : TargetListDecorator(target_list)
    {
    }

  /**
   * Looks up an item in the traffic list.
   * @param id Target id
   * @return The \ref AdsbTarget pointer, NULL if not found at all or,
   *         if found, is not an \ref AdsbTarget.
   */
  AdsbPtr FindTraffic(TargetId id)
    {
    return AdsbListDecorator::AdsbCast(this->target_list.FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return The \ref AdsbTarget pointer, NULL if not found at all or,
   *         if found, is not an \ref AdsbTarget.
   */
  const AdsbPtr FindTraffic(TargetId id) const
    {
    return AdsbListDecorator::AdsbCast(this->target_list.FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, NULL if not found at all or,
   *         if found, is not an \ref AdsbTarget.
   */
  AdsbPtr FindTraffic(const TCHAR* name)
    {
    return AdsbListDecorator::AdsbCast(this->target_list.FindTraffic(name));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, NULL if not found at all or,
   *         if found, is not an \ref AdsbTarget.
   */
  const AdsbPtr FindTraffic(const TCHAR *name) const
    {
    return AdsbListDecorator::AdsbCast(this->target_list.FindTraffic(name));
    }

  /**
   * Allocates a new target object on the list.
   * @return If an object can be allocated on the this then a 
   *         \ref RemoteTarget pointer to that element. Otherwise
   *         nullptr.
   */
  AdsbPtr AllocateTraffic()
    {
    if (!this->target_list.list.full())
      {
      AdsbPtr ptr = std::make_shared<AdsbTarget>(*new AdsbTarget);
      this->target_list.list.append(ptr);
      return ptr;
      }
    return std::shared_ptr<AdsbTarget>(nullptr);
    }

  /**
   * Inserts a new \ref AdsbTarget object to the list.
   * @param The target.
   * @return A pointer to the new element that has been added or nullptr
   *         if the list is full.
   */
  AdsbPtr AddTarget(AdsbTarget& target)
    {
    if (this->target_list.list.full())
      return std::shared_ptr<AdsbTarget>(nullptr);   // Too bad.

    AdsbPtr ptr = std::make_shared<AdsbTarget>(target);
#ifndef NDEBUG
    printf("%s, %d: %p\n", __FILE__, __LINE__, ptr.get());
    printf("%s, %d: %d\n", __FILE__, __LINE__, target.DebugType());
    printf("%s, %d: %d\n", __FILE__, __LINE__, ptr->DebugType());
#endif
    this->target_list.list.append(ptr);
    return ptr;
    }
  /**
   * Cast a \ref TargetPtr to a \ref AdsbPtr.
   * @param t The \ref TargetPtr to cast.
   * @return If the argument can be cast to a \ref AdsbPtr (because it really
   *         is one) then return it, otherwise return nullptr.
   */
  static AdsbPtr AdsbCast(TargetPtr t)
    {
    return std::dynamic_pointer_cast<AdsbTarget>(t);
    }
  };

/**
 * \}
 */
