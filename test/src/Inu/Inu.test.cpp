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

#include <iostream>

#include "../../../src/Engine/Navigation/INU/Inu.hpp"

int main()
  {
  int test = 0;
    {
    // Try contructing a Inu object.
    std::cout << "Test " << ++test << std::endl;
    Inu inu(0.005);
    }

    {
    std::cout << "Test " << ++test << std::endl;
    IMUmatrix a, b;
    }

    {
    std::cout << "Test " << ++test << std::endl;
    Inu inu(.1);
    IMUvector f, v, a, w;
    f.resize(3);
    v.resize(3);
    w.resize(3);
    for (size_t i = 0; i < 10; i++)
      {
      if (inu.Update(w, f, v, inu.gfh(0)) == true)
        {
        a = inu.v_dot_super_e();
        std::cout << i << ": ";
        a.put(std::cout);
        std::cout << std::endl;
        }
      else
        {
        std::cout << "Singular" << std::endl;
        break;
        }
      }
    }

    {
    std::cout << "Test " << ++test << std::endl;
    Inu inu(.1);
    IMUvector f, v, a, w;
    IMUmatrix Omega;
    f.resize(3);
    v.resize(3);
    w.resize(3);
    f(2) = f(1) = f(0) = 1.0;
    if (inu.Update(w, f, v, inu.gfh(0)) == true)
      {
      a = inu.v_dot_super_e();
      a.put(std::cout);
      std::cout << std::endl;
      }
    else
      std::cout << "Singular" << std::endl;
    }

    {
    std::cout << "Test " << ++test << std::endl;
    Inu inu(.1);
    IMUvector f, v, a, w;
    IMUmatrix Omega;
    f.resize(3);
    v.resize(3);
    w.resize(3);
    f(2) = f(1) = f(0) = 1.0;
    for (int i = 0; i < 10; i++)
      {
      if (inu.Update(w, f, v, inu.gfh(0)) == true)
        {
        a = inu.v_dot_super_e();
        std::cout << i << ": ";
        a.put(std::cout);
        std::cout << std::endl;
        }
      else
        {
        std::cout << "Singular" << std::endl;
        break;
        }
      f(2) = f(1) = f(0) = 0.0;
      }
    }

  return 0;
  }
