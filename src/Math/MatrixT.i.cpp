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
template<class T> inline MatrixT<T>&
MatrixT<T>::operator=(const MatrixT<T>& rhs)
  {
  this->a = rhs.a;
  return *this;
  }

//------------------------------------------------------------------------------
template<class T> inline T
MatrixT<T>::operator()(size_t i, size_t j)
  {
  return (row(i))[j];
  }

//------------------------------------------------------------------------------
template<class T> inline T
MatrixT<T>::operator()(size_t i, size_t j) const
  {
  return (row(i))[j];
  }

//------------------------------------------------------------------------------
template<class T> inline slice_iter<T>
MatrixT<T>::operator[](size_t i)
  {
  return row(i);
  }

//------------------------------------------------------------------------------
template<class T> inline Cslice_iter<T>
MatrixT<T>::operator[](size_t i) const
  {
  return row(i);
  }

//------------------------------------------------------------------------------
template<class T> inline slice_iter<T>
MatrixT<T>::row(size_t i)
  {
  return slice_iter<T>(&this->a, std::slice(i, 3, 3));
  }

//------------------------------------------------------------------------------
template<class T> inline Cslice_iter<T>
MatrixT<T>::row(size_t i) const
  {
  return Cslice_iter<T>(&(this->a), std::slice(i, 3, 3));
  }

//------------------------------------------------------------------------------
template<class T> inline slice_iter<T>
slice_iter<T>::end()
  {
  slice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
template<class T> inline slice_iter<T>&
slice_iter<T>::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
template<class T> inline T&
slice_iter<T>::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
template<class T> inline T&
slice_iter<T>::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
template<class T> inline T&
slice_iter<T>::operator*()
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
template<class T> inline T&
slice_iter<T>::deref(size_t i) const
  {
  return (*this->a)[this->s.start() + i * this->s.stride()];
  }

//------------------------------------------------------------------------------
template<class T> inline const Cslice_iter<T>
Cslice_iter<T>::end()
  {
  Cslice_iter t = *this;
  t.i = s.size();
  return t;
  }

//------------------------------------------------------------------------------
template<class T> inline const Cslice_iter<T>&
Cslice_iter<T>::operator++()
  {
  this->i++;
  return *this;
  }

//------------------------------------------------------------------------------
template<class T> inline const T&
Cslice_iter<T>::operator[](size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
template<class T> inline const T&
Cslice_iter<T>::operator()(size_t i)
  {
  return this->deref(this->i = i);
  }

//------------------------------------------------------------------------------
template<class T> inline const T&
Cslice_iter<T>::operator*() const
  {
  return this->deref(this->i);
  }

//------------------------------------------------------------------------------
template<class T> inline const T&
Cslice_iter<T>::deref(size_t i) const
  {
  return (*this->a)[this->s.start() + i * this->s.stride()];
  }

