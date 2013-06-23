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

#include "InuMatrix.hpp"
#include "InuVector.hpp"

//------------------------------------------------------------------------------
InuMatrix::InuMatrix()
  {
  this->resize(3, 3);
  }

//------------------------------------------------------------------------------
InuMatrix::~InuMatrix()
  {
  }

//------------------------------------------------------------------------------
InuMatrix
operator+(const InuMatrix& x, const InuMatrix& y)
  {
  InuMatrix rtn;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      rtn(i, j) = x(i, j) + y(i, j);
  return rtn;
  }

//------------------------------------------------------------------------------
InuMatrix
operator-(const InuMatrix& x, const InuMatrix& y)
  {
  InuMatrix rtn;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      rtn(i, j) = x(i, j) - y(i, j);
  return rtn;
  }

//------------------------------------------------------------------------------
InuMatrix
operator*(const InuMatrix& x, const InuMatrix& y)
  {
  InuMatrix rtn;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      {
      rtn(i, j) = 0.0;
      for (int k = 0; k < 3; k++)
        rtn(i, j) += x(i, k) * y(k, j);
      }
  return rtn;
  }

//------------------------------------------------------------------------------
InuMatrix
operator*(const InuMatrix& x, const InuVector& y)
  {
  InuVector rtn;

  for (int i = 0; i < 3; i++)
    rtn(i) = x(i, 0) * y(0) + x(i, 1) * y(1) + x(i, 2) * y(2);
  }
