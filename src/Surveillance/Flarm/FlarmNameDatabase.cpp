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

#include "Surveillance/Flarm/FlarmNameDatabase.hpp"

//------------------------------------------------------------------------------
int
FlarmNameDatabase::Find(TargetId id) const noexcept
  {
  assert(id.IsDefined());

  for (unsigned i = 0, size = data.size(); i != size; ++i)
    if (data[i].id == id)
      return i;

  return -1;
  }

//------------------------------------------------------------------------------
int
FlarmNameDatabase::Find(const TCHAR *name) const noexcept
  {
  assert(name != nullptr);

  for (unsigned i = 0, size = data.size(); i != size; ++i)
    if (data[i].name.equals(name))
      return i;

  return -1;
  }

//------------------------------------------------------------------------------
const TCHAR*
FlarmNameDatabase::Get(TargetId id) const noexcept
  {
  int i = Find(id);
  if (i < 0)
    return nullptr;

  return data[i].name;
  }

//------------------------------------------------------------------------------
TargetId
FlarmNameDatabase::Get(const TCHAR* name) const noexcept
  {
  int i = Find(name);
  if (i < 0)
    return TargetId::Undefined();

  return data[i].id;
  }

//------------------------------------------------------------------------------
unsigned
FlarmNameDatabase::Get(const TCHAR* name,
                       TargetId* buffer,
                       unsigned max) const noexcept
  {
  assert(name != nullptr);
  assert(buffer != nullptr);
  assert(max > 0);

  unsigned n = 0;
  for (unsigned i = 0, size = data.size(); i != size && n != max; ++i)
    if (data[i].name.equals(name))
      buffer[n++] = data[i].id;

  return n;
  }

//------------------------------------------------------------------------------
bool
FlarmNameDatabase::Set(TargetId id, const TCHAR *name) noexcept
  {
  assert(id.IsDefined());
  assert(name != nullptr);

  int i = Find(id);
  if (i >= 0)
    {
    /* update existing record */
    data[i].name = name;
    return true;
    }
  else if (!data.full())
    {
    /* create new record */
    data.append({id, name});
    return true;
    }
  else
    /* error: database is full */
    return false;
  }
