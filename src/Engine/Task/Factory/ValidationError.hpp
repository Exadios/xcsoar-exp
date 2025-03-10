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

#include "util/EnumBitSet.hxx"

#include <cstdint>

/** Task Validation Error Types */
enum class TaskValidationErrorType : uint8_t {
  NO_VALID_START,
  NO_VALID_FINISH,
  TASK_NOT_CLOSED,
  TASK_NOT_HOMOGENEOUS,
  INCORRECT_NUMBER_TURNPOINTS,
  EXCEEDS_MAX_TURNPOINTS,
  UNDER_MIN_TURNPOINTS,
  TURNPOINTS_NOT_UNIQUE,
  EMPTY_TASK,
  NON_FAI_OZS,
  NON_MAT_OZS,

  /**
   * The task doesn't have the required shape, e.g. for FAI triangle.
   */
  WRONG_SHAPE,

  /**
   * This special value is used to determine the number of items
   * above.
   */
  COUNT
};

using TaskValidationErrorSet = EnumBitSet<TaskValidationErrorType>;

/**
 * Is this an error which is not acceptable?  This ignores warnings.
 */
constexpr bool
IsError(TaskValidationErrorSet set) noexcept
{
  /**
   * These codes are just warnings; they may be displayed, but the
   * user is allowed to proceed.
   */
  constexpr TaskValidationErrorSet warnings{
    TaskValidationErrorType::TURNPOINTS_NOT_UNIQUE,
  };

  constexpr TaskValidationErrorSet errors = ~warnings;

  return !(set & errors).IsEmpty();
}
