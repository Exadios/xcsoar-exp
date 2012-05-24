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

#include <Math/fixed.hpp>
#include <Math/Kalman/kalman/kmatrix.hpp>
#include <Math/Kalman/kalman/kvector.hpp>

/**
 * @file
 * Classes to implement matrices and vectors for the INU.
 */

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
     * Copy operator.
     * @param rhs The KVector of the rhs.
     * @return This KVector.
     */
    Kalman::KVector<fixed, 1, false> &operator=(const Kalman::KVector<fixed, 1, false>& rhs);

    /**
     * Index assignment.
     * @param i Index.
     * @return The vector element.
     */
    fixed& operator()(size_t i);

    /**
     * Index value.
     * @param i Index.
     * @return The value of the indexed element.
     */
    const fixed& operator()(size_t i) const;

  private:
    /**
     * The vector.
     */
    Kalman::KVector<fixed, 1, false>v;
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
     * Copy operator.
     * @param rhs The KMatrix of the rhs.
     * @return This KMatrix.
     */
    Kalman::KMatrix<fixed, 1, false>& operator=(const Kalman::KMatrix<fixed, 1, false>& rhs);

    /**
     * Index assignment.
     * @param i Row index.
     * @param j Column index.
     * @return The matrix row.
     */
    fixed& operator()(size_t i, size_t j);

    /**
     * Index value.
     * @param i Row index.
     * @param j Column index.
     * @return The matrix row.
     */
    const fixed& operator()(size_t i, size_t j) const;

  private:
    Kalman::KMatrix<fixed, 1, false>a;
};

#endif // INU_HPP
