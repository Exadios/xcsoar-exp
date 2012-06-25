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

#include "Math/fixed.hpp"
#include "Math/MatrixT.hpp"
#include "Math/VectorT.hpp"
#include <iostream>

bool test_basic_ctor();
bool test_addition();
bool test_subtraction();
bool test_multiplication();
bool test_matrix_vector_multiplication();

bool
test_basic_ctor()
  {
  MatrixT<fixed, 3, 3> A;
  MatrixT<fixed, 3, 3> B;
  MatrixT<fixed, 3, 3> C;
  B = A;
  C = A + B;
  return true;
  }

bool
test_addition()
  {
  MatrixT<size_t, 3, 3> A;
  MatrixT<size_t, 3, 3> B;
  MatrixT<size_t, 3, 3> C;
  size_t i, j;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      A[i][j] = j + 3 * i;
      B[i][j] = j + 3 * i + 10;
      }
  C = A + B;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      if (C[i][j] != j + 3 * i + j + 3 * i + 10)
        return false;
      }
  return true;
  }

bool
test_subtraction()
  {
  MatrixT<size_t, 3, 3> A;
  MatrixT<size_t, 3, 3> B;
  MatrixT<size_t, 3, 3> C;
  size_t i, j;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      A[i][j] = j + 3 * i + 10;
      B[i][j] = j + 3 * i;
      }
  C = A - B;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      if (C[i][j] != j + 3 * i + 10 - (j + 3 * i))
        return false;
      }
  return true;
  }

bool
test_multiplication()
  {
  MatrixT<size_t, 3, 3> A;
  MatrixT<size_t, 3, 3> B;
  MatrixT<size_t, 3, 3> C;
  size_t i, j, k;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      A[i][j] = j + 3 * i;
      B[i][j] = j + 3 * i;
      }
  C = A * B;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      {
      size_t acc = 0;
      for (k = 0; k < 3; k++)
	acc += (3 * i + k) * (3 * k + j);
      if (C[i][j] != acc)
        return false;
      }

  return true;
  }

bool
test_matrix_vector_multiplication()
  {
  MatrixT<size_t, 3, 3> A;
  VectorT<size_t, 3>    B;
  VectorT<size_t, 3>    C;
  size_t i, j;

  for (i = 0; i < 3; i++)
    {
    for (j = 0; j < 3; j++)
      A[i][j] = j + 3 * i;
    B[i] = i;
    }
  C = A * B;

  for (i = 0; i < 3; i++)
    {
    size_t acc = 0;
    for (j = 0; j < 3; j++)
      acc += (j + 3 * i) * i;
    if (C[i] != acc)
      return false;
    }
  
  return true;
  }

int
main(int argc, const char *argv[])
  {
  if (test_basic_ctor() == false)
    return 1;
  if (test_addition() == false)
    return 2;
  if (test_subtraction() == false)
    return 3;
  if (test_multiplication() == false)
    return 4;

  return 0;
  }
