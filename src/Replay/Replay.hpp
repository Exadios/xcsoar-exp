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

#include "ui/event/Timer.hpp"
#include "NMEA/Info.hpp"
#include "time/PeriodClock.hpp"
#include "time/Stamp.hpp"
#include "system/Path.hpp"

class Logger;
class ProtectedTaskManager;
class AbstractReplay;
class CatmullRomInterpolator;
class Error;

/**
 * \todo
 *
 * At the moment replay of NMEA files fails. The problem is that the 
 * time of the data in this thread (main) and the time of the data in the
 * Merge thread are different. The time in this thread appears to be ahead
 * of that of the Merge thread causing FLARM data to be thrown away.
 *
 * Investigate and fix. (Issue 5)
 */

class Replay final
{
  UI::Timer timer{[this]{ OnTimer(); }};

  double time_scale;

  AbstractReplay *replay;

  Logger *logger;
  ProtectedTaskManager &task_manager;

  AllocatedPath path = nullptr;

  /**
   * The time of day according to replay input.  This is negative if
   * unknown.
   */
  TimeStamp virtual_time;

  /**
   * If this value is not negative, then we're in fast-forward mode:
   * replay is going as quickly as possible.  This value denotes the
   * time stamp when we will stop going fast-forward.  If
   * #virtual_time is negative, then this is the duration, and
   * #virtual_time will be added as soon as it is known.
   */
  TimeStamp fast_forward;

  /**
   * Keeps track of the wall-clock time between two Update() calls.
   */
  PeriodClock clock;

  /**
   * The last NMEAInfo returned by the #AbstractReplay instance.  It
   * is held back until #virtual_time has passed #next_data.time.
   */
  NMEAInfo next_data;

  CatmullRomInterpolator *cli;

public:
  Replay(Logger *_logger, ProtectedTaskManager &_task_manager)
    :time_scale(1), replay(nullptr),
     logger(_logger), task_manager(_task_manager), cli(nullptr) {
  }

  ~Replay() {
    Stop();
  }

  bool IsActive() const {
    return replay != nullptr;
  }

private:
  bool Update();

public:
  void Stop();

  /**
   * Throws std::runtime_errror on error.
   */
  void Start(Path _path);

  Path GetFilename() const {
    return path;
  }

  double GetTimeScale() const {
    return time_scale;
  }

  void SetTimeScale(const double _time_scale) {
    time_scale = _time_scale;
  }

  /**
   * Start fast-forwarding the replay by the specified number of
   * seconds.  This replays the given amount of time from the input
   * time as quickly as possible.  Returns false if unable to fast forward.
   */
  bool FastForward(FloatDuration delta_s) noexcept {
    if (!IsActive())
      return false;

    if (virtual_time.IsDefined()) {
      fast_forward = virtual_time + delta_s;
      return true;
    } else {
      fast_forward = TimeStamp{delta_s};
      return false;
    }
  }

  TimeStamp GetVirtualTime() const noexcept {
    return virtual_time;
  }

private:
  void OnTimer();
};
