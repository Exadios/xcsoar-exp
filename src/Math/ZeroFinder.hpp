// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include <cassert>

/**
 * Zero finding and minimisation search algorithm
 *
 * Can be given initial value as hint of solution.
 *
 */
class ZeroFinder {
protected:
  /** min value of search range */
  const double xmin;
  /** max value of search range */
  const double xmax;
  /** search tolerance in x */
  const double tolerance;

public:
  /**
   * Constructor of zero finder search algorithm
   *
   * @param _xmin Minimum allowable value of x
   * @param _xmax Maximum allowable value of x
   * @param _tolerance Absolute tolerance of solution (in x)
   */
  constexpr ZeroFinder(double _xmin, double _xmax, double _tolerance) noexcept
    :xmin(_xmin), xmax(_xmax), tolerance(_tolerance)
  {
    assert(xmin < xmax);
  }

  /**
   * Abstract method for function to be minimised or root-solved
   *
   * @param x Value of x
   *
   * @return f(x)
   */
  virtual double f(const double x) noexcept = 0;

  /**
   * Find closest value of x that produces f(x)=0
   * Method used is a variant of a bisector search.
   * To enforce search, set xstart outside range 
   *
   * @param xstart Initial guess of x
   *
   * @return x value of best solution
   */
  [[gnu::pure]]
  double find_zero(double xstart) noexcept;

  /**
   * Find value of x that minimises f(x)
   * Method used is a variant of a bisector search.
   * To enforce search, set xstart outside range 
   *
   * @param xstart Initial guess of x
   *
   * @return x value of best solution
   */
  [[gnu::pure]]
  double find_min(double xstart) noexcept;

private:
  [[gnu::pure]]
  double find_zero_actual(double xstart) noexcept;

  [[gnu::pure]]
  double find_min_actual(double xstart) noexcept;

  /**
   * Tolerance in f of minimisation routine at x
   */
  [[gnu::pure]]
  double tolerance_actual_min(double x) const noexcept;

  /**
   * Tolerance in f of zero finding routine at x
   */
  [[gnu::pure]]
  double tolerance_actual_zero(double x) const noexcept;

  /**
   * Test whether solution is within tolerance
   *
   * @param xstart Initial value of x
   * @param tol_act Actual tolerance of routine evaluated at xstart
   *
   * @return true if no search required (xstart is good)
   */
  [[gnu::pure]]
  bool solution_within_tolerance(double xstart, double tol_act) noexcept;

};
