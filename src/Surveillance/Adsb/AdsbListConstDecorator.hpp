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
  AdsbListConstDecorator(const TargetList* target_list)
    : TargetListConstDecorator(target_list)
    {
    }

  /**
   * Looks up an item in the traffic list.
   * @param id Target id
   * @return the \ref AdsbTarget pointer, NULL if not found
   */
  AdsbPtr FindAdsb(TargetId id)
    {
    return AdsbListConstDecorator::AdsbCast(this->target_list->FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id Target id
   * @return the \ref AdsbTarget pointer, NULL if not found
   */
  const TargetPtr FindAdsb(TargetId id) const
    {
    return AdsbListConstDecorator::AdsbCast(this->target_list->FindTraffic(id));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, NULL if not found
   */
  AdsbPtr FindAdsb(const TCHAR *name)
    {
    return AdsbListConstDecorator::AdsbCast(this->target_list->FindTraffic(name));
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name The name or call sign
   * @return The \ref AdsbTarget pointer, NULL if not found
   */
  const AdsbPtr FindAdsb(const TCHAR *name) const
    {
    return AdsbListConstDecorator::AdsbCast(this->target_list->FindTraffic(name));
    }

  /**
   * Returns a \ref TargetList of ADSB targets.
   * @return The list of ADSB targets.
   */
  TargetList AdsbTargets() const;

private:
  static const AdsbPtr AdsbCast(const TargetPtr t)
    {
    return std::dynamic_pointer_cast<AdsbTarget>(t);
    }
  };


/**
 * \}
 */
