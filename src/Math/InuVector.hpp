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
}*/

#ifndef _INUVECTOR_HPP
#define _INUVECTOR_HPP
/**
 * \addtogroup Math
 * @{
 */

#include "Math/kalman/kalman/kvector.hpp"

/**
 * This class implements some operation on the KMatrix class.
 */
class InuVector : public Kalman::KVector<double, false>
  {
public:
  /**
   * Ctor.
   */
  InuVector();

  /**
   * Dtor.
   */
  virtual ~InuVector();

private:

  friend InuVector operator+(const InuVector&, const InuVector&);
  friend InuVector operator-(const InuVector&, const InuVector&);
  friend InuVector operator^(const InuVector&, const InuVector&);
  friend double    operator*(const InuVector&, const InuVector&);
  };

/**
 * Add two INU vectors.
 * @param x LH vector.
 * @param y RH vector.
 * @return The result of \f$\bf{x} + \bf{y}\f$.
 */
InuVector operator+(const InuVector&, const InuVector&);

/**
 * Subtract two INU vectors.
 * @param x LH vector.
 * @param y RH vector.
 * @return The result of \f$\bf{x} - \bf{y}\f$.
 */
InuVector operator-(const InuVector&, const InuVector&);

/**
 * Form the cross product of two INU vectors.
 * @param x LH vector.
 * @param y RH vector.
 * @return The result of \f$\bf{x} \times \bf{y}\f$.
 */
InuVector operator^(const InuVector&, const InuVector&);

/**
 * Form the dot product of two INU vectors.
 * @param x LH vector.
 * @param y RH vector.
 * @return The result of \f$\bf{x} \cdot \bf{y}\f$.
 */
double    operator*(const InuVector&, const InuVector&);

/**
 * @}
 */
#endif  // _INUVECTOR_HPP
