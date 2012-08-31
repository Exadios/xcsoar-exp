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

typedef MatrixT<Term, 3, 3> M;

int main(int argc, const char *argv[])
  {
  M A, B, R;

  for (int i = 0; i < 3; i++)
    {
    for (int j = 0; j < 3; j++)
      {
      std::stringstream s;
      s << "A" << i << j;
      Term name(s.str());
      A[i][j] = name;
      }
    }

   for (int i = 0; i < 3; i++)
    {
    for (int j = 0; j < 3; j++)
      {
      std::stringstream s;
      s << "B" << i << j;
      Term name(s.str());
      B[i][j] = name;
      }
    }


  R = A * B;

  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      std::cout << "R" << i << j << " = " << R[i][j] << std::endl;
  return 0;
  }
