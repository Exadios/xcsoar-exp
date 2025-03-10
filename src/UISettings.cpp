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

#include "UISettings.hpp"

void
UISettings::SetDefaults()
{
  display.SetDefaults();

  menu_timeout = std::chrono::seconds{8 * 4};

  scale = 100;

  custom_dpi = 0;  // automatic

  thermal_assistant_position = ThermalAssistantPosition::BOTTOM_LEFT;

  enable_airspace_warning_dialog = true;

  popup_message_position = PopupMessagePosition::CENTER;

  haptic_feedback = HapticFeedback::DEFAULT;

  show_menu_button = true;

  format.SetDefaults();
  map.SetDefaults();
  info_boxes.SetDefaults();
  vario.SetDefaults();
  traffic.SetDefaults();
  pages.SetDefaults();
  dialog.SetDefaults();
  sound.SetDefaults();
}
