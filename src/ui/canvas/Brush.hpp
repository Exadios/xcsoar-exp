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

#include "Color.hpp"
#include "Features.hpp"
#include "Screen/Debug.hpp"

#include <xcsoar-cassert>

#ifdef USE_GDI
class Bitmap;
#endif

/**
 * A Brush is used for drawing filled circles, rectangles and so on
 */
class Brush
{
protected:
#ifndef USE_GDI
  Color color = Color::Transparent();
#else
  HBRUSH brush = nullptr;
#endif

public:
#ifndef USE_GDI
  Brush() noexcept = default;

  constexpr explicit Brush(const Color _color) noexcept
    :color(_color)  {}
#else
  /** Base Constructor of the Brush class */
  Brush() noexcept = default;

  /**
   * Constructor (creates a Brush object of the given Color
   * @param c Color of the Brush
   */
  explicit Brush(const Color c) {
    Create(c);
  }

  /** Destructor */
  ~Brush() noexcept {
    Destroy();
  }

  Brush(const Brush &other) = delete;
  Brush &operator=(const Brush &other) = delete;
#endif

public:
  /**
   * Sets the Color of the Brush
   * @param c The new Color
   */
  void Create(const Color c);

#ifdef HAVE_HATCHED_BRUSH

  /**
   * Creates a bitmap-based Brush
   * @param bitmap The bitmap the new Brush will be based on
   */
  void Create(const Bitmap &bitmap);

#endif

  /**
   * Resets the Brush to nullptr
   */
  void Destroy() noexcept;

  /**
   * Returns whether the Brush is defined (!= nullptr)
   * @return True if the Brush is defined, False otherwise
   */
  bool IsDefined() const noexcept {
#ifndef USE_GDI
    return !color.IsTransparent();
#else
    return brush != nullptr;
#endif
  }

#ifndef USE_GDI
  constexpr bool IsHollow() const noexcept {
    return color.IsTransparent();
  }

  const Color GetColor() const noexcept {
    return color;
  }
#else
  /**
   * Returns the native HBRUSH object
   * @return The native HBRUSH object
   */
  HBRUSH Native() const noexcept {
    return brush;
  }
#endif

#ifdef ENABLE_OPENGL
  /**
   * Configures this brush in the OpenGL context.
   */
  void Bind() const noexcept {
    color.Bind();
  }

  void BindUniform(GLint location) const noexcept {
    color.Uniform(location);
  }
#endif /* OPENGL */
};

#ifndef USE_GDI

inline void
Brush::Create(const Color c)
{
  assert(IsScreenInitialized());

  color = c;
}

inline void
Brush::Destroy() noexcept
{
  assert(!IsDefined() || IsScreenInitialized());

  color = Color::Transparent();
}

#endif
