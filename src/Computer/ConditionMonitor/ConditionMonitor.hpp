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

#include "time/Stamp.hpp"

struct NMEAInfo;
struct DerivedInfo;
struct ComputerSettings;

/**
 * Base class for system to monitor changes in state and issue
 * warnings or informational messages based on various conditions.
 */
class ConditionMonitor
{
protected:
  TimeStamp LastTime_Notification = TimeStamp::Undefined();
  TimeStamp LastTime_Check = TimeStamp::Undefined();
  FloatDuration Interval_Notification;
  const FloatDuration Interval_Check;

public:
  constexpr ConditionMonitor(FloatDuration _interval_notification,
                             FloatDuration _interval_check) noexcept
    :Interval_Notification(_interval_notification),
     Interval_Check(_interval_check) {}

  void Update(const NMEAInfo &basic, const DerivedInfo &calculated,
              const ComputerSettings &settings) noexcept;

private:
  virtual bool CheckCondition(const NMEAInfo &basic,
                              const DerivedInfo &calculated,
                              const ComputerSettings &settings) noexcept = 0;
  virtual void Notify() noexcept = 0;
  virtual void SaveLast() noexcept = 0;

  bool Ready_Time_Notification(TimeStamp t) const noexcept;
  bool Ready_Time_Check(TimeStamp t, bool *restart) noexcept;
};
