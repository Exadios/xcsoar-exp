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

#include "FlarmProfile.hpp"
#include "Map.hpp"
#include "Surveillance/ColorDatabase.hpp"

#include <string>

#include <string.h>

static void
LoadColor(const ProfileMap& map, TargetColorDatabase& db,
          const char* key, TargetColor color)
{
  const char* ids = map.Get(key);
  if (ids == nullptr)
    return;

  const char* p = ids;
  while (true) {
    char* endptr;
    TargetId id = TargetId::Parse(p, &endptr);
    if (id.IsDefined())
      db.Set(id, color);

    p = strchr(endptr, ',');
    if (p == nullptr)
      break;

    ++p;
  }
}

void
Profile::Load(const ProfileMap& map, TargetColorDatabase& db)
{
  LoadColor(map, db, "FriendsGreen", TargetColor::GREEN);
  LoadColor(map, db, "FriendsBlue", TargetColor::BLUE);
  LoadColor(map, db, "FriendsYellow", TargetColor::YELLOW);
  LoadColor(map, db, "FriendsMagenta", TargetColor::MAGENTA);
}

void
Profile::Save(ProfileMap& map, const TargetColorDatabase& db)
{
  std::string ids[4];

  for (const auto &i : db) {
    assert(i.first.IsDefined());
    assert((int)i.second < (int)TargetColor::COUNT);

    if (i.second == TargetColor::NONE)
      continue;

    unsigned color_index = (int)i.second - 1;

    char id_buffer[16];
    ids[color_index] += i.first.Format(id_buffer);
    ids[color_index] += ',';
  }

  map.Set("FriendsGreen", ids[0].c_str());
  map.Set("FriendsBlue", ids[1].c_str());
  map.Set("FriendsYellow", ids[2].c_str());
  map.Set("FriendsMagenta", ids[3].c_str());
}
