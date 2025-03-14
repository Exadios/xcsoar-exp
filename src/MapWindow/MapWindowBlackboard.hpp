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

#include "Blackboard/BaseBlackboard.hpp"
#include "Blackboard/ComputerSettingsBlackboard.hpp"
#include "Blackboard/MapSettingsBlackboard.hpp"
#include "thread/Debug.hpp"
#include "UIState.hpp"

/**
 * Blackboard used by map window: provides read-only access to local
 * copies of data required by map window
 * 
 */
class MapWindowBlackboard:
  public BaseBlackboard,
  public ComputerSettingsBlackboard,
  public MapSettingsBlackboard
{
  UIState ui_state;

protected:
  /**
   * Give a reference to the GPS data (i.e. the input data)
   * @return A const reference to the GPS data.
   */
  [[gnu::const]]
  const MoreData &Basic() const {
    assert(InDrawThread());

    return BaseBlackboard::Basic();
  }

  /**
   * Give a reference to the calculated data.
   * @return A const reference to the calculated data.
   */
  [[gnu::const]]
  const DerivedInfo &Calculated() const {
    assert(InDrawThread());

    return BaseBlackboard::Calculated();
  }

  [[gnu::const]]
  const ComputerSettings &GetComputerSettings() const {
    assert(InDrawThread());

    return ComputerSettingsBlackboard::GetComputerSettings();
  }

  [[gnu::const]]
  const MapSettings &GetMapSettings() const {
    assert(InDrawThread());

    return settings_map;
  }

  [[gnu::const]]
  const UIState &GetUIState() const {
    assert(InDrawThread());

    return ui_state;
  }

  /**
   * Set internal variables.
   * @param nmea_info Input data from a device. Sets gps_data.
   * @param derived_info Calculated data. Sets calculated_info.
   */
  void ReadBlackboard(const MoreData &nmea_info,
                      const DerivedInfo &derived_info);
  void ReadComputerSettings(const ComputerSettings &settings);
  void ReadMapSettings(const MapSettings &settings);

  void ReadUIState(const UIState &new_value) {
    ui_state = new_value;
  }
};
