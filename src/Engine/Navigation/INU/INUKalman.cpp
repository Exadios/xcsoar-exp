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
#include "Engine/Navigation/INU/Inu.hpp"
#include "Engine/Navigation/INU/INUKalman.hpp"

//------------------------------------------------------------------------------
INUKalman::INUKalman(fixed dT, int m)
  : dT(dT),
    m(m),
    s(0)
  {
  this->DT = this->m * this->dT;
  }

//------------------------------------------------------------------------------
INUKalman::~INUKalman()
  {
  }

//------------------------------------------------------------------------------
void
INUKalman::Initialize(const INUKalmanState& x, const INUSystemMatrix& P)
  {
  this->x = x;
  this->P = P;
  }

// Arbitary gyro and accelerometer drift and bias aging rates. Defined here
// temperally.
#define QG fixed(0.9)
#define QA fixed(0.9)

//------------------------------------------------------------------------------
INUKalmanState&
INUKalman::Innovate(const INUUpdate& u)
  {
  this->x[0] += this->x[4] * this->dT;
  this->x[1] += this->x[5] * this->dT;
  this->x[2] += this->x[6] * this->dT;
  this->x[3] += this->x[7] * this->dT;
  this->x[4] += u[0] * this->dT;
  this->x[5] += u[1] * this->dT;
  this->x[6] += u[2] * this->dT;
  if (this->s == this->m)
    {
    this->x[4]  += this->x[8]  * this->DT + this->x[11] * this->DT * this->DT;
    this->x[5]  += this->x[9]  * this->DT + this->x[12] * this->DT * this->DT;
    this->x[6]  += this->x[10] * this->DT + this->x[13] * this->DT * this->DT;
    this->x[8]  += this->x[8]  * QG * this->DT;
    this->x[9]  += this->x[9]  * QG * this->DT;
    this->x[10] += this->x[10] * QG * this->DT;
    this->x[11] += this->x[11] * QA * this->DT;
    this->x[12] += this->x[12] * QA * this->DT;
    this->x[13] += this->x[13] * QA * this->DT;
    }
  else
    {
    }

  // I am unsure of the APA^T expansion below. It does not look correct to my
  // eye. Needs confirmation by manual checking of a sample.
  INUSystemMatrix R;
  R[0][0] = this->P[0][0] + this->dT * this->P[4][0] + this->P[0][1] +
            this->P[0][2] + this->P[0][3] + this->P[0][4] + 
	    this->dT * this->P[4][4] * this->dT + this->P[0][5] +
	    this->P[0][6] + this->P[0][7];
  R[0][1] = this->P[0][0] + this->P[0][1] + this->dT * this->P[4][1] +
            this->P[0][2] + this->P[0][3] + this->P[0][4] + this->P[0][5] +
	    this->dT * this->P[4][5] * this->dT + this->P[0][6] + this->P[0][7];
  R[0][2] = this->P[0][0] + this->P[0][1] + this->P[0][2] + 
            this->dT * this->P[4][2] + this->P[0][3] + this->P[0][4] +
	    this->P[0][5] + this->P[0][6] + this->dT * this->P[4][6] *
	    this->dT + this->P[0][7];
  R[0][3] = this->P[0][0] + this->P[0][1] + this->P[0][2] + this->P[0][3] +
            this->dT * this->P[4][3] + this->P[0][4] + this->P[0][5] +
	    this->P[0][6] + this->P[0][7] + this->dT * this->P[4][7] * this->dT;
  R[0][4] = this->P[0][0] + this->P[0][1] + this->P[0][2] + this->P[0][3] +
            this->P[0][4] + this->dT * this->P[4][4] + this->P[0][5] +
	    this->P[0][6] + this->P[0][7];
  R[0][5] = this->P[0][0] + this->P[0][1] + this->P[0][2] + this->P[0][3] +
            this->P[0][4] + this->P[0][5] + this->dT * this->P[4][5] + 
	    this->P[0][6] + this->P[0][7];
  R[0][6] = this->P[0][0] + this->P[0][1] + this->P[0][2] + this->P[0][3] +
            this->P[0][4] + this->P[0][5] + this->P[0][6] + 
	    this->dT * this->P[4][6] + this->P[0][7];
  R[0][7] = this->P[0][0] + this->P[0][1] + this->P[0][2] + this->P[0][3] +
            this->P[0][4] + this->P[0][5] + this->P[0][6] + this->P[0][7] +
	    this->dT * this->P[4][7];
  R[1][0] = this->P[1][0] + this->dT * this->P[5][0] + this->P[1][1] + 
            this->P[1][2] + this->P[1][3] + this->P[1][4] + 
	    this->dT * this->P[5][4] * this->dT + this->P[1][5] +
	    this->P[1][6] + this->P[1][7];
  R[1][1] = this->P[1][0] + this->P[1][1] + this->dT * this->P[5][1] + 
            this->P[1][2] + this->P[1][3] + this->P[1][4] + this->P[1][5] + 
	    this->dT * this->P[5][5] * this->dT + this->P[1][6] + this->P[1][7];
  R[1][2] = this->P[1][0] + this->P[1][1] + this->P[1][2] + 
            this->dT * this->P[5][2] + this->P[1][3] + this->P[1][4] +
	    this->P[1][5] + this->P[1][6] +
	    this->dT * this->P[5][6] * this->dT + this->P[1][7];
  R[1][3] = this->P[1][0] + this->P[1][1] + this->P[1][2] + this->P[1][3] +
            this->dT * this->P[5][3] + this->P[1][4] + this->P[1][5] +
	    this->P[1][6] + this->P[1][7] + this->dT * this->P[5][7] * this->dT;
  R[1][4] = this->P[1][0] + this->P[1][1] + this->P[1][2] + this->P[1][3] + 
            this->P[1][4] + this->dT * this->P[5][4] + this->P[1][5] + 
	    this->P[1][6] + this->P[1][7];
  R[1][5] = this->P[1][0] + this->P[1][1] + this->P[1][2] + this->P[1][3] + 
            this->P[1][4] + this->P[1][5] + this->dT * this->P[5][5] + 
	    this->P[1][6] + this->P[1][7];
  R[1][6] = this->P[1][0] + this->P[1][1] + this->P[1][2] + this->P[1][3] + 
            this->P[1][4] + this->P[1][5] + this->P[1][6] + 
	    this->dT * this->P[5][6] + this->P[1][7];
  R[1][7] = this->P[1][0] + this->P[1][1] + this->P[1][2] + this->P[1][3] + 
            this->P[1][4] + this->P[1][5] + this->P[1][6] + this->P[1][7] + 
	    this->dT * this->P[5][7];
  R[2][0] = this->P[2][0] + this->dT * this->P[6][0] + this->P[2][1] + 
            this->P[2][2] + this->P[2][3] + this->P[2][4] + 
	    this->dT * this->P[6][4] * this->dT + this->P[2][5] + 
	    this->P[2][6] + this->P[2][7];
  R[2][1] = this->P[2][0] + this->P[2][1] + this->dT * this->P[6][1] + 
            this->P[2][2] + this->P[2][3] + this->P[2][4] + this->P[2][5] +
	    this->dT * this->P[6][5] * this->dT + this->P[2][6] + this->P[2][7];
  R[2][2] = this->P[2][0] + this->P[2][1] + this->P[2][2] +
            this->dT * this->P[6][2] + this->P[2][3] + this->P[2][4] +
	    this->P[2][5] + this->P[2][6] + 
	    this->dT * this->P[6][6] * this->dT + this->P[2][7];
  R[2][3] = this->P[2][0] + this->P[2][1] + this->P[2][2] + this->P[2][3] +
            this->dT * this->P[6][3] + this->P[2][4] + this->P[2][5] +
	    this->P[2][6] + this->P[2][7] + this->dT * this->P[6][7] * this->dT;
  R[2][4] = this->P[2][0] + this->P[2][1] + this->P[2][2] + this->P[2][3] + 
            this->P[2][4] + this->dT * this->P[6][4] + this->P[2][5] + 
	    this->P[2][6] + this->P[2][7];
  R[2][5] = this->P[2][0] + this->P[2][1] + this->P[2][2] + this->P[2][3] + 
            this->P[2][4] + this->P[2][5] + this->dT * this->P[6][5] + 
	    this->P[2][6] + this->P[2][7];
  R[2][6] = this->P[2][0] + this->P[2][1] + this->P[2][2] + this->P[2][3] + 
            this->P[2][4] + this->P[2][5] + this->P[2][6] + 
	    this->dT * this->P[6][6] + this->P[2][7];
  R[2][7] = this->P[2][0] + this->P[2][1] + this->P[2][2] + this->P[2][3] + 
            this->P[2][4] + this->P[2][5] + this->P[2][6] + this->P[2][7] + 
	    this->dT * this->P[6][7];
  R[3][0] = this->P[3][0] + this->dT * this->P[7][0] + this->P[3][1] + 
            this->P[3][2] + this->P[3][3] + this->P[3][4] + 
	    this->dT * this->P[7][4] * this->dT + this->P[3][5] + 
	    this->P[3][6] + this->P[3][7];
  R[3][1] = this->P[3][0] + this->P[3][1] + this->dT * this->P[7][1] + 
            this->P[3][2] + this->P[3][3] + this->P[3][4] + this->P[3][5] + 
	    this->dT * this->P[7][5] * this->dT + this->P[3][6] + this->P[3][7];
  R[3][2] = this->P[3][0] + this->P[3][1] + this->P[3][2] + 
            this->dT * this->P[7][2] + this->P[3][3] + this->P[3][4] + 
	    this->P[3][5] + this->P[3][6] + 
	    this->dT * this->P[7][6] * this->dT + this->P[3][7];
  R[3][3] = this->P[3][0] + this->P[3][1] + this->P[3][2] + this->P[3][3] + 
            this->dT * this->P[7][3] + this->P[3][4] + this->P[3][5] + 
	    this->P[3][6] + this->P[3][7] + 
	    this->dT * this->P[7][7] * this->dT;
  R[3][4] = this->P[3][0] + this->P[3][1] + this->P[3][2] + this->P[3][3] + 
            this->P[3][4] + this->dT * this->P[7][4] + this->P[3][5] + 
	    this->P[3][6] + this->P[3][7];
  R[3][5] = this->P[3][0] + this->P[3][1] + this->P[3][2] + this->P[3][3] + 
            this->P[3][4] + this->P[3][5] + this->dT * this->P[7][5] + 
	    this->P[3][6] + this->P[3][7];
  R[3][6] = this->P[3][0] + this->P[3][1] + this->P[3][2] + this->P[3][3] + 
            this->P[3][4] + this->P[3][5] + this->P[3][6] + 
	    this->dT * this->P[7][6] + this->P[3][7];
  R[3][7] = this->P[3][0] + this->P[3][1] + this->P[3][2] + this->P[3][3] + 
            this->P[3][4] + this->P[3][5] + this->P[3][6] + this->P[3][7] + 
	    this->dT * this->P[7][7];
  R[4][0] = this->P[4][0] + this->P[4][1] * this->P[4][2] * this->P[4][3] *
            this->P[4][4] * this->dT;
  R[4][1] = this->P[4][0] * this->P[4][1] + this->P[4][2] * this->P[4][3] *
            this->P[4][4] * this->P[4][5] * this->dT;
  R[4][2] = this->P[4][0] * this->P[4][1] * this->P[4][2] + this->P[4][3] *
            this->P[4][4] * this->P[4][5] * this->P[4][6] * this->dT;
  R[4][3] = this->P[4][0] * this->P[4][1] * this->P[4][2] * this->P[4][3] +
            this->P[4][4] * this->P[4][5] * this->P[4][6] * this->P[4][7] *
	    this->dT;
  R[4][4] = this->P[4][0] * this->P[4][1] * this->P[4][2] * this->P[4][3] *
            this->P[4][4];
  R[4][5] = this->P[4][0] * this->P[4][1] * this->P[4][2] * this->P[4][3] *
            this->P[4][4] * this->P[4][5];
  R[4][6] = this->P[4][0] * this->P[4][1] * this->P[4][2] * this->P[4][3] *
            this->P[4][4] * this->P[4][5] * this->P[4][6];
  R[4][7] = this->P[4][0] * this->P[4][1] * this->P[4][2] * this->P[4][3] *
            this->P[4][4] * this->P[4][5] * this->P[4][6] * this->P[4][7];
  R[5][0] = this->P[5][0] + this->P[5][1] * this->P[5][2] * this->P[5][3] *
            this->P[5][4] * this->dT;
  R[5][1] = this->P[5][0] * this->P[5][1] + this->P[5][2] * this->P[5][3] * 
            this->P[5][4] * this->P[5][5] * this->dT;
  R[5][2] = this->P[5][0] * this->P[5][1] * this->P[5][2] + this->P[5][3] *
            this->P[5][4] * this->P[5][5] * this->P[5][6] * this->dT;
  R[5][3] = this->P[5][0] * this->P[5][1] * this->P[5][2] * this->P[5][3] +
            this->P[5][4] * this->P[5][5] * this->P[5][6] * this->P[5][7] *
	    this->dT;
  R[5][4] = this->P[5][0] * this->P[5][1] * this->P[5][2] * this->P[5][3] * 
            this->P[5][4];
  R[5][5] = this->P[5][0] * this->P[5][1] * this->P[5][2] * this->P[5][3] *
            this->P[5][4] * this->P[5][5];
  R[5][6] = this->P[5][0] * this->P[5][1] * this->P[5][2] * this->P[5][3] *
            this->P[5][4] * this->P[5][5] * this->P[5][6];
  R[5][7] = this->P[5][0] * this->P[5][1] * this->P[5][2] * this->P[5][3] *
            this->P[5][4] * this->P[5][5] * this->P[5][6] * this->P[5][7];
  R[6][0] = this->P[6][0] + this->P[6][1] * this->P[6][2] * this->P[6][3] *
            this->P[6][4] * this->dT;
  R[6][1] = this->P[6][0] * this->P[6][1] + this->P[6][2] * this->P[6][3] *
            this->P[6][4] * this->P[6][5] * this->dT;
  R[6][2] = this->P[6][0] * this->P[6][1] * this->P[6][2] + this->P[6][3] *
            this->P[6][4] * this->P[6][5] * this->P[6][6] * this->dT;
  R[6][3] = this->P[6][0] * this->P[6][1] * this->P[6][2] * this->P[6][3] +
            this->P[6][4] * this->P[6][5] * this->P[6][6] * this->P[6][7] *
	    this->dT;
  R[6][4] = this->P[6][0] * this->P[6][1] * this->P[6][2] * this->P[6][3] *
            this->P[6][4];
  R[6][5] = this->P[6][0] * this->P[6][1] * this->P[6][2] * this->P[6][3] *
            this->P[6][4] * this->P[6][5];
  R[6][6] = this->P[6][0] * this->P[6][1] * this->P[6][2] * this->P[6][3] *
            this->P[6][4] * this->P[6][5] * this->P[6][6];
  R[6][7] = this->P[6][0] * this->P[6][1] * this->P[6][2] * this->P[6][3] *
            this->P[6][4] * this->P[6][5] * this->P[6][6] * this->P[6][7];
  R[7][0] = this->P[7][0] + this->P[7][1] * this->P[7][2] * this->P[7][3] *
            this->P[7][4] * this->dT;
  R[7][1] = this->P[7][0] * this->P[7][1] + this->P[7][2] * this->P[7][3] *
            this->P[7][4] * this->P[7][5] * this->dT;
  R[7][2] = this->P[7][0] * this->P[7][1] * this->P[7][2] + this->P[7][3] *
            this->P[7][4] * this->P[7][5] * this->P[7][6] * this->dT;
  R[7][3] = this->P[7][0] * this->P[7][1] * this->P[7][2] * this->P[7][3] +
            this->P[7][4] * this->P[7][5] * this->P[7][6] * this->P[7][7] *
	    this->dT;
  R[7][4] = this->P[7][0] * this->P[7][1] * this->P[7][2] * this->P[7][3] *
            this->P[7][4];
  R[7][5] = this->P[7][0] * this->P[7][1] * this->P[7][2] * this->P[7][3] *
            this->P[7][4] * this->P[7][5];
  R[7][6] = this->P[7][0] * this->P[7][1] * this->P[7][2] * this->P[7][3] *
            this->P[7][4] * this->P[7][5] * this->P[7][6];
  R[7][7] = this->P[7][0] * this->P[7][1] * this->P[7][2] * this->P[7][3] *
            this->P[7][4] * this->P[7][5] * this->P[7][6] * this->P[7][7];
  this->P = R + this->Q;
  return this->x;
  }

//------------------------------------------------------------------------------
void
INUKalman::Update(const INUObservation& z)
  {
  this->sl.Lock();
  s = 0;
  this->sl.Unlock();



  }

//------------------------------------------------------------------------------
void
INUKalman::Step(const INUUpdate& u, const INUObservation& z)
  {
  this->Innovate(u);
  this->Update(z);
  }

//------------------------------------------------------------------------------
const INUKalmanState&
INUKalman::State() const
  {
  return this->x;
  }

