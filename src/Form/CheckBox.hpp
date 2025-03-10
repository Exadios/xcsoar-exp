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

#include "ui/window/PaintWindow.hpp"
#include "util/tstring.hpp"

#include <xcsoar-cassert>
#include <functional>

struct DialogLook;
class ContainerWindow;

/**
 * This class is used for creating buttons.
 */
class CheckBoxControl : public PaintWindow {
  bool checked, dragging, pressed;

  const DialogLook *look;
  tstring caption;

  using Callback = std::function<void(bool)>;
  Callback callback;

public:
  void Create(ContainerWindow &parent, const DialogLook &look,
              tstring::const_pointer caption,
              const PixelRect &rc,
              const WindowStyle style,
              Callback _callback) noexcept;

  [[gnu::pure]]
  static unsigned GetMinimumWidth(const DialogLook &look, unsigned height,
                                  tstring::const_pointer caption) noexcept;

  /**
   * Set the function that will receive click events.
   */
  void SetCallback(Callback _callback) noexcept {
    assert(!_callback);

    callback = std::move(_callback);
  }

  bool GetState() const {
    return checked;
  }

  void SetState(bool value);

protected:
  void SetPressed(bool value);

  virtual bool OnClicked() noexcept;

  /* virtual methods from class Window */
  bool OnKeyCheck(unsigned key_code) const noexcept override;
  bool OnKeyDown(unsigned key_code) noexcept override;
  bool OnMouseMove(PixelPoint p, unsigned keys) noexcept override;
  bool OnMouseDown(PixelPoint p) noexcept override;
  bool OnMouseUp(PixelPoint p) noexcept override;
  void OnSetFocus() noexcept override;
  void OnKillFocus() noexcept override;
  void OnCancelMode() noexcept override;
  void OnPaint(Canvas &canvas) noexcept override;
};
