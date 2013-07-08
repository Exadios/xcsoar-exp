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
 /**
 * \addtogroup test_Inu
 * @{
 */

#include <iostream>
#include "Imu.hpp"
#include "Math/InuMatrix.hpp"
#include "Math/InuVector.hpp"

//------------------------------------------------------------------------------
InuSimulator::Imu::Imu(double dt)
  {
  this->dt = dt;
  }

//------------------------------------------------------------------------------
InuSimulator::Imu::~Imu()
  {
  }

//------------------------------------------------------------------------------
void
InuSimulator::Imu::Update()
  {
  }

//------------------------------------------------------------------------------
InuVector &
InuSimulator::Imu::Accelerometer() const
  {
  InuMatrix dot_R_caret_b_n;
  dot_R_caret_b_n = this->R_caret_b_n * this->Omega_caret_n_bn;
  this->R_caret_b_n = this->R_caret_n_b + dot_R_caret_b_n;
  InuMatrix Omega_caret_n_ie;
  Omega_caret_n_ie = this->R_caret_n_e * this->Omega_caret_e_ie * this->R_caret_e_n;
  this->a_caret_b = this->R_caret_b_n * (this->a_caret_n - (this->Omega_caret_n_en + 2 * Omega_caret_n_ie) * this->v_caret_n + this->g_caret_n);
  return this->a_caret_b;
  }

//------------------------------------------------------------------------------
InuVector &
InuSimulator::Imu::Gyro() const
  {
  return this->Omega_caret_b_ib;
  }

//------------------------------------------------------------------------------
void
InuSimulator::Imu::R_caret_b_n(const KMatrix &t_0)
  {
  this->R_caret_b_n   = t_0;
  }

//------------------------------------------------------------------------------
void
InuSimulator::Imu::Omega_caret_n_nb(const KMatrix &Omega_caret_n_nb)
  {
  this->Omega_caret_n_nb = t_0;
  }
