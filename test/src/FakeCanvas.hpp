/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
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


#ifndef _FAKE_CANVAS_HPP
#define _FAKE_CANVAS_HPP

class RasterPoint;
class Angle;

class Canvas
  {

public:
  /**
   * Ctor.
   */
  Canvas();

  /**
   * Dtor.
   */
  ~Canvas();

  /**
   * Draw a polygon.
   * @param p Poloygon vertices.
   * @param n Number of vertices in p.
   */
  void DrawPolygon(const RasterPoint p[], const unsigned n);

  /**
   * Draw a keyhole.
   * @param x Reference x.
   * @param y Reference y.
   * @param sr Small radius.
   * @param br Big radius.
   * @param s Start vector.
   * @param e End vector.
   */
  void DrawKeyhole(int x, int y, unsigned sr, unsigned br, Angle s, Angle e);

private:
  unsigned frame; // Frame number.
  };

#endif  // _FAKE_CANVAS_HPP
