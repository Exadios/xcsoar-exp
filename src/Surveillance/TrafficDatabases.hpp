/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2025 The XCSoar Project
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

#include "Surveillance/ColorDatabase.hpp"
#include "Surveillance/Flarm/FlarmNameDatabase.hpp"
#include "Surveillance/Flarm/FlarmNetDatabase.hpp"

struct TrafficDatabases
  {
  TargetColorDatabase target_colors;

  FlarmNameDatabase flarm_names;

  FlarmNetDatabase flarm_net;

  /**
   * FlarmId of the glider to track.  Check FlarmId::IsDefined()
   * before using this attribute.
   *
   * This is a copy of TeamCodeSettings::team_flarm_id.
   */
  TargetId team_flarm_id;

  TrafficDatabases()
    :team_flarm_id(TargetId::Undefined())
    {
    }

  [[gnu::pure]]
  TargetColor GetColor(TargetId id) const
    {
    TargetColor color = target_colors.Get(id);
    if (color == TargetColor::NONE && id == team_flarm_id)
      /* if no color found but target is teammate, use green */
      color = TargetColor::GREEN;

    return color;
    }

  [[gnu::pure]]
  const TCHAR* FindNameById(TargetId id) const;

  [[gnu::pure]] // gcc_nonnull_all
  TargetId FindIdByName(const TCHAR* name) const;

  /**
   * Look up all records with the specified name.
   *
   * @param max The maximum size of the given buffer
   * @return The number of items copied to the given buffer
   */
  // gcc_nonnull_all
  unsigned FindIdsByName(const TCHAR* name,
                         TargetId* buffer, unsigned max) const;
};

/**
 * \}
 */
