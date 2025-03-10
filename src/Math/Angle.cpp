/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2025 The XCSoar Project
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

#include "Angle.hpp"

#include <xcsoar-cassert>

Angle::DMS
Angle::ToDMS() const noexcept
{
  DMS dms;
  dms.negative = value < 0;

  unsigned value = lround(AbsoluteDegrees() * 3600);

  dms.seconds = value % 60;
  value /= 60;

  dms.minutes = value % 60;
  value /= 60;

  dms.degrees = value;

  return dms;
}

Angle::DMM
Angle::ToDMM() const noexcept
{
  DMM dmm;
  dmm.positive = value >= 0;

  unsigned value = lround(AbsoluteDegrees() * 60000);
  dmm.degrees = value / 60000;
  value %= 60000;
  dmm.minutes = value / 1000;
  dmm.decimal_minutes = value % 1000;

  return dmm;
}

double
Angle::AbsoluteDegrees() const noexcept
{
  return Absolute().Degrees();
}

double
Angle::AbsoluteRadians() const noexcept
{
  return Absolute().Radians();
}

Angle
Angle::AsBearing() const noexcept
{
  assert(!isnan(value));
  assert(!isinf(value));
  assert(fabs(value) < 100 * FullCircle().Native());

#ifndef FIXED_MATH
  /* Workaround for endless loops below; this is only for release
     builds.  Debug builds will crash due to assertion failure.  Right
     now, I don't know where those abnormal values come from, but I
     hope we'll find out soon - until that happens, this workaround
     reduces some user frustration with XCSoar freezes. */
  if (!isnormal(value))
    return Zero();
#endif

  Angle retval(value);

  while (retval < Zero())
    retval += FullCircle();

  while (retval >= FullCircle())
    retval -= FullCircle();

  return retval;
}

Angle
Angle::AsDelta() const noexcept
{
  assert(!isnan(value));
  assert(!isinf(value));
  assert(fabs(value) < 100 * FullCircle().Native());

#ifndef FIXED_MATH
  /* same workaround as in AsBearing() */
  if (!isnormal(value))
    return Zero();
#endif

  Angle retval(value);

  while (retval <= -HalfCircle())
    retval += FullCircle();

  while (retval > HalfCircle())
    retval -= FullCircle();

  return retval;
}

Angle
Angle::Reciprocal() const noexcept
{
  return (*this + HalfCircle()).AsBearing();
}

Angle
Angle::HalfAngle(const Angle end) const noexcept
{
  if (value == end.value) {
    return Reciprocal();
  } else if (value > end.value) {
    if ((*this - end) < HalfCircle())
      return (*this + end).Half().Reciprocal();
    else
      return (*this + end).Half();
  } else {
    if ((end - *this) < HalfCircle())
      return (*this + end).Half().Reciprocal();
    else
      return (*this + end).Half();
  }
}

Angle
Angle::Fraction(const Angle end, const double fraction) const noexcept
{
  if (value == end.value)
    return Angle(value);

  const Angle diff = Angle(end.value - value).AsDelta();
  return Angle(value + diff.value * fraction);
}

bool
Angle::Between(const Angle start, const Angle end) const noexcept
{
  Angle width = (end - start).AsBearing();
  Angle delta = (*this - start).AsBearing();

  return delta <= width;
}

bool
Angle::CompareRoughly(Angle other, Angle threshold) const noexcept
{
  const Angle delta = (*this - other).AsDelta();
  return delta >= -threshold && delta <= threshold;
}
