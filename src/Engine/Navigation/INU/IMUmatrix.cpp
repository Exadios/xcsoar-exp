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
}
 */

#include "Navigation/INU/IMUmatrix.hpp"

//------------------------------------------------------------------------------
IMUvector::IMUvector()
  {
  this->v.resize(3);
  this->v = fixed(0);
  }

//------------------------------------------------------------------------------
IMUvector::IMUvector(fixed x0, fixed x1, fixed x2)
  {
  this->v(1) = x0;
  this->v(2) = x1;
  this->v(3) = x2;
  }

//------------------------------------------------------------------------------
IMUvector::IMUvector(const IMUvector& rhs)
  {
  for (size_t i = 1; i <= 3; i++)
    this->v(i) = rhs(i);
  }

//------------------------------------------------------------------------------
IMUvector::~IMUvector()
  {
  }

//------------------------------------------------------------------------------
IMUvector&
IMUvector::operator=(const IMUvector& rhs)
  {
  for (int i = 1; i <= 3; i++)
    this->v(i) = rhs(i);
  return *this;
  }

//------------------------------------------------------------------------------
Kalman::KVector<fixed, 1, false>&
IMUvector::operator=(const Kalman::KVector<fixed, 1, false> &rhs)
  {
  this->v = rhs;
  return this->v;
  }

//------------------------------------------------------------------------------
fixed&
IMUvector::operator()(size_t i)
  {
  return this->v(i);
  }

//------------------------------------------------------------------------------
const fixed&
IMUvector::operator()(size_t i) const
  {
  return this->v(i);
  }

//------------------------------------------------------------------------------
IMUmatrix::IMUmatrix()
  {
  this->a.resize(3, 3);
  this->a = fixed(0);
  }

//------------------------------------------------------------------------------
IMUmatrix::IMUmatrix(const IMUmatrix& rhs)
  {
  for (int i = 1; i <= 3; i++)
    for (int j = 1; j <= 3; j++)
      this->a(i, j) = rhs(i, j);
  }

//------------------------------------------------------------------------------
IMUmatrix::~IMUmatrix()
  {
  }

//------------------------------------------------------------------------------
IMUmatrix&
IMUmatrix::operator=(const IMUmatrix& rhs)
  {
  for (int i = 1; i <= 3; i++)
    for (int j = 1; j <= 3; j++)
      this->a(i, j) = rhs(i, j);
  return *this;
  }

//------------------------------------------------------------------------------
Kalman::KMatrix<fixed, 1, false>&
IMUmatrix::operator=(const Kalman::KMatrix<fixed, 1, false>& rhs)
  {
  this->a = rhs;
  return this->a;
  }

//------------------------------------------------------------------------------
fixed&
IMUmatrix::operator()(size_t i, size_t j)
  {
  return this->a(i, j);
  }

//------------------------------------------------------------------------------
const fixed&
IMUmatrix::operator()(size_t i, size_t j) const
  {
  return this->a(i, j);
  }

