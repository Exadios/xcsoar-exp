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

#include <limits>
#include <float.h>
#include "Navigation/INU/Inu.hpp"

//-----------------------------------------------------------------------------
Inu::Inu(fixed dt)
  {
  this->dt = dt;
  this->init = false;
  }

//-----------------------------------------------------------------------------
Inu::~Inu()
  {
  }

//----------------------------------------------------------------------------
bool
Inu::Update(IMUmatrix &Omega, IMUvector &f, IMUvector &v, fixed gx)
  {
  // Write out the system in full, for clarity, and let the optimizer do
  // its work.
  this->a[0] = this->R[0][0] * f[0] +
               this->R[0][1] * f[1] +
	       this->R[0][2] * f[2] -
                fixed(2) * (Omega[0][0] * v[0] +
		            Omega[0][1] * v[1] +
			    Omega[0][2] * v[2]) +
	        gx; 
  this->a[1] = this->R[1][0] * f[1] +
               this->R[1][1] * f[1] +
	       this->R[1][2] * f[2] -
	       fixed(2) * (Omega[1][0] * v[0] +
	                   Omega[1][1] * v[1] +
			   Omega[1][2] * v[2]) +
	       gx;
  this->a[2] = this->R[2][0] * f[1] +
               this->R[2][1] * f[1] +
	       this->R[2][2] * f[2] -
	       fixed(2) * (Omega[2][0] * v[0] +
	                   Omega[2][1] * v[1] +
			   Omega[2][2] * v[2]) +
	       gx;

  IMUmatrix Num, Inv;
  Num[0][0] = fixed(2) + Omega[0][0] * this->dt;
  Num[0][1] = fixed(0) + Omega[0][1] * this->dt;
  Num[0][2] = fixed(0) + Omega[0][2] * this->dt;
  Num[1][0] = fixed(0) + Omega[1][0] * this->dt;
  Num[1][1] = fixed(2) + Omega[1][1] * this->dt;
  Num[1][2] = fixed(0) + Omega[1][2] * this->dt;
  Num[2][0] = fixed(0) + Omega[2][0] * this->dt;
  Num[2][1] = fixed(0) + Omega[2][1] * this->dt;
  Num[2][2] = fixed(0) + Omega[2][2] * this->dt;
  Inv[0][0] = fixed(2) - Omega[0][0] * this->dt;
  Inv[0][1] = fixed(0) - Omega[0][1] * this->dt;
  Inv[0][2] = fixed(0) - Omega[0][2] * this->dt;
  Inv[1][0] = fixed(0) - Omega[1][0] * this->dt;
  Inv[1][1] = fixed(2) - Omega[1][1] * this->dt;
  Inv[1][2] = fixed(0) - Omega[1][2] * this->dt;
  Inv[2][0] = fixed(0) - Omega[2][0] * this->dt;
  Inv[2][1] = fixed(0) - Omega[2][1] * this->dt;
  Inv[2][2] = fixed(0) - Omega[2][2] * this->dt;

  // Check for non sigularity.
  fixed D = Inv[0][0] * (Inv[1][1] * Inv[2][2] - Inv[2][1] * Inv[1][2]) -
            Inv[0][1] * (Inv[1][0] * Inv[2][2] - Inv[2][0] * Inv[1][2]) +
	    Inv[0][2] * (Inv[1][0] * Inv[2][1] - Inv[2][0] * Inv[1][2]);
  if ((double )fabs(D) < (10 * DBL_MIN))
    return false;  // Woops! A singularity or, at least, ill conditioned.
  
  // Compute the transform of the cofactors, C^T.
  IMUmatrix CT;
  CT[0][0] = ( Inv[1][1] * Inv[2][2] - Inv[2][1] * Inv[1][2]) / D;
  CT[1][0] = (-Inv[1][0] * Inv[2][2] + Inv[2][0] * Inv[1][2]) / D;
  CT[2][0] = ( Inv[1][0] * Inv[2][1] - Inv[2][0] * Inv[1][1]) / D;
  CT[0][1] = (-Inv[0][1] * Inv[2][2] + Inv[2][1] * Inv[0][2]) / D;
  CT[1][1] = ( Inv[0][0] * Inv[2][2] - Inv[2][0] * Inv[0][2]) / D;
  CT[2][1] = (-Inv[0][0] * Inv[2][1] + Inv[2][0] * Inv[0][1]) / D;
  CT[0][2] = ( Inv[0][1] * Inv[1][2] - Inv[1][1] * Inv[0][2]) / D;
  CT[1][2] = (-Inv[0][0] * Inv[2][2] + Inv[1][0] * Inv[0][2]) / D;
  CT[2][2] = ( Inv[0][0] * Inv[1][1] - Inv[1][0] * Inv[0][1]) / D;

  // Compute the Pade approximation.
  IMUmatrix P;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      P[i][j] = Num[i][0] * CT[0][j] + 
                Num[i][1] * CT[1][j] + 
		Num[i][2] * CT[2][j];

  // Compute the update of R.
  IMUmatrix R;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      R[i][j] = this->R[i][0] * P[0][j] +
                this->R[i][1] * P[1][j] +
		this->R[i][2] * P[2][j];
  this->R = R;

  return true;
  }

//------------------------------------------------------------------------------
#define G_0 fixed(9.80665)   // m/s^2
#define R_m fixed(6371009)  // m
fixed
Inu::gfh(fixed h) const
  {
  return fixed(G_0) * (R_m / (h + R_m)) * (R_m / (h + R_m));
  }
  
//------------------------------------------------------------------------------
IMUvector::IMUvector()
  {
  for (int i = 0; i < 3; i++)
    this->v[i] = fixed(0.0);
  }

//------------------------------------------------------------------------------
IMUvector::IMUvector(fixed x0, fixed x1, fixed x2)
  {
  this->v[0] = x0;
  this->v[1] = x1;
  this->v[2] = x2;
  }

//------------------------------------------------------------------------------
IMUvector::IMUvector(const IMUvector& rhs)
  {
  for (int i = 0; i < 3; i++)
    this->v[i] = rhs[i];
  }

//------------------------------------------------------------------------------
IMUvector::~IMUvector()
  {
  }

//------------------------------------------------------------------------------
IMUvector&
IMUvector::operator=(const IMUvector& rhs)
  {
  for (int i = 0; i < 3; i++)
    this->v[i] = rhs[i];
  return *this;
  }

//------------------------------------------------------------------------------
fixed&
IMUvector::operator[](size_t i)
  {
  return this->v[i];
  }

//------------------------------------------------------------------------------
fixed
IMUvector::operator[](size_t i) const
  {
  return this->v[i];
  }

//------------------------------------------------------------------------------
IMUmatrix::IMUmatrix()
  {
  for (int i = 0; i < 3; i++)
    this->a[i] = IMUvector(fixed(0.0), fixed(0.0), fixed(0.0));
  }

//------------------------------------------------------------------------------
IMUmatrix::IMUmatrix(const IMUmatrix& rhs)
  {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      this->a[i][j] = rhs[i][j];
  }

//------------------------------------------------------------------------------
IMUmatrix::~IMUmatrix()
  {
  }

//------------------------------------------------------------------------------
IMUmatrix&
IMUmatrix::operator=(const IMUmatrix& rhs)
  {
  for (int i = 0; i < 3; i++)
    this->a[i] = rhs[i];
  return *this;
  }

//------------------------------------------------------------------------------
IMUvector&
IMUmatrix::operator[](size_t i)
  {
  return this->a[i];
  }

//------------------------------------------------------------------------------
IMUvector
IMUmatrix::operator[](size_t i) const
  {
  return this->a[i];
  }

