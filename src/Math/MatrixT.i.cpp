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
template<class T, size_t Trow, size_t Tcol> inline MatrixT<T, Trow, Tcol>&
MatrixT<T, Trow, Tcol>::operator=(const MatrixT<T, Trow, Tcol>& rhs)
  {
  this->a = rhs.a;
  return *this;
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline T
MatrixT<T, Trow, Tcol>::operator()(size_t i, size_t j)
  {
  assert(i < Trow);
  assert(j < Tcol);
  return (row(i))[j];
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline T
MatrixT<T, Trow, Tcol>::operator()(size_t i, size_t j) const
  {
  assert(i < Trow);
  assert(j < Tcol);
  return (row(i))[j];
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline slice_iter<T>
MatrixT<T, Trow, Tcol>::operator[](size_t i)
  {
  assert(i < Trow);
  return row(i);
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline Cslice_iter<T>
MatrixT<T, Trow, Tcol>::operator[](size_t i) const
  {
  assert(i < Trow);
  return row(i);
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline void
MatrixT<T, Trow, Tcol>::operator+(const MatrixT<T, Trow, Tcol>& rhs)
  {
  assert(rhs.nrow() == Trow);
  assert(rhs.ncol() == Tcol);
  for (int i = 0; i < Trow; i++)
    for (int j = 0; j < Tcol; j++)
      (*this)[i][j] = (*this)[i][j] + rhs[i][j];
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline void
MatrixT<T, Trow, Tcol>::operator-(const MatrixT<T, Trow, Tcol>& rhs)
  {
  assert(rhs.nrow() == Trow);
  assert(rhs.ncol() == Tcol);
  for (int i = 0; i < Trow; i++)
    for (int j = 0; j < Tcol; j++)
      (*this)[i][j] = (*this)[i][j] - rhs[i][j];
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline void
MatrixT<T, Trow, Tcol>::operator*(const MatrixT<T, Trow, Tcol>& rhs)
  {
  assert(rhs.nrow() == Tcol);
  assert(rhs.ncol() == Trow);
  for (int i = 0; i < Trow; i++)
    for (int j = 0; j < Tcol; j++)
      for (int k = 0; k < Tcol; k++)
        (*this)[i][j] = (*this)[i][k] * rhs[k][j];
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline slice_iter<T>
MatrixT<T, Trow, Tcol>::row(size_t i)
  {
  assert(i < Trow);
  return slice_iter<T>(&this->a, std::slice(i, Trow, Tcol));
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline Cslice_iter<T>
MatrixT<T, Trow, Tcol>::row(size_t i) const
  {
  assert(i < Trow);
  return Cslice_iter<T>(&(this->a), std::slice(i, Trow, Tcol));
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline size_t
MatrixT<T, Trow, Tcol>::nrow() const
  {
  return Trow;
  }

//------------------------------------------------------------------------------
template<class T, size_t Trow, size_t Tcol> inline size_t
MatrixT<T, Trow, Tcol>::ncol() const
  {
  return Tcol;
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

