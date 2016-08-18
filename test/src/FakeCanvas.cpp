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

#include "FakeCanvas.hpp"
#include "Screen/Memory/PixelTraits.hpp"
#include "Screen/Memory/Point.hpp"
#include "Math/Angle.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

//------------------------------------------------------------------------------
Canvas::Canvas()
  : frame(0)
  {
  }

//------------------------------------------------------------------------------
Canvas::~Canvas()
  {
  }

//------------------------------------------------------------------------------
void
Canvas::DrawPolygon(const RasterPoint *p, const unsigned n)
  {

  if (this->frame >= 100000)
    return; // Don't bother.

  std::ostringstream cn("Canvas.");
  cn << std::setfill('0') << std::setw(5) << this->frame << ".plot" << std::ends;
  std::ostringstream dn("Canvas.");
  dn << std::setfill('0') << std::setw(5) << this->frame << ".data" << std::ends;
  std::ofstream cf(cn.str());
  std::ofstream df(dn.str());
  if ((cf.fail() == true) || (df.fail() == true))
    {
    std::cerr << "Failed to open conrol or data file" << std::endl;
    return;
    }

  for (unsigned i = 0; i < n; i++)
    df << p[i].x << " " << p[i].y << std::endl;

  cf << "plot " << dn.str() << " withfillcurves closed" << std::endl;

  cf.close();
  df.close();

  this->frame++;
  }

//-----------------------------------------------------------------------------
void
Canvas::DrawKeyhole(int x, int y, unsigned sr, unsigned br, Angle s, Angle e)
  {
  ::KeyHole(*this, x, y, br, s, e, sr);
  }
