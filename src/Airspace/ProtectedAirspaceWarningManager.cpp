/* Copyright_License {

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

#include "Airspace/ProtectedAirspaceWarningManager.hpp"
#include "Airspace/AirspaceWarningManager.hpp"

const FlatProjection &
ProtectedAirspaceWarningManager::GetProjection() const noexcept
{
  /* access to FlatProjection does not need to be protected */
  UnprotectedLease lease(const_cast<ProtectedAirspaceWarningManager &>(*this));
  return lease->GetProjection();
}

void
ProtectedAirspaceWarningManager::Clear() noexcept
{
  ExclusiveLease lease(*this);
  lease->clear();
}

void
ProtectedAirspaceWarningManager::AcknowledgeAll() noexcept
{
  ExclusiveLease lease(*this);
  lease->AcknowledgeAll();
}

bool
ProtectedAirspaceWarningManager::IsEmpty() const noexcept
{
  Lease lease(*this);
  return lease->empty();
}

bool
ProtectedAirspaceWarningManager::GetAckDay(const AbstractAirspace &airspace) const noexcept
{
  Lease lease(*this);
  return lease->GetAckDay(airspace);
}

void
ProtectedAirspaceWarningManager::AcknowledgeDay(ConstAirspacePtr airspace,
                                                const bool set) noexcept
{
  ExclusiveLease lease(*this);
  lease->AcknowledgeDay(std::move(airspace), set);
}

void
ProtectedAirspaceWarningManager::AcknowledgeWarning(ConstAirspacePtr airspace,
                                                    const bool set) noexcept
{
  ExclusiveLease lease(*this);
  lease->AcknowledgeWarning(std::move(airspace), set);
}

void
ProtectedAirspaceWarningManager::AcknowledgeInside(ConstAirspacePtr airspace,
                                                   const bool set) noexcept
{
  ExclusiveLease lease(*this);
  lease->AcknowledgeInside(std::move(airspace), set);
}

void
ProtectedAirspaceWarningManager::Acknowledge(ConstAirspacePtr airspace) noexcept
{
  ExclusiveLease lease(*this);
  lease->Acknowledge(std::move(airspace));
}

std::optional<AirspaceWarning>
ProtectedAirspaceWarningManager::GetTopWarning() const noexcept
{
  const Lease lease(*this);
  if (auto i = lease->begin(); i != lease->end())
    return *i;

  return std::nullopt;
}
