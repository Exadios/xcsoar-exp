/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
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

#include "Surveillance/Flarm/FlarmDetails.hpp"
#include "FLARM/Global.hpp"
#include "Surveillance/TrafficDatabases.hpp"
#include "FLARM/FlarmId.hpp"
#include "util/StringCompare.hxx"

#include <cassert>

#ifdef FLARMNET_ORG
//------------------------------------------------------------------------------
const FlarmNetRecord*
FlarmDetails::LookupRecord(const TargetId& id)
  {
  // try to find flarm from FlarmNet.org File
  if (::traffic_databases == nullptr)
    return NULL;

  return ::traffic_databases->flarm_net.FindRecordById(id);
  }

//------------------------------------------------------------------------------
const TCHAR*
FlarmDetails::LookupCallsign(const TargetId& id)
  {
  if (::traffic_databases == nullptr)
    return nullptr;

  return ::traffic_databases->FindNameById(id);
  }

//------------------------------------------------------------------------------
TargetId
FlarmDetails::LookupId(const TCHAR* cn)
  {
  assert(::traffic_databases != nullptr);

  return ::traffic_databases->FindIdByName(cn);

  }

//------------------------------------------------------------------------------
bool
FlarmDetails::AddSecondaryItem(const TargetId& id, const TCHAR* name)
  {
  assert(id.IsDefined());
  assert(::traffic_databases != nullptr);

  return traffic_databases->flarm_names.Set(id, name);
  }

//------------------------------------------------------------------------------
unsigned
FlarmDetails::FindIdsByCallSign(const TCHAR* cn, FlarmId array[],
                                unsigned size)
  {
  assert(cn != NULL);
  assert(!StringIsEmpty(cn));
  assert(::traffic_databases != nullptr);

  return ::traffic_databases->FindIdsByName(cn, array, size);
  }
#else
//------------------------------------------------------------------------------
const FlarmNetRecord*
FlarmDetails::LookupRecord(const TargetId& )
  {
  return NULL;
  }

//------------------------------------------------------------------------------
const TCHAR*
FlarmDetails::LookupCallsign(const TargetId& )
  {
  return nullptr;
  }

//------------------------------------------------------------------------------
TargetId
FlarmDetails::LookupId(const TCHAR* )
  {
  TargetId id;
  id.Arg(0);
  return id;
  }

//------------------------------------------------------------------------------
bool
FlarmDetails::AddSecondaryItem(const TargetId&, const TCHAR* )
  {
  return false;
  }

//------------------------------------------------------------------------------
unsigned
FlarmDetails::FindIdsByCallSign(const TCHAR*, TargetId*, unsigned )
  {
  return 0;
  }
#endif  // FLARMNET_ORG


