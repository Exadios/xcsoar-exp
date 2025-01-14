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

//#include "Features.hpp"
#include "ui/dim/Rect.hpp"
#include "ui/dim/Point.hpp"
#include "ui/dim/Size.hpp"

#include <xcsoar-cassert>

#ifdef USE_WINUSER
#include <windef.h> // for HWND (needed by winuser.h)
#include <winuser.h>
#else
#include <boost/intrusive/list_hook.hpp>
#endif

/**
 * \cond NEVER
 */
class Font;
class Canvas;
class ContainerWindow;

/**
 * A portable wrapper for describing a window's style settings on
 * creation.
 */
class WindowStyle
  {
protected:
  bool visible = true;
  bool enabled = true;
  bool tab_stop = false, control_parent = false;
  bool has_border = false;


public:
  /** The window is initially not visible. */
  void Hide() noexcept
    {
    visible = false;
    }

  /**
   * The window is initially disabled.
   * A disabled window cannot receive input from the user.
   */
  void Disable() noexcept
    {
    enabled = false;
    }

  /**
   * The window is a control that can receive the keyboard focus when the
   * user presses the TAB key. Pressing the TAB key changes the keyboard
   * focus to the next control with the WS_TABSTOP style.
   */
  void TabStop() noexcept
    {
    tab_stop = true;
    }

  /**
   * If the search for the next control with the WS_TABSTOP style encounters
   * a window with the WS_EX_CONTROLPARENT style, the system recursively
   * searches the window's children.
   */
  void ControlParent() noexcept
    {
    control_parent = true;
    }

  /** The window has a thin-line border. */
  void Border() noexcept
    {
    has_border = true;
    }

  /** The window has a sunken 3D border. */
  void SunkenEdge() noexcept 
    {
    Border();
    }

  /** The window has a vertical scroll bar. */
  void VerticalScroll() noexcept
    {
    }

  void Popup()
    {
    }

  friend class Window;
  };

/**
 * A Window is a portion on the screen which displays something, and
 * which optionally interacts with the user.  To draw custom graphics
 * into a Window, derive your class from #PaintWindow.
 */
