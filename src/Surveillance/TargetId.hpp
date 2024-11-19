/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
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

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include <cstdint>
#include <compare> // for the defaulted spaceship operator

#ifdef _UNICODE
#include <tchar.h>
#endif

/**
 * The identification number of a Surveillance target. It is supposed that
 * a target of any type can be indexed by a eight digit hex code.
 */
class TargetId
  {
private:
  static constexpr uint32_t UNDEFINED_VALUE = 0;

  uint32_t value;

  /**
   * Ctor for internal use only.
   * @param value The target Id to set.
   */
  constexpr
  TargetId(uint32_t value)
  : value(value)
  {
  }

public:
  /**
   * Default ctor.
   *
   * The explicit default is necessary to keep the many classes that have 
   * variables of this type as "is_trivial" which, I suppose, is a 
   * Good Thing (mr).
   *
   * Please do not use this class until Clear has been called (sigh).
   */
  TargetId() = default;

  /**
   * Initialization ctor
   * @param id An integer representation of the id. Value less than 0 will
   *           result is an UNDEFINED value.
   */
  TargetId(long int id)
    {
    if (id < 0)
      this->value = UNDEFINED_VALUE;
    else
      value = id;
    }

  /**
   * Give an undefined TargetId.
   * @return Always an undefined TargetId.
   */
  static constexpr TargetId Undefined() noexcept
    {
    return TargetId(UNDEFINED_VALUE);
    }

  /**
   * Give a defined status.
   * @return Always true.
   */
  constexpr bool IsDefined() const noexcept
    {
    return this->value != UNDEFINED_VALUE;
    }

  /**
   * Clear this object to undefined.
   */
  constexpr void Clear() noexcept
    {
    this->value = UNDEFINED_VALUE;
    }

  /**
   * Give the value of the id.
   * @return The id.
   */
  constexpr uint32_t Arg() const noexcept
    {
    return this->value;
    }

  /** Set the value of the id.
   * @param value The desired value of the id.
   */
  constexpr void Arg(const uint32_t value)
    {
    this->value = value;
    }

#if 0
  /**
   * Give this id as a \ref FlarmId. This is temporarily necessary because
   * \ref FlarmId * is used in many ancillary areas of XCSoar.
   * @return This id as a \ref FlarmId.
   * \todo Get rid of \ref FlarmId throughout XCSoar.
   */
  FlarmId ToFlarmId() const;
#endif

  friend constexpr auto operator<=>(const TargetId&,
                                    const TargetId&) noexcept = default;

  /**
   * Set the state of this object from a string value.
   * @param input The Id value given in hex form.
   * @param endptr_r If **endptr_r is not equal to NULL then *endptr_r 
   *                 is set to point to the first char after the termination
   *                 of the parsing of input. If all of input is parsed then
   *                 '\0' will be returned.
   * @return A TargetId having the state corresponding to input.
   */
  static TargetId Parse(const char* input, char** endptr_r) noexcept;
#ifdef _UNICODE
  static TargetId Parse(const TCHAR* input, TCHAR** endptr_r) noexcept;
#endif

  /**
   * Give the Id as a hex string.
   * @param buffer The location of the return string. Must point to a valid
   *               memory area capable of holding the result.
   * @return Returns buffer as supplied.
   */
  const char* Format(char* buffer) const noexcept;
#ifdef _UNICODE
  const TCHAR* Format(TCHAR* buffer) const noexcept;
#endif
  };

/**
 * \}
 */
