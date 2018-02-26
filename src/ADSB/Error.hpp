/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#ifndef XCSOAR_ADSB_ERROR_HPP
#define XCSOAR_ADSB_ERROR_HPP

#include "NMEA/Validity.hpp"
#include "Compiler.h"

#include <type_traits>

#include <stdint.h>
#include <tchar.h>

#ifdef WIN32
#undef NO_ERROR
#endif

struct AdsbError {
  enum Severity : uint8_t {
    NO_ERROR = 0x00,
    INFORMATION_ONLY = 0x01,
    REDUCED_FUNCTIONALITY = 0x02,
    FATAL_PROBLEM = 0x03,
  };

  enum Code : uint8_t {
    POWER = 0x21,
    RF_COMMUNICATION = 0x41,
    COMMUNICATION = 0x51,
    OTHER = 0xf1,
  };

  Validity available;

  Severity severity;
  Code code;

  bool IsWarning() const
    {
    return this->severity >= REDUCED_FUNCTIONALITY;
    }

  bool IsError() const
    {
    return this->severity >= FATAL_PROBLEM;
    }

  void Clear() 
    {
    this->available.Clear();
    }

  void Complement(const AdsbError &add)
    {
    if (this->available.Complement(add.available))
      {
      this->severity = add.severity;
      this->code = add.code;
      }
    }

  void Expire(gcc_unused double clock)
    {
    /* no expiry; this object will be cleared only when the device
       connection is lost */
    }

  /**
   * Returns a human-readable translatable string for the given value.
   * The caller is responsible for calling gettext() on the return
   * value.
   */
  gcc_const
  static const TCHAR *ToString(Severity severity);

  /**
   * Returns a human-readable translatable string for the given value.
   * The caller is responsible for calling gettext() on the return
   * value.
   */
  gcc_const
  static const TCHAR *ToString(Code code);

  gcc_pure
  const TCHAR *GetSeverityString() const
    {
    return this->ToString(this->severity);
    }

  gcc_pure
  const TCHAR *GetCodeString() const
    {
    return this->ToString(this->code);
    }
  };

static_assert(std::is_trivial<AdsbError>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_ERROR_HPP
