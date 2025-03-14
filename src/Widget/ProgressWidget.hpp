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

#include "WindowWidget.hpp"
#include "Operation/MessageOperationEnvironment.hpp"
#include "util/tstring.hpp"

class PluggableOperationEnvironment;

/**
 * A widget which displays a progress bar.
 */
class ProgressWidget final : public WindowWidget, MessageOperationEnvironment {
  class ProgressBar;

  PluggableOperationEnvironment &env;

  tstring text;

public:
  explicit ProgressWidget(PluggableOperationEnvironment &_env,
                          const TCHAR *_text) noexcept
    :env(_env), text(_text) {}

  /* virtual methods from class Widget */
  PixelSize GetMinimumSize() const noexcept override;
  PixelSize GetMaximumSize() const noexcept override;
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  void Unprepare() noexcept override;

private:
  /* virtual methods from class OperationEnvironment */
  void SetText(const TCHAR *text) noexcept override;
  void SetProgressRange(unsigned range) noexcept override;
  void SetProgressPosition(unsigned position) noexcept override;
};
