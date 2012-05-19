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

class IMUvector;
class IMUmatrix;


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
    fixed operator[](size_t i) const;

  private:
    /**
     * The vector.
     */
    fixed v[3];
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
     * @param i Index.
     * @return The matrix row.
     */
    IMUvector& operator[](size_t i);

    /**
     * Index value.
     * @param i Index.
     * @return The matrix row.
     */
    IMUvector operator[](size_t i) const;

  private:
    IMUvector a[3];
};

/**
 * @file
 * A class to implement a Inertial Navigation Unit (INU) using gyros and
 * accelerometers (typically MEMS devices) found on many Android platforms.
 *
 */

class Inu
{
  public:
  
    /**
     * CTOR.
     * @param dt The IMU sample rate in seconds.
     */
    Inu(fixed dt);

    /**
     * DTOR.
     */
     virtual ~Inu();

    /**
     * Update the state of the INS. To be called once every IMU sample, \deltaT.
     * @param Omega The vector of the gyro rotation velocities in the 'b'
     *              domain.
     * @param f The vector of the accelerometer forces in the 'b' domain.
     * @param v The vector of the velocities in the 'e' domain.
     * @param gx The value of gravity, G, at our current position in the
     *           'e' domain.
     * @result If a result can be computed then true, false otherwise.
     */
     bool Update(IMUmatrix& Omega, IMUvector& f, IMUvector& v, fixed gx);

   private:
     
     /**
      * Multiply two 3x3 matrices.
      * @param A The L.H. matrix.
      * @param B The R.H. matrix.
      * @return The result matrix.
      */
     inline IMUmatrix multiply(const IMUmatrix& A, const IMUmatrix& B)
       {
       int i, j;
       IMUmatrix R;

       for (i = 0; i < 3; i++)
         for (j = 0; j < 3; j++)
	   R[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j];
       return R;
       }

     /**
      * Compute an approximate gravitational force ans a function of height
      * above the Geod.
      * @param h Height above the Geog in meters.
      * @return Gravitational acceleration at h.
      */
     fixed gfh(fixed h) const;

     /**
      * The IMU sample rate in seconds.
      */
     fixed dt;

     /**
      * The attitude state in the 'e' domain.
      */
     IMUmatrix R;

     /**
      * The acceleration state in the 'e' domain.
      */
     IMUvector a;
     
     /**
      * The attitude rate in the 'e' domain.
     */
     IMUmatrix Omega;

     /**
      * Has Compute been initialized by one, or more, sample(s).
      */
     bool init;
};

#if 0
class std::IMUslice
{
  public:
    /**
     * Ctor.
     */
    IMUslice();

    /**
     * Ctor.
     * @param start The start index.
     * @param stride The distance between elements.
     */
    IMUslice(size_t start, size_t stride);

    /**
     * Dtor.
     */
    virtual ~IMUslice();

  private:
    /**
     * Start index.
     */
    size_t start;
};

class std::IMUslice_iter
{
  public:
    /**
     * Ctor.
     */
    IMUslice_iter(valarray<fixed *v, IMUslice s);

    /**
     */
};
#endif

#endif // INU_HPP
