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

#include <sstream>
#include <iostream>
#include "eqn-expansion.hpp"

typedef MatrixT<Term, 8, 8> M;

int main(int argc, const char *argv[])
  {
  Term zero("0");
  Term one("1");
  Term dT("dT");
  M A, P, At, R;

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      {
      A[i][j] = zero;
      At[i][j] = zero;
      }

  for (int i = 0; i < 8; i++)
    {
    A[i][i] = one;
    At[i][i] = one;
    }

  for (int i = 0; i < 4; i++)
    {
    A[i][i + 4] = dT;
    At[i + 4][i] = dT;
    }

  for (int i = 0; i < 8; i++)
    {
    for (int j = 0; j < 8; j++)
      {
      std::stringstream s;
      s << "P[" << i << "][" << j << "]";
      Term name(s.str());
      P[i][j] = name;
      }
    }

  R = A * P * At;

  for (size_t i = 0; i < 8; i++)
    for (size_t j = 0; j < 8; j++)
      std::cout << R[i][j] << std::endl;
  return 0;
  }
