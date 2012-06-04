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
#ifndef MATRIXT_HPP
#define MATRIXT_HPP

#include <valarray>

/**
 * @file
 * Classes to implement template matrices.
 */

template<typename T> class slice_iter;
template<typename T> class Cslice_iter;

template<class T> class MatrixT
{
  public:
    /**
     * Default Ctor. Elements are initialized to 0.
     */
    MatrixT();

    /**
     * Ctor.
     * @param m Number of rows.
     * @param n Number of columns
     */
    explicit MatrixT(size_t m, size_t n);

    /**
     * Copy Ctor.
     * @param rhs Right hand side.
     */
    MatrixT(const MatrixT& rhs);

    /**
     * Dtor.
     */
    virtual ~MatrixT();

    /**
     * Copy operator.
     * @param rhs The right hand side.
     * @return The value of this object.
     */
    MatrixT& operator=(const MatrixT &rhs);

    /**
     * Index assignment.
     * @param i Row index.
     * @param j Column index.
     * @return The matrix row.
     */
    T operator()(size_t i, size_t j);

    /**
     * Index value.
     * @param i Row index.
     * @param j Column index.
     * @return The matrix row.
     */
    T operator()(size_t i, size_t j) const;

    /**
     * Index assignment.
     * @param i Row index.
     * @return The iterator.
     */
    slice_iter<T> operator[](size_t i);

    /**
     * Index value.
     * @param i Row index.
     * @return The iterator.
     */
    Cslice_iter<T> operator[](size_t i) const;

    /**
     * Give the row.
     * @param i The row index.
     * @return The row.
     */
    slice_iter<T> row(size_t i);

    /**
     * Give the row.
     * @param i The row index.
     * @return The row.
     */
    Cslice_iter<T> row(size_t i) const;

    /**
     * Give the column.
     * @param i Column index.
     * @return The column.
     */
    slice_iter<T> column(size_t i);

    /**
     * Give the column.
     * @param i Column index.
     * @return The column.
     */
    Cslice_iter<T> column(size_t i) const;

  private:
    /**
     * The matrix.
     */
    std::valarray<T> a;
};

template<class T> class slice_iter
{
  public:
    /**
     * Ctor.
     * @param a The array.
     * @param s The slice of the array.
     */
    slice_iter(std::valarray<T> *a, std::slice s);

    /**
     * Dtor.
     */
    virtual ~slice_iter();

    /**
     * The end of iteration.
     * @return The last iterator + 1.
     */
    slice_iter end();

    /**
     * Increment.
     * @return This + 1.
     */
    slice_iter& operator++();

    /**
     * Indexed assignment.
     * @param i Index.
     * @return The indexed element.
     */
    T& operator[](size_t i);

    /**
     * Indexed assignment.
     * @param i Index.
     * @return The indexed element.
     */
    T& operator()(size_t i);

    /**
     * Dereference.
     * @param The pointer.
     * @return The dereferenced element.
     */
    T& operator*();

  private:

    /**
     * Reference to the array.
     */
    std::valarray<T> *v;

    /**
     * The current slice.
     */
    std::slice s;

    /**
     * The current index.
     */
    size_t i;

    /**
     * Dereference.
     * @param i Index.
     * @return The dereferenced element.
     */
    T& deref(size_t i) const;
};

template<typename T> class Cslice_iter
{
  public:
    /**
     * Ctor.
     * @param a The array.
     * @param s The slice of the array.
     */
    Cslice_iter(const std::valarray<T> *v, std::slice s);

    /**
     * Dtor.
     */
    virtual ~Cslice_iter();

    /**
     * The end of iteration.
     * @return The last iterator + 1.
     */
    const Cslice_iter end();

    /**
     * Increment.
     * @return This + 1.
     */
    const Cslice_iter& operator++();

    /**
     * Index value.
     * @param i Index.
     * @return The indexed element.
     */
    const T& operator[](size_t i);

    /**
     * Index value.
     * @param i Index.
     * @return The indexed element.
     */
    const T& operator()(size_t i);

    /**
     * Dereference.
     * @param The pointer.
     * @return The dereferenced element.
     */
    const T& operator*() const;

  private:
    /**
     * Reference to the array.
     */
    const std::valarray<T> *a;

    /**
     * The current slice.
     */
    std::slice s;

    /**
     * The current index.
     */
    size_t i;

    /**
     * Dereference.
     * @param i Index.
     * @return The dereferenced element.
     */
    const T& deref(size_t i) const;
};

#include "MatrixT.i.cpp"

#endif // MATRIXT_HPP
