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

#ifdef ANDROID
#include "Android/Product.hpp"
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

/**
 * Returns whether this is a debug build.
 */
constexpr
static inline bool
IsDebug() noexcept
{
#ifdef NDEBUG
  return false;
#else
  return true;
#endif
}

/**
 * Is XCSoar running on ancient and slow hardware?  If yes, then some
 * expensive UI features are disabled.
 */
constexpr
static inline bool
IsAncientHardware() noexcept
{
  return false;
}

/**
 * Returns whether the application is running on Android
 */
constexpr
static inline bool
IsAndroid() noexcept
{
#if defined(ANDROID)
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on an apple device
 */
constexpr
static inline bool
IsApple() noexcept
{
#if defined(__APPLE__)
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on a Mac OS X device
 */
constexpr
static inline bool
IsMacOSX() noexcept
{
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on an iOS device
 */
constexpr
static inline bool
IsIOS() noexcept
{
#if defined(__APPLE__) && TARGET_OS_IPHONE
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on a Kobo e-book reader.
 */
constexpr
static inline bool
IsKobo() noexcept
{
#ifdef KOBO
  return true;
#else
  return false;
#endif
}

/**
 * Returns whether the application is running on an embedded platform.
 * @return True if host hardware is an embedded platform, False otherwise
 */
constexpr
static inline bool
IsEmbedded() noexcept
{
  return IsAndroid() || IsKobo() || IsIOS();
}

/**
 * Does this device have little main memory?  On those, some expensive
 * features are disabled.
 */
constexpr
static inline bool
HasLittleMemory() noexcept
{
  return IsAncientHardware();
}

/**
 * Returns whether the application is compiled with IOIOLib
 */
constexpr
static inline bool
HasIOIOLib() noexcept
{
#ifdef ANDROID
  return true;
#else
  return false;
#endif
}

/**
 * Does this device have a pointer device? (mouse or touch screen)
 * @return True if a touch screen or mouse is assumed for the hardware
 * that XCSoar is running on, False if the hardware has only buttons
 */
#if defined(USE_CONSOLE) && !defined(KOBO)
[[gnu::pure]]
bool
HasPointer() noexcept;
#else
constexpr
static inline bool
HasPointer() noexcept
{
  return true;
}

#endif

/**
 * Does this device have a touch screen?  This is useful to know for
 * sizing controls, as a touch screen may require bigger areas.
 */
#ifdef USE_LIBINPUT
[[gnu::pure]]
bool
HasTouchScreen() noexcept;
#else
constexpr
static inline bool
HasTouchScreen() noexcept
{
  return IsAndroid() || IsKobo() || IsIOS();
}
#endif

/**
 * Does this device have a keyboard device?
 * @return True if a keyboard is assumed for the hardware
 * that XCSoar is running on, False if the hardware has no keyboard
 */
#ifdef USE_LIBINPUT
[[gnu::pure]]
bool
HasKeyboard() noexcept;
#else
constexpr
static inline bool
HasKeyboard() noexcept
{
  return !IsEmbedded();
}
#endif

/**
 * Does this device have a cursor keys?  These may be used to navigate
 * in modal dialogs.  Without cursor keys, focused controls do not
 * need to be highlighted.
 */
#ifndef ANDROID
constexpr
#endif
static inline bool
HasCursorKeys() noexcept
{
  /* we assume that all Windows (CE) devices have cursor keys; some do
     not, but that's hard to detect */

#ifdef ANDROID
  return has_cursor_keys;
#else
  return !IsKobo() && !IsIOS();
#endif
}

/**
 * Does this device have a display with colors?
 */
#ifdef ANDROID
[[gnu::const]]
#else
constexpr
#endif
static inline bool
HasColors() noexcept
{
#ifdef ANDROID
  return !IsNookSimpleTouch();
#elif defined(GREYSCALE)
  return false;
#else
  return !IsKobo();
#endif
}

/**
 * Is dithering black&white used on the display?
 */
#if defined(ANDROID) && defined(__arm__)
[[gnu::const]]
#else
constexpr
#endif
static inline bool
IsDithered() noexcept
{
#ifdef DITHER
  return true;
#elif defined(ANDROID) && defined(__arm__)
  return is_dithered;
#else
  return false;
#endif
}

/**
 * Does this device have an electronic paper screen, such as E-Ink?
 * Such screens need some special cases, because they are very slow
 * and show ghosting.  Animations shall be disabled when this function
 * returns true.
 */
#if defined(ANDROID) && defined(__arm__)
[[gnu::const]]
#else
constexpr
#endif
static inline bool
HasEPaper() noexcept
{
#if defined(ANDROID) && defined(__arm__)
  return IsNookSimpleTouch();
#else
  return IsKobo();
#endif
}
