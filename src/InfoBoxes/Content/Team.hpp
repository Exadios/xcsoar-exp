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
 */

#include "InfoBoxes/Content/Base.hpp"

/**
 * Presently this functionality is not implemented due to ADS-B not being
 * supported.
 */
/** \todo Investigate the utility of Flarm Traffic Details (Users Manual,
 *        section 8.6) and make a decision of whether to implement it with
 *        the addition of ADS-B data. (Issue 5) (COMPILE_TEAM_CODE).
 */

class InfoBoxContentTeamCode : public InfoBoxContent
{
public:
  void Update(InfoBoxData &data) noexcept override;
  bool HandleKey(const InfoBoxKeyCodes keycode) noexcept override;
  const InfoBoxPanel *GetDialogContent() noexcept override;
};

void
UpdateInfoBoxTeamBearing(InfoBoxData &data) noexcept;

void
UpdateInfoBoxTeamBearingDiff(InfoBoxData &data) noexcept;

void
UpdateInfoBoxTeamDistance(InfoBoxData &data) noexcept;
