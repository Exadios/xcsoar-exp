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

#include "Operation.hpp"

/**
 * An #OperationEnvironment implementation that forwards all calls to
 * another #OperationEnvironment instance.  Similar to
 * #ProxyOperationEnvironment, but this one's underlying instance is
 * optional.
 */
class PluggableOperationEnvironment final : public OperationEnvironment {
  OperationEnvironment *other = nullptr;

public:
  void SetOperationEnvironment(OperationEnvironment &_other) noexcept {
    other = &_other;
  }

  void SetOperationEnvironment(std::nullptr_t n) noexcept {
    other = n;
  }

  /* virtual methods from class OperationEnvironment */
  bool IsCancelled() const noexcept override;
  void SetCancelHandler(std::function<void()> handler) noexcept override;
  void Sleep(std::chrono::steady_clock::duration duration) noexcept override;
  void SetErrorMessage(const TCHAR *text) noexcept override;
  void SetText(const TCHAR *text) noexcept override;
  void SetProgressRange(unsigned range) noexcept override;
  void SetProgressPosition(unsigned position) noexcept override;
};