class Window
  {
  friend class ContainerWindow;

  friend class WindowList;
  typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>> SiblingsHook;
  SiblingsHook siblings;

protected:
  ContainerWindow *parent = nullptr;

private:

private:
  bool tab_stop, control_parent;

  bool visible = true;
  bool transparent = false;
  bool enabled;
  bool focused = false;
  bool capture = false;
  bool has_border = false;

public:
  Window() = default;
  virtual ~Window() noexcept;

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  ContainerWindow *GetParent() const noexcept
    {
    assert(IsDefined());

    return parent;
    }

protected:

  bool HasBorder() const noexcept
    {
    return true;
    }

public:
  bool IsDefined() const noexcept
    {
    return true;
    }

  PixelPoint GetTopLeft() const noexcept
    {
    PixelPoint p;
    return p;
    }

  void Create(ContainerWindow *, const PixelRect, const WindowStyle ) noexcept;

  void Destroy() noexcept;

  /**
   * Determines the root owner window of this Window.  This is
   * probably a pointer to the #MainWindow instance.
   */
  [[gnu::pure]]
  ContainerWindow *GetRootOwner() noexcept;

  /**
   * Checks whether the window is "maximised" within its parent
   * window, i.e. whether its dimensions are not smaller than its
   * parent's dimensions.
   */
  [[gnu::pure]]
  bool IsMaximised() const noexcept;

  void Move(PixelPoint ) noexcept
    {
    }

  void Move(PixelPoint, PixelSize) noexcept
    {
    }

  void Move(const PixelRect ) noexcept
    {
    }

  void MoveToCenter() noexcept
    {
    }

  /**
   * Like Move(), but does not trigger a synchronous redraw.  The
   * caller is responsible for redrawing.
   */
  void FastMove(PixelPoint , PixelSize ) noexcept
    {
    }

  void FastMove(const PixelRect) noexcept
    {
    }

  /**
   * Move the Window to the specified position within the parent
   * ContainerWindow and make it visible.
   */
  void MoveAndShow(const PixelRect ) noexcept
    {
    }

  void Resize(PixelSize) noexcept
    {
    }

  void BringToTop() noexcept;
  void BringToBottom() noexcept;

  void ShowOnTop() noexcept
    {
    }

  /**
   * Determine whether this Window is visible.  This method disregards
   * the visibility of parent windows, it just checks if the "visible"
   * flag is set for this Window.
   */
  [[gnu::pure]]
  bool IsVisible() const noexcept
    {
    return true;
    }

  void Show() noexcept;
  void Hide() noexcept;

  /**
   * Like Hide(), but does not trigger a synchronous redraw of the
   * parent window's background.
   */
  void FastHide() noexcept
    {
    }

  void SetVisible(bool ) noexcept
    {
    }

  /**
   * Attempt to make this window visible by scrolling the parent (via
   * ContainerWindow::ScrollTo()).
   */
  void ScrollParentTo() noexcept;

  bool IsTransparent() const noexcept
    {
    return true;
    }

  /**
   * Declare this window "transparent".  This means that portions of
   * the windows below it may be visible, and it will not be
   * considered "covering" windows behind it completely.  This flag is
   * evaluated by WindowList::IsCovered().
   */
  void SetTransparent() noexcept
    {
    }

  [[gnu::pure]]
  bool IsTabStop() const noexcept
    {
    return false;
    }

  [[gnu::pure]]
  bool IsControlParent() const noexcept
    {
    return false;
    }

  /**
   * Can this window get user input?
   */
  [[gnu::pure]]
  bool IsEnabled() const noexcept
    {
    return true;
    }

  /**
   * Specifies whether this window can get user input.
   */
  void SetEnabled(bool enabled) noexcept;

  virtual Window *GetFocusedWindow() noexcept;
  virtual void SetFocus() noexcept;

  /**
   * Called by the parent window when this window loses focus, or when
   * one of its (indirect) child windows loses focus.  This method is
   * responsible for invoking OnKillFocus().
   */
  virtual void ClearFocus() noexcept;

  /**
   * Send keyboard focus to this window's parent.  This should usually
   * only be called when this window owns the keyboard focus, and
   * doesn't want it anymore.
   */
  void FocusParent() noexcept;

  [[gnu::pure]]
  bool HasFocus() const noexcept
    {
    return true;
    }

  void SetCapture() noexcept;
  void ReleaseCapture() noexcept;
  virtual void ClearCapture() noexcept;

protected:
  virtual void EnableCapture() noexcept {}
  virtual void DisableCapture() noexcept {}

public:

  /**
   * Returns the position within the parent window.
   */
  [[gnu::pure]]
  const PixelRect GetPosition() const noexcept;

  /**
   * Translate coordinates relative to this window to coordinates
   * relative to the parent window.
   */
  [[gnu::pure]]
  PixelPoint ToParentCoordinates(const PixelPoint p) const noexcept
    {
    return p;
    }

  [[gnu::pure]]
  PixelRect ToParentCoordinates(const PixelRect &r) const noexcept
    {
    return r;
    }

  [[gnu::pure]]
  const PixelRect GetClientRect() const noexcept
    {
    PixelRect r;
    return r;
    }

  [[gnu::pure]]
  const PixelSize GetSize() const noexcept
    {
    PixelSize p;
    return p;
    }

  /**
   * Checks whether the specified coordinates are inside the Window's
   * client area.
   */
  [[gnu::pure]]
  bool IsInside(PixelPoint ) const noexcept
    {
    return false;
    }

  /**
   * Returns the parent's client area rectangle.
   */
  [[gnu::pure]]
  PixelRect GetParentClientRect() const noexcept;

  virtual void Invalidate() noexcept;

protected:
public:
  /**
   * @return true on success, false if the window should not be
   * created
   */
  virtual void OnCreate();
  virtual void OnDestroy() noexcept;
  virtual void OnResize(PixelSize new_size) noexcept;
  virtual bool OnMouseMove(PixelPoint p, unsigned keys) noexcept;
  virtual bool OnMouseDown(PixelPoint p) noexcept;
  virtual bool OnMouseUp(PixelPoint p) noexcept;
  virtual bool OnMouseDouble(PixelPoint p) noexcept;
  virtual bool OnMouseWheel(PixelPoint p, int delta) noexcept;

  /**
   * Checks if the window wishes to handle a special key, like cursor
   * keys and tab.  This wraps the WIN32 message WM_GETDLGCODE.
   *
   * @return true if the window will handle they key, false if the
   * dialog manager may use it
   */
  [[gnu::pure]]
  virtual bool OnKeyCheck(unsigned key_code) const noexcept;

  virtual bool OnKeyDown(unsigned key_code) noexcept;
  virtual bool OnKeyUp(unsigned key_code) noexcept;

  /**
   * A character was entered with the (virtual) keyboard.  This will
   * not be called if OnKeydown() has been handled already.
   *
   * @param ch the unicode character
   * @return true if the event was handled
   */
  virtual bool OnCharacter(unsigned ch) noexcept;

  virtual void OnCancelMode() noexcept;
  virtual void OnSetFocus() noexcept;
  virtual void OnKillFocus() noexcept;

public:
  };

/**
 * \endcond
 */
