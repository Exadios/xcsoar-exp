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

#include "Surveillance/TrafficDatabases.hpp"
#include "Surveillance/Flarm/FlarmNetRecord.hpp"
#include "util/StringCompare.hxx"

//------------------------------------------------------------------------------
const TCHAR* 
TrafficDatabases::FindNameById(TargetId id) const
  {
  // try to find flarm from userFile
  const TCHAR* name = this->flarm_names.Get(id);
  if (name != nullptr)
    return name;

  // try to find flarm from FlarmNet.org File
  const FlarmNetRecord *record = this->flarm_net.FindRecordById(id);
  if (record != nullptr)
    return record->callsign;

  return nullptr;
  }

//------------------------------------------------------------------------------
TargetId
TrafficDatabases::FindIdByName(const TCHAR* name) const
  {
  assert(!StringIsEmpty(name));

  // try to find flarm from userFile
  const TargetId id = this->flarm_names.Get(name);
  if (id.IsDefined())
    return id;

  // try to find flarm from FlarmNet.org File
  const FlarmNetRecord* record = this->flarm_net.FindFirstRecordByCallSign(name);
  if (record != NULL)
    return record->GetId();

  return TargetId::Undefined();
  }

//------------------------------------------------------------------------------
unsigned
TrafficDatabases::FindIdsByName(const TCHAR* name,
                                TargetId* buffer, unsigned max) const
  {
  assert(!StringIsEmpty(name));

  unsigned n = this->flarm_names.Get(name, buffer, max);
  if (n < max)
    n += this->flarm_net.FindIdsByCallSign(name, buffer + n, max - n);

  return n;
  }
