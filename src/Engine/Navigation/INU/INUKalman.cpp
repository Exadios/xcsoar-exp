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

  INUSystemMatrix R;

  R[0][0] = P[0][0];
  R[1][1] = P[1][1];
  R[2][2] = P[2][2];
  R[3][3] = P[3][3];
  R[4][4] = P[4][4];
  R[5][5] = P[5][5];
  R[6][6] = P[6][6];
  R[7][7] = P[7][7];

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

