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

#include "Surveillance/TargetListBase.hpp"

//------------------------------------------------------------------------------
TargetListBase:: TargetListBase()
  {
  }

//------------------------------------------------------------------------------
TargetListBase::TargetListBase(const TargetListBase&)
  {
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
void
TargetListBase::Complement(const TargetList& )
  {
  assert(false);  // NOP here - abort on debug.
  }

//------------------------------------------------------------------------------
Validity*
TargetListBase::Modified()
  {
  return (Validity*)nullptr;  // NOP here.
  }

//------------------------------------------------------------------------------
const Validity*
TargetListBase::Modified() const
  {
  return (Validity*)nullptr;  // NOP here.
  }

//------------------------------------------------------------------------------
Validity*
TargetListBase::NewTraffic()
  {
  return (Validity*)nullptr;  // NOP here.
  }

//------------------------------------------------------------------------------
const Validity*
TargetListBase::NewTraffic() const
  {
  return (Validity*)nullptr;  // NOP here.
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
TrivialArrayExtender< std::shared_ptr< RemoteTarget >, TargetListBase::MAX_COUNT >*
TargetListBase::List()
  {
  assert(false); 
#ifdef NDEBUG
  return (TrivialArrayExtender< std::shared_ptr< RemoteTarget >, TargetListBase::MAX_COUNT >*)nullptr;   // NOP here.
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
const TrivialArrayExtender< std::shared_ptr< RemoteTarget >, TargetListBase::MAX_COUNT >*
TargetListBase::List() const
  {
  assert(false);
#ifdef NDEBUG
  return (TrivialArrayExtender< std::shared_ptr< RemoteTarget >, TargetListBase::MAX_COUNT >*)nullptr;   // NOP here.
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
TargetPtr
TargetListBase::FindTraffic(TargetId)
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
const TargetPtr
TargetListBase::FindTraffic(TargetId) const
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
TargetPtr
TargetListBase::FindTraffic(const TCHAR*)
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
const TargetPtr
TargetListBase::FindTraffic(const TCHAR*) const
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
TargetPtr
TargetListBase::AllocateTraffic()
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }

//------------------------------------------------------------------------------
#ifndef NDEBUG
[[noreturn]]
#endif
TargetPtr
TargetListBase::AddTarget(RemoteTarget&)
  {
  assert(false);	// This function should never be called.
#ifdef NDEBUG
  return nullptr;
#endif
  }
