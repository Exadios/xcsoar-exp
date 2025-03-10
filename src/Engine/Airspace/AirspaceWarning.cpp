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

#include "AirspaceWarning.hpp"
#include "util/Compiler.h"

#include <algorithm>

void
AirspaceWarning::SaveState() noexcept
{
  state_last = state;
  state = WARNING_CLEAR;
  expired_last = expired;
}

void
AirspaceWarning::UpdateSolution(const State _state,
                                const AirspaceInterceptSolution &_solution) noexcept
{
  if (IsStateAccepted(_state)) {
    state = _state;
    solution = _solution;
  }
}


bool
AirspaceWarning::WarningLive(const Duration ack_time,
                             const Duration dt) noexcept
{
  // propagate settings from manager
  if (acktime_warning == null_acktime)
    acktime_warning = ack_time;

  if (acktime_inside == null_acktime)
    acktime_inside = ack_time;

  if ((state != WARNING_CLEAR)
      && (state < state_last)
      && (state_last == WARNING_INSIDE))
    // if inside was acknowledged, consider warning to be acknowledged
    acktime_warning = std::max(acktime_warning, acktime_inside);

  if (acktime_warning > dt)
    acktime_warning-= dt;
  else
    acktime_warning = {};

  if (acktime_inside > dt)
    acktime_inside-= dt;
  else
    acktime_inside = {};

  if (debounce_time > dt)
    debounce_time-= dt;
  else
    debounce_time = {};

  expired = IsAckExpired();

  if (state == WARNING_CLEAR)
    return !expired;

  return true;
}

bool
AirspaceWarning::ChangedState() const noexcept
{
  if (expired > expired_last)
    return true;

  if ((state_last == WARNING_CLEAR) && (state > WARNING_CLEAR))
    return IsAckExpired();

  if ((state_last < WARNING_INSIDE) && (state == WARNING_INSIDE))
    return IsAckExpired();

  return false;
}

bool
AirspaceWarning::IsAckExpired() const noexcept
{
  if (ack_day)
    // these ones persist
    return false;

  switch (state) {
  case WARNING_CLEAR:
    /* no warning at all, assume it's still acked */
    return true;

  case WARNING_TASK:
  case WARNING_FILTER:
  case WARNING_GLIDE:
    return acktime_warning.count() <= 0;

  case WARNING_INSIDE:
    return acktime_inside.count() <= 0;
  };

  // unknown, should never get here
  gcc_unreachable();
  return true;
}

void
AirspaceWarning::Acknowledge() noexcept
{
  if (state == WARNING_INSIDE)
    acktime_inside = null_acktime;
  else if (state != WARNING_CLEAR)
    acktime_warning = null_acktime;
}

void
AirspaceWarning::AcknowledgeInside(const bool set) noexcept
{
  if (set)
    acktime_inside = null_acktime;
  else
    acktime_inside = {};
}

void
AirspaceWarning::AcknowledgeWarning(const bool set) noexcept
{
  if (set)
    acktime_warning = null_acktime;
  else
    acktime_warning = {};
}

bool
AirspaceWarning::operator<(const AirspaceWarning &other) const noexcept
{
  // compare bother.ack
  if (IsAckExpired() != other.IsAckExpired())
    // least expired top
    return IsAckExpired() > other.IsAckExpired();

  // compare bother.state
  if (GetWarningState() != other.GetWarningState())
    // most severe top
    return GetWarningState() > other.GetWarningState();

  // state and ack equal, compare bother.time to intersect
  return GetSolution().elapsed_time < other.GetSolution().elapsed_time;
}
