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

#include "Surveillance/TargetId.hpp"
#include "Surveillance/Color.hpp"

#include <map>
#include <cassert>

class TargetColorDatabase 
  {
  typedef std::map<TargetId, TargetColor> Map;
  Map data;

public:
  typedef Map::const_iterator const_iterator;

  [[gnu::pure]]
  const_iterator begin() const
    {
    return data.begin();
    }

  [[gnu::pure]]
  const_iterator end() const
    {
    return data.end();
    }

  [[gnu::pure]]
  TargetColor Get(TargetId id) const
    {
    auto i = data.find(id);
    if (i == data.end())
      return TargetColor::NONE;

    return i->second;
    }

  void Set(TargetId id, TargetColor color)
    {
    assert(color != TargetColor::NONE);
    assert(color != TargetColor::COUNT);

    auto i = data.insert(std::make_pair(id, color));
    if (!i.second)
      /* the id already exists in the map: overwrite the old color */
      i.first->second = color;
    }

  void Remove(TargetId id)
    {
    data.erase(id);
    }
  };

/**
 * \}
 */
