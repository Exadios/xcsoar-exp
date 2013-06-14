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

#ifndef IMU_HPP
#define IMU_HPP

/**
 * \ingroup test_Inu
 */
 /* @{ */

#include "Math/kalman/kalman/kvector.hpp"

namespace InuSimulator
  {

  typedef Kalman::KVector<fixed, false> IMUVector;
  typedef Kalman::KMatrix<fixed, false> IMUMatrix;

  class Imu
    {
  public:
    /**
     * Ctor.
     * @param dt The sample interval in seconds.
     */
    Imu(double dt);

    /**
     * Dtor.
     */
    virtual ~Imu();

    /**
     * Advance the state of the system by one sample interval.
     */
    void Update();

    /**
     * Give the accelerometer data of this state. \f$.\f$.
     * @return The acc data, \f$\begin{bmatrix} x & y & z \end{bmatrix}\f$, in
     *         meters per second per second.
     */
    IMUVector &Accelerometer() const;

    /**
     * Give the gyro data of this state.
     * @return The gyro data, $\begin{bmatrix} x & y & z \end{bmatrix}, in radians per second.
     */
    IMUVector &Gyro() const;

    /**
     * Initialize the $R^b_n$ roation for time, $t_0$.
     * @param t_0 The Matrix at $t_0$
     */
    void R_caret_n_b(const KMatrix &t_0);

    /**
     * Initialize the $\Omega^n_{nb}$ matrix at time, $t_0$.
     * @param Omega_caret_n_nb The matrix at $T_0$.
     */
    void Omega_caret_n_nb(const KMatrix &t_0);

  protected:

  private:
    /**
     * The sample interval in seconds.
     */
    double dt;

    /**
     * $\mat{a^b}$
     */
    IMUVector a_caret_b;

    /**
     * $\mat{\Omega^b_{ib}}$
     */
    IMUVector Omega_caret_b_ib;

    IMUMatrix R_caret_n_b;

    IMUMatrix Omega_caret_n_nb;

    };
  };

/*  \@} */
#endif
