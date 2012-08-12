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
#include "Math/kalman/kalman/kmatrix.hpp"
#include "Math/kalman/kalman/kfilter.hpp"
#include "Math/kalman/kalman/ekfilter.hpp"
#include "Math/VectorT.hpp"
#include "test_debug.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

//using namespace std;
//using namespace Kalman;

//==============================================================================
class BasicCtor : public Kalman::EKFilter<fixed, false, false, true>
  {
  public:
    void aFunction();

  protected:
    void makeMeasure();
    void makeProcess();
  };

using namespace std;
using namespace Kalman;

void
BasicCtor:: makeMeasure()
  {
  }

void
BasicCtor::makeProcess()
  {
  }

void
BasicCtor::aFunction()
  {
  }

static bool
test_basic_ctor()
  {
  // See if the compiler can handle it.
  BasicCtor filter;

  filter.aFunction();

  return true;
  }

//==============================================================================
class Plane : public Kalman::EKFilter<double, false, true, false>
  {
  public:
    Plane();

    float uniform();
    float normal();
    bool NearEnough(double a, double b);

  protected:
    void makeBaseA();
    void makeBaseH();
    void makeBaseV();
    void makeBaseR();
    void makeBaseW();
    void makeBaseQ();

    void makeA();
    void makeH();
    void makeProcess();
    void makeMeasure();

    double Period, Mass, Bfriction, Portance, Gravity;
  };

typedef Plane::Vector Vector;
typedef Plane::Matrix Matrix;

Plane::Plane()
  {
  setDim(4, 1, 2, 2, 2);
  Period = 0.2;
  Gravity = 9.8;
  Bfriction = 0.35;
  Portance = 3.92;
  Mass = 1000;
  }

void Plane::makeBaseA()
  {
  A(0, 0) = 1.0;
  // A(0, 1) = Period - Period * Period * Bfriction / Mass * x(1);
  A(0, 2) = 0.0;
  A(0, 3) = 0.0;

  A(1, 0) = 0.0;
  // A(1, 1) = 1 - 2 * Period * Bfriction / Mass * x(1);
  A(1, 2) = 0.0;
  A(1, 3) = 0.0;

  A(2, 0) = 0.0;
  // A(2, 1) = Period * Period * Portance / Mass * x(1);
  A(2, 2) = 1.0;
  A(2, 3) = Period;

  A(3, 0) = 0.0;
  // A(3, 1) = 2 * Period * Portance / Mass * x(1);
  A(3, 2) = 0.0;
  A(3, 3) = 1.0;
  }

void Plane::makeA()
  {
  // A(0, 0) = 1.0;
  A(0, 1) = Period - Period * Period * Bfriction / Mass * x(1);
  // A(0, 2) = 0.0;
  // A(0, 3) = 0.0;

  // A(1, 0) = 0.0;
  A(1, 1) = 1 - 2 * Period * Bfriction / Mass * x(1);
  // A(1, 2) = 0.0;
  // A(1, 3) = 0.0;

  // A(2, 0) = 0.0;
  A(2, 1) = Period * Period * Portance / Mass * x(1);
  // A(2, 2) = 1.0;
  // A(2, 3) = Period;

  // A(3, 0) = 0.0;
  A(3, 1) = 2 * Period * Portance / Mass * x(1);
  // A(3, 2) = 0.0;
  // A(3, 3) = 1.0;
  }

void Plane::makeBaseW()
  {
  W(0, 0) = 0.0;
  W(0, 1) = 0.0;
  W(1, 0) = 1.0;
  W(1, 1) = 0.0;
  W(2, 0) = 0.0;
  W(2, 1) = 0.0;
  W(3, 0) = 0.0;
  W(3, 1) = 1.0;
  }

void Plane::makeBaseQ()
  {
  Q(0, 0) = 0.01 * 0.01;
  Q(0, 1) = 0.01 * 0.01 / 10.0;
  Q(1, 0) = 0.01 * 0.01 / 10.0;
  Q(1, 1) = 0.01 * 0.01;
  }

void Plane::makeBaseH()
  {
  // H(0, 0) = -x(2) / (x(0) * x(0) + x(2) * x(2));
  H(0, 1) = 0.0;
  // H(0, 2) = x(0) / (x(0) * x(0) + x(2) * x(2));
  H(0, 3) = 0.0;

  // H(1, 0) = x(0) / sqrt(x(0) * x(0) + x(2) * x(2));
  H(1, 1) = 0.0;
  // H(1, 2) = x(2) / sqrt(x(0) * x(0) + x(2) * x(2));
  H(1, 3) = 0.0;
  }

void Plane::makeH()
  {
  H(0, 0) = -x(2) / (x(0) * x(0) + x(2) * x(2));
  // H(0, 1) = 0.0;
  H(0, 2) = x(0) / (x(0) * x(0) + x(2) * x(2));
  // H(0, 3) = 0.0;

  H(1, 0) = x(0) / sqrt(x(0) * x(0) + x(2) * x(2));
  // H(1, 1) = 0.0;
  H(1, 2) = x(2) / sqrt(x(0) * x(0) + x(2) * x(2));
  // H(1, 3) = 0.0;
  }

void Plane::makeBaseV()
  {
  V(0, 0) = 1.0;
  V(1, 1) = 1.0;
  }

void Plane::makeBaseR()
  {
  R(0, 0) = 0.01 * 0.01;
  R(1, 1) = 50 * 50;
  }

