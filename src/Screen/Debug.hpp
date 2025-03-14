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

#ifdef NDEBUG

static inline void
ScreenInitialized() {}

static inline void
ScreenDeinitialized() {}

#else

/**
 * Call this when the screen library has been initialized.
 */
void
ScreenInitialized();

/**
 * Call this when the screen library has been deinitialized.
 */
void
ScreenDeinitialized();

/**
 * Determine if the screen library has been initialized and is
 * available.
 */
bool
IsScreenInitialized();

#endif
