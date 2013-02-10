/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2013 The XCSoar Project
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

#include "FinalGlideBarLook.hpp"
#include "Screen/Layout.hpp"

static Color
ColorWithAlpha(const Color &c, uint8_t a)
{
#ifdef ENABLE_OPENGL
  return Color(c.Red(), c.Green(), c.Blue(), a);
#else
  return c;
#endif
}


void
FinalGlideBarLook::Initialise(const Font &_font)
{
  const uint8_t alpha = 0xA0;

  brush_below.Set(ColorWithAlpha(COLOR_RED, alpha));
  brush_below_mc0.Set(ColorWithAlpha(LightColor(COLOR_RED), alpha));
  pen_below.Set(Layout::ScalePenWidth(1), DarkColor(COLOR_RED));

  brush_below_landable.Set(ColorWithAlpha(COLOR_ORANGE, alpha));
  brush_below_landable_mc0.Set(ColorWithAlpha(LightColor(COLOR_ORANGE), alpha));
  pen_below_landable.Set(Layout::ScalePenWidth(1),
                         DarkColor(COLOR_ORANGE));

  brush_above.Set(ColorWithAlpha(COLOR_GREEN, alpha));
  brush_above_mc0.Set(ColorWithAlpha(LightColor(LightColor(COLOR_GREEN)), alpha));
  pen_above.Set(Layout::ScalePenWidth(1), DarkColor(COLOR_GREEN));

  font = &_font;
}
