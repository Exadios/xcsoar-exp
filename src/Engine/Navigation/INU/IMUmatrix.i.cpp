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


//------------------------------------------------------------------------------
inline IMUvector&
IMUvector::operator=(const IMUvector& rhs)
  {
  for (int i = 1; i <= 3; i++)
    this->v[i] = rhs[i];
  return *this;
  }

//------------------------------------------------------------------------------
inline fixed&
IMUvector::operator[](size_t i)
  {
  return this->v[i];
  }

//------------------------------------------------------------------------------
inline const fixed&
IMUvector::operator[](size_t i) const
  {
  return this->v[i];
  }

//------------------------------------------------------------------------------
inline IMUmatrix&
IMUmatrix::operator=(const IMUmatrix& rhs)
  {
  this->a = rhs.a;
  return *this;
  }

//------------------------------------------------------------------------------
inline IMUslice_iter
IMUmatrix::operator()(size_t i)
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline IMUCslice_iter
IMUmatrix::operator()(size_t i) const
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline IMUslice_iter
IMUmatrix::operator[](size_t i)
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline IMUCslice_iter
IMUmatrix::operator[](size_t i) const
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline IMUslice_iter
IMUmatrix::row(size_t i)
  {
  return IMUslice_iter(&this->a, IMUslice(i, 3, 3));
  }

//------------------------------------------------------------------------------
inline IMUCslice_iter
IMUmatrix::row(size_t i) const
  {
  return IMUCslice_iter(&(this->a), IMUslice(i, 3, 3));
  }

//------------------------------------------------------------------------------
inline IMUslice_iter
IMUslice_iter::end()
  {
  IMUslice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
inline IMUslice_iter&
IMUslice_iter::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
inline fixed&
IMUslice_iter::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline fixed&
IMUslice_iter::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline fixed&
IMUslice_iter::operator*()
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
inline fixed&
IMUslice_iter::deref(size_t i) const
  {
  return (*this->v)[this->s.start() + i * this->s.stride()];
  }

//------------------------------------------------------------------------------
inline const IMUCslice_iter
IMUCslice_iter::end()
  {
  IMUCslice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
inline const IMUCslice_iter&
IMUCslice_iter::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
inline const fixed&
IMUCslice_iter::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline const fixed&
IMUCslice_iter::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline const fixed&
IMUCslice_iter::operator*() const
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
inline const fixed&
IMUCslice_iter::deref(size_t i) const
  {
  return (*this->v)[this->s.start() + i * this->s.stride()];
  }

