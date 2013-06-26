/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
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
}*/

#include "InuVector.hpp"
#include "InuMatrix.hpp"

//------------------------------------------------------------------------------
InuVector::InuVector()
  {
  this->resize(3);
  }

//------------------------------------------------------------------------------
InuVector::~InuVector()
  {
  }

//------------------------------------------------------------------------------
InuVector
operator+(const InuVector& x, const InuVector& y)
  {
  InuVector rtn;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      rtn(i) = x(i) + y(i);
  return rtn;
  }

//------------------------------------------------------------------------------
InuVector
operator-(const InuVector& x, const InuVector& y)
  {
  InuVector rtn;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      rtn(i) = x(i) - y(i);
  return rtn;
  }

//------------------------------------------------------------------------------
InuVector
operator^(const InuVector& x, const InuVector& y)
  {
  InuVector rtn;

  rtn(0) = x(1) * y(2) - x(2) * y(1);
  rtn(1) = x(2) * y(0) - x(0) * y(2);
  rtn(2) = x(0) * y(1) - x(1) * y(0);
  return rtn;
  }

//------------------------------------------------------------------------------
double
operator*(const InuVector& x, const InuVector& y)
  {
  return x(0) * y(0) + x(1) * y(1) + x(2) * y(2);
  }
