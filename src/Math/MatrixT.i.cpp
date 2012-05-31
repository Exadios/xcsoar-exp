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
inline MatrixT&
MatrixT::operator=(const MatrixT& rhs)
  {
  this->a = rhs.a;
  return *this;
  }

//------------------------------------------------------------------------------
inline slice_iter
MatrixT::operator()(size_t i)
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline Cslice_iter
MatrixT::operator()(size_t i) const
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline slice_iter
MatrixT::operator[](size_t i)
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline Cslice_iter
MatrixT::operator[](size_t i) const
  {
  return row(i);
  }

//------------------------------------------------------------------------------
inline slice_iter
MatrixT::row(size_t i)
  {
  return slice_iter(&this->a, slice(i, 3, 3));
  }

//------------------------------------------------------------------------------
inline Cslice_iter
MatrixT::row(size_t i) const
  {
  return Cslice_iter(&(this->a), slice(i, 3, 3));
  }

//------------------------------------------------------------------------------
inline slice_iter
slice_iter::end()
  {
  slice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
inline slice_iter&
slice_iter::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
inline T&
slice_iter::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline T&
slice_iter::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline T&
slice_iter::operator*()
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
inline T&
slice_iter::deref(size_t i) const
  {
  return (*this->a)[this->s.start() + i * this->s.stride()];
  }

//------------------------------------------------------------------------------
inline const Cslice_iter
Cslice_iter::end()
  {
  Cslice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
inline const Cslice_iter&
Cslice_iter::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
inline const T&
Cslice_iter::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline const T&
Cslice_iter::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
inline const T&
Cslice_iter::operator*() const
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
inline const T&
Cslice_iter::deref(size_t i) const
  {
  return (*this->a)[this->s.start() + i * this->s.stride()];
  }

