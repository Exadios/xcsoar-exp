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

#include "Blackboard/InterfaceBlackboard.hpp"
#include "thread/Debug.hpp"

struct UIState;
class MainWindow;

/** 
 * Class to hold data/methods accessible by all interface subsystems
 */
namespace CommonInterface {

namespace Private {

extern UIState ui_state;
extern InterfaceBlackboard blackboard;

/**
 * True if movement was detected on a real GPS.
 */
extern bool movement_detected;

} // namespace Private

// window.. make this protected TODO so have to subclass to get access
extern MainWindow *main_window;

static inline bool
MovementDetected() noexcept
{
  return Private::movement_detected;
}

// TODO: make this protected
/**
 * Returns InterfaceBlackboard.Basic (NMEA_INFO) (read-only)
 * @return InterfaceBlackboard.Basic
 */
[[gnu::const]]
static inline const MoreData &
Basic() noexcept
{
  assert(InMainThread());

  return Private::blackboard.Basic();
}

/**
 * Returns InterfaceBlackboard.Calculated (DERIVED_INFO) (read-only)
 * @return InterfaceBlackboard.Calculated
 */
[[gnu::const]]
static inline const DerivedInfo &
Calculated() noexcept
{
  assert(InMainThread());

  return Private::blackboard.Calculated();
}

[[gnu::const]]
static inline const SystemSettings &
GetSystemSettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.GetSystemSettings();
}

[[gnu::const]]
static inline SystemSettings &
SetSystemSettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.SetSystemSettings();
}

/**
 * Returns the InterfaceBlackboard.ComputerSettings (read-only)
 * @return The InterfaceBlackboard.ComputerSettings
 */
[[gnu::const]]
static inline const ComputerSettings &
GetComputerSettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.GetComputerSettings();
}

/**
 * Returns the InterfaceBlackboard.ComputerSettings (read-write)
 * @return The InterfaceBlackboard.ComputerSettings
 */
[[gnu::const]]
static inline ComputerSettings &
SetComputerSettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.SetComputerSettings();
}

[[gnu::const]]
static inline const UISettings &
GetUISettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.GetUISettings();
}

/**
 * Returns the InterfaceBlackboard.MapSettings (read-only)
 * @return The InterfaceBlackboard.MapSettings
 */
[[gnu::const]]
static inline const MapSettings &
GetMapSettings() noexcept
{
  assert(InMainThread());

  return GetUISettings().map;
}

[[gnu::const]]
static inline const FullBlackboard &
Full() noexcept
{
  assert(InMainThread());

  return Private::blackboard;
}

[[gnu::const]]
static inline LiveBlackboard &
GetLiveBlackboard() noexcept
{
  assert(InMainThread());

  return Private::blackboard;
}

[[gnu::const]]
static inline UISettings &
SetUISettings() noexcept
{
  assert(InMainThread());

  return Private::blackboard.SetUISettings();
}

[[gnu::const]]
static inline const DisplaySettings &
GetDisplaySettings() noexcept
{
  assert(InMainThread());

  return GetUISettings().display;
}

/**
 * Returns the InterfaceBlackboard.MapSettings (read-write)
 * @return The InterfaceBlackboard.MapSettings
 */
[[gnu::const]]
static inline MapSettings &
SetMapSettings() noexcept
{
  assert(InMainThread());

  return SetUISettings().map;
}

static inline const UIState &
GetUIState() noexcept
{
  assert(InMainThread());

  return Private::ui_state;
}

static inline UIState &
SetUIState() noexcept
{
  assert(InMainThread());

  return Private::ui_state;
}

static inline void
ReadBlackboardBasic(const MoreData &nmea_info) noexcept
{
  assert(InMainThread());

  Private::blackboard.ReadBlackboardBasic(nmea_info);
}

static inline void
ReadBlackboardCalculated(const DerivedInfo &derived_info) noexcept
{
  assert(InMainThread());

  Private::blackboard.ReadBlackboardCalculated(derived_info);
}

static inline void
ReadCommonStats(const CommonStats &common_stats) noexcept
{
  assert(InMainThread());

  Private::blackboard.ReadCommonStats(common_stats);
}

static inline void
AddListener(BlackboardListener &listener) noexcept
{
  assert(InMainThread());

  Private::blackboard.AddListener(listener);
}

static inline void
RemoveListener(BlackboardListener &listener) noexcept
{
  assert(InMainThread());

  Private::blackboard.RemoveListener(listener);
}

static inline void
BroadcastGPSUpdate() noexcept
{
  assert(InMainThread());

  Private::blackboard.BroadcastGPSUpdate();
}

static inline void
BroadcastCalculatedUpdate() noexcept
{
  assert(InMainThread());

  Private::blackboard.BroadcastCalculatedUpdate();
}

static inline void
BroadcastComputerSettingsUpdate() noexcept
{
  assert(InMainThread());

  Private::blackboard.BroadcastComputerSettingsUpdate();
}

static inline void
BroadcastUISettingsUpdate() noexcept
{
  assert(InMainThread());

  Private::blackboard.BroadcastUISettingsUpdate();
}

} // namespace CommonInterface
