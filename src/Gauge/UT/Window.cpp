/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
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

#include "ui/window/Window.hpp"
//#include "ContainerWindow.hpp"
//#include "Screen/Debug.hpp"
//#include "ui/event/Globals.hpp"
//#include "ui/event/Queue.hpp"

/**
 * \cond NEVER
 */
#ifdef ENABLE_OPENGL
#include "ui/canvas/opengl/Debug.hpp"
#endif

#include <xcsoar-cassert>

#if defined(USE_WINUSER) && !defined(NDEBUG)
#include <processthreadsapi.h>
#endif

//------------------------------------------------------------------------------
Window::~Window() noexcept
  {
  Destroy();
  }

#ifndef NDEBUG

//------------------------------------------------------------------------------
//void
//Window::AssertThread() const noexcept
//  {
//  }

//------------------------------------------------------------------------------
//void
//Window::AssertThreadOrUndefined() const noexcept
//  {
//  }

#endif /* !NDEBUG */

//------------------------------------------------------------------------------
void
Window::Destroy() noexcept
  {
  }

//------------------------------------------------------------------------------
ContainerWindow*
Window::GetRootOwner() noexcept
  {
  return nullptr;
  }

//------------------------------------------------------------------------------
void
Window::ScrollParentTo() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::OnCreate()
  {
  }

//------------------------------------------------------------------------------
void
Window::OnDestroy() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::OnResize([[maybe_unused]] PixelSize ) noexcept
  {
  }

//------------------------------------------------------------------------------
bool
Window::OnMouseMove([[maybe_unused]] PixelPoint,
                    [[maybe_unused]] unsigned keys) noexcept
  {
  /* not handled here */
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnMouseDown([[maybe_unused]] PixelPoint) noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnMouseUp([[maybe_unused]] PixelPoint) noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnMouseDouble(PixelPoint) noexcept
  {
  /* fall back to OnMouseDown() if the class didn't override
     OnMouseDouble() */
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnMouseWheel([[maybe_unused]] PixelPoint ,
                     [[maybe_unused]] int delta) noexcept
  {
  return false;
  }

#ifdef HAVE_MULTI_TOUCH

//------------------------------------------------------------------------------
bool
Window::OnMultiTouchDown() noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnMultiTouchUp() noexcept
  {
  return false;
  }

#endif /* HAVE_MULTI_TOUCH */

//------------------------------------------------------------------------------
bool
Window::OnKeyCheck([[maybe_unused]] unsigned key_code) const noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnKeyDown([[maybe_unused]] unsigned key_code) noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnKeyUp([[maybe_unused]] unsigned key_code) noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool
Window::OnCharacter([[maybe_unused]] unsigned ch) noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
void
Window::OnCancelMode() noexcept
  {
  ReleaseCapture();
  }

//------------------------------------------------------------------------------
void
Window::OnSetFocus() noexcept
  {
  assert(!focused);

  focused = true;
  }

//------------------------------------------------------------------------------
void
Window::OnKillFocus() noexcept
  {
  assert(focused);

  ReleaseCapture();

  focused = false;
  }

//------------------------------------------------------------------------------
Window*
Window::GetFocusedWindow() noexcept
  {
  return nullptr;
  }

//------------------------------------------------------------------------------
void
Window::SetFocus() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::ClearFocus() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::ClearCapture() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::ReleaseCapture() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::Invalidate() noexcept
  {
  }

//------------------------------------------------------------------------------
void
Window::AssertThread() const noexcept
  {
  }

/**
 * \endcond
 */
