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
#ifndef INU_HPP
#define INU_HPP

#include <valarray>
#include <Math/fixed.hpp>
#include <Math/Kalman/kalman/kmatrix.hpp>
#include <Math/Kalman/kalman/kvector.hpp>

/**
 * @file
 * Classes to implement matrices and vectors for the INU.
 */

class IMUslice_iter;
class IMUCslice_iter;

typedef std::slice IMUslice;
typedef std::slice_array<fixed> IMUslice_array;

class IMUvector
{
  public:
    /**
     * Default Ctor. Elements are initialized to 0.
     */
    IMUvector();

    /**
     * Initialization Ctor.
     * @param x0 The first element.
     * @param x1 The second element.
     * @param x2 The third element.
     */
    IMUvector(fixed x0, fixed x1, fixed x2);

    /**
     * Copy Ctor.
     * @param rhs The right hand side.
     */
    IMUvector(const IMUvector& rhs);

    /**
     * Dtor.
     */
    ~IMUvector();

    /**
     * Copy operator.
     * @param rhs The right hand side.
     * @return The value of this object.
     */
    IMUvector &operator=(const IMUvector& rhs);

    /**
     * Index assignment.
     * @param i Index.
     * @return The vector element.
     */
    fixed& operator[](size_t i);

    /**
     * Index value.
     * @param i Index.
     * @return The value of the indexed element.
     */
    const fixed& operator[](size_t i) const;

  private:
    /**
     * The vector.
     */
    std::valarray<fixed> v;
};

class IMUmatrix
{
  public:
    /**
     * Default Ctor. Elements are initialized to 0.
     */
    IMUmatrix();

    /**
     * Copy Ctor.
     * @param rhs Right hand side.
     */
    IMUmatrix(const IMUmatrix& rhs);

    /**
     * Dtor.
     */
    virtual ~IMUmatrix();

    /**
     * Copy operator.
     * @param rhs The right hand side.
     * @return The value of this object.
     */
    IMUmatrix& operator=(const IMUmatrix &rhs);

    /**
     * Index assignment.
     * @param i Row index.
     * @return The matrix row.
     */
    IMUslice_iter operator()(size_t i);

    /**
     * Index value.
     * @param i Row index.
     * @return The matrix row.
     */
    IMUCslice_iter operator()(size_t i) const;

    /**
     * Index assignment.
     * @param i Row index.
     * @return The iterator.
     */
    IMUslice_iter operator[](size_t i);

    /**
     * Index value.
     * @param i Row index.
     * @return The iterator.
     */
    IMUCslice_iter operator[](size_t i) const;

    /**
     * Give the row.
     * @param i The row index.
     * @return The row.
     */
    IMUslice_iter row(size_t i);

    /**
     * Give the row.
     * @param i The row index.
     * @return The row.
     */
    IMUCslice_iter row(size_t i) const;

  private:
    std::valarray<fixed> a;
};

class IMUslice_iter
{
  public:
    /**
     * Ctor.
     */
    IMUslice_iter(std::valarray<fixed> *v, IMUslice s);

    /**
     * Dtor.
     */
    virtual ~IMUslice_iter();

    /**
     * The end of iteration.
     * @return The last itterator + 1.
     */
    IMUslice_iter end();

    /**
     * Increment.
     * @return This + 1.
     */
    IMUslice_iter& operator++();

    /**
     * Index assignment.
     * @param i Index.
     * @return The indexed element.
     */
    fixed& operator[](size_t i);

    /**
     * Index assignment.
     * @param i Index.
     * @return The indexed element.
     */
    fixed& operator()(size_t i);

    /**
     * Dereference.
     * @param The pointer.
     * @return The dereferenced element.
     */
    fixed& operator*();

  private:
    std::valarray<fixed> *v;
    IMUslice s;
    size_t i;

    /**
     * Dereference.
     * @param i Index.
     * @return The dereferenced element.
     */
    fixed& deref(size_t i) const;
};

class IMUCslice_iter
{
  public:
    /**
     * Ctor.
     */
    IMUCslice_iter(const std::valarray<fixed> *v, IMUslice s);

    /**
     * Dtor.
     */
    virtual ~IMUCslice_iter();

    /**
     * The end of iteration.
     * @return The last itterator + 1.
     */
    const IMUCslice_iter end();

    /**
     * Increment.
     * @return This + 1.
     */
    const IMUCslice_iter& operator++();

    /**
     * Index assignment.
     * @param i Index.
     * @return The indexed element.
     */
    const fixed& operator[](size_t i);

    /**
     * Index assignment.
     * @param i Index.
     * @return The indexed element.
     */
    const fixed& operator()(size_t i);

    /**
     * Dereference.
     * @param The pointer.
     * @return The dereferenced element.
     */
    const fixed& operator*() const;

  private:
    const std::valarray<fixed> *v;
    IMUslice s;
    size_t i;

    /**
     * Dereference.
     * @param i Index.
     * @return The dereferenced element.
     */
    const fixed& deref(size_t i) const;
};

#include "IMUmatrix.i.cpp"

#endif // INU_HPP