void Plane::makeProcess()
  {
  Vector x_(x.size());
  x_(0) = x(0) + x(1) * Period + (Period * Period) / 2 *
          (u(0) / Mass - Bfriction / Mass * x(1) * x(1));
  x_(1) = x(1) + (u(0) / Mass - Bfriction / Mass * x(1) * x(1)) * Period;
  x_(2) = x(2) + x(3) * Period + (Period * Period) / 2 *
          (Portance / Mass * x(1) * x(1) - Gravity);
  x_(3) = x(3) + (Portance / Mass * x(1) * x(1) - Gravity) * Period;
  x.swap(x_);
  }

void Plane::makeMeasure()
  {
  z(0) = atan2(x(2), x(0));
  z(1) = sqrt(x(0) * x(0) + x(2) * x(2));
  }

// Simple uniform distribution of zero mean and unit variance
float Plane::uniform(void)
  {
  return((((float)rand())/(RAND_MAX-1) - 0.5f)* 3.464101615138f);
  }

// Simple approximation of normal dist. by the sum of uniform dist.
float Plane::normal(void)
  {
  int n = 6;
  int i;
  float temp = 0.0;

  for(i = 0; i < n; i++)
    temp += uniform();
  temp /= sqrt((float)n);
  return temp;
  }

bool Plane::NearEnough(double a, double b)
  {
  if (fabs(a - b) > 1e-5)
    return false;

  return true;
  }

static bool
test_sample_ekf()
  {
  std::ifstream dataInput;
  std::ifstream dataResult;
  std::string tmpStr;

  const unsigned NTRY = 500;
  const unsigned n = 4;	//nb states
  const unsigned m = 2;	//nb measures

  // Matlab format
  selectKVectorContext(createKVectorContext(" ", "[ ", " ];", 4));
  selectKMatrixContext(createKMatrixContext(" ", " ;\n  ", "[ ", " ];", 4));

  Plane filter;

  static const double _P0[] = {100.0 * 100.0, 0.0, 0.0, 0.0,
                               0.0, 10.0 * 10.0, 0.0, 0.0,
                               0.0, 0.0, 25.0 * 25.0, 0.0,
                               0.0, 0.0, 0.0, 10.0 * 10.0};

  Vector x(n);
  Matrix P0(n, n, _P0);

  Vector F(NTRY);
  Matrix Measure(m, NTRY);

  dataInput.open("test/src/data.m", std::ifstream::in);
  dataResult.open("test/src/trajectory_udu_load.m", std::ifstream::in);

  // Do not use ifstream::fail() because a preprocessor "fail" is defined
  // from within test_debug - very bad!
  if (dataInput.rdstate() & ios::failbit)
    {
    std::cout << "Unable to open input file!" << std::endl;
    return false;
    }

  if (dataResult.rdstate() & ios::failbit)
    {
    std::cout << "Unable to open output file!" << std::endl;
    return false;
    }

  std::cout << "Loading inputs and measures from file <data.m>." << std::endl;

  //Read the inputs vector. This vector have been generated by the Matlab script <generation.m>
  dataInput >> tmpStr;
  dataInput >> tmpStr;
  dataInput >> F;

  if (dataInput.rdstate() & ios::failbit)
    {
    std::cout << "IO error!" << std::endl;
    return false;
    }

  //Output as a Matlab colonn vector.  This will be used by the Matlab script <result.m>
  selectKVectorContext(createKVectorContext(";", "[ ", " ];", 4));
  selectKMatrixContext(createKMatrixContext(" ", " ;\n  ", "[ ", " ];", 4));


  //Read the measures matrix. This matrix have been generated by the Matlab script <generation.m>
  dataInput >> tmpStr;
  dataInput >> tmpStr;
  dataInput >> tmpStr;
  dataInput >> Measure;

  if (dataInput.rdstate() & ios::failbit)
    {
    std::cout << "IO error!" << std::endl;
    return false;
    }

  unsigned i = 0, j = 0;
  Vector z(m);

  //Initiale estimate
  std::cout << "angle: "
            << Measure(0, 0)
	    << " rayon: "
	    << Measure(1,0)
	    << std::endl;
  x(0) = cos(Measure(0, 0)) * Measure(1, 0);
  x(1) = 60;
  x(2) = sin(Measure(0, 0)) * Measure(1, 0);
  x(3) = 0;

  filter.init(x, P0);

  std::cout << "xp(" << ":," << i <<") = " << filter.getX() << std::endl;

  for (i = 1; i < NTRY; ++i)
    {
    // filter
    for(j = 0; j < m; j++)
      z(j) = Measure(j, i);

    Vector u(1, F(i));

    filter.step(u, z);

    Vector x(4);
    x = filter.getX();

    std::cout << "xp(" << ":," << i + 1 << ") = " << x << std::endl;
#if 0
    Vector result(4);
    dataResult >> result(0);
    dataResult >> result(1);
    dataResult >> result(2);
    dataResult >> result(3);
    std::cout << result << std::endl;
    for (unsigned k = 0; k < 4; k++)
      if (filter.NearEnough(result(k), x(k)) != true)
        return false;
#endif
    }


  dataResult.close();
  dataInput.close();
  return true;
  }

//------------------------------------------------------------------------------
int
main(int argc, const char *argv[])
  {
  plan_tests(2);

  ok(test_basic_ctor(), "basic ctor", 0);
  ok(test_sample_ekf(), "plane test", 0);

  return exit_status();
  }
