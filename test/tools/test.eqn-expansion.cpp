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

#include "eqn-expansion.hpp"
#include <sstream>
#include <iostream>

bool
basic_operation()
  {
  Term A;

  Term B = A;
  std::stringstream s;
  s << "P[0][0]";
  Term D(s.str());
  std::cout << "D = " << D << std::endl;

  Term C = A - B;

  return true;
  }

bool
operators()
  {
  Term P00("P[0][0]"), P01("p[0][1]"), R;

  R = P00 + P01;
  std::cout << "R = " << R << std::endl;
  return true;
  }
int main()
  {
  if (basic_operation() == false)
    return 1;
  if (operators() == false)
    return 2;

  return 0;
  }
