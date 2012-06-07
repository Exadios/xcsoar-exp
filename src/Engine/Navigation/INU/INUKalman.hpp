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
#ifndef KALMAN_HPP
#define KALMAN_HPP

#include <Math/fixed.hpp>
#include <Math/VectorT.hpp>
#include <Math/MatrixT.hpp>
#include "IMUmatrix.hpp"

/**
 * A class designed to take INU and GPS data and merge those two data in an
 * optimal manner.
 */

typedef VectorT<fixed, 6> INUKalmanState;
typedef VectorT<fixed, 3> INUUpdate;

class INUKalman 
  {
  public:
    /**
     * Ctor.
     */
    INUKalman();

    /**
     * Dtor.
     */
    virtual ~INUKalman();

    /**
     * Initialization. Set the system state to \f$t_0\f$.
     */
    void Initialize();

    /**
     * Inovate. Compute \f$\hat{x} \left(t_k | t_{k - 1} \right)\f$ and 
     * \f$P \left( (t_k | t_{k - 1} \right)\f$.
     * @param u The control input to the plant model.
     * @return The predicted state vector.
     */
    INUKalmanState& Inovate(const INUUpdate& u);

    /**
     * Update. Compute \f$K \left( t_k \right) \f$,
     * \f$P \left( t_k | t_k \right ) \f$ and
     * \f$\hat{x} \left( t_k | t_k \right) \f$.
     */
    void Update();

  private:
    /**
     * No copy Ctor.
     */
    INUKalman(const INUKalman &rhs);

    /**
     * No copy operator.
     */
    INUKalman &operator=(const INUKalman &rhs);
    INUKalman operator=(const INUKalman &rhs) const;
  };
#endif // KALMAN_HPP
