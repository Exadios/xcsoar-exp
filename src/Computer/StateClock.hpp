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

#pragma once

#include "time/FloatDuration.hxx"

#include <algorithm>

#include <xcsoar-cassert>

/**
 * Track how much time the aircraft was in a certain state
 * (e.g. "moving").  It provides tools to decrement the measured
 * duration while the aircraft was not in that state.
 *
 * @param max_value the maximum value; at this peak, the countdown
 * will start, and after this duration, the state will be cleared
 * @param max_delta the maximum delta accepted by Add() and
 * Subtract(), to avoid hiccups due to temporary GPS outage
 */
template<unsigned max_value, unsigned max_delta>
class StateClock {
  FloatDuration value;

public:
  void Clear() {
    value = {};
  }

  bool IsDefined() const {
    assert(value.count() >= 0);

    return value.count() > 0;
  }

private:
  static constexpr FloatDuration LimitDelta(FloatDuration delta) noexcept {
    assert(delta.count() >= 0);

    return std::min(delta, FloatDuration{max_delta});
  }

public:
  void Add(FloatDuration delta) noexcept {
    assert(value.count() >= 0);
    assert(value <= FloatDuration{max_value});

    value += LimitDelta(delta);
    if (value > FloatDuration{max_value})
      value = FloatDuration{max_value};
  }

  void Subtract(FloatDuration delta) noexcept {
    assert(value.count() >= 0);
    assert(value <= FloatDuration(max_value));

    value -= LimitDelta(delta);
    if (value.count() < 0)
      value = {};
  }

  constexpr bool operator>=(FloatDuration other) const noexcept {
    assert(other.count() > 0);
    assert(value.count() >= 0);
    assert(value <= FloatDuration{max_value});

    return value >= other;
  }
};
