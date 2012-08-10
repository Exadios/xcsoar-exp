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

class BasicCtor : public Kalman::EKFilter<fixed, false, false, true>
  {
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

static bool
test_basic_ctor()
  {
  // See if the compiler can handle it.
  BasicCtor filter;

  return true;
  }

int
main(int argc, const char *argv[])
  {
  plan_tests(1);

  ok(test_basic_ctor(), "basic ctor", 0);

  return exit_status();
  }
