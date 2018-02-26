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

#include "Error.hpp"
#include "Util/Macros.hpp"
#include "Language/Language.hpp"

static const TCHAR *const severity_strings[] =
  {
  N_("No error"),
  N_("Information"),
  N_("Reduced functionality"),
  N_("Fatal problem"),
  };

const TCHAR *
AdsbError::ToString(Severity severity)
  {
  unsigned i = (unsigned)severity;
  return i < ARRAY_SIZE(severity_strings)
    ? severity_strings[i]
    : N_("Unknown");
  }

static constexpr struct
  {
  AdsbError::Code code;
  const TCHAR *string;
  } error_strings[] = 
  {
    { AdsbError::Code::POWER, N_("Power") },
    { AdsbError::Code::COMMUNICATION, N_("Communication") },
    { AdsbError::Code::OTHER, N_("Other") },
    { AdsbError::Code::OTHER, nullptr }
  };

const TCHAR *
AdsbError::ToString(Code code)
  {
  for (auto i = error_strings; i->string != nullptr; ++i)
    if (i->code == code)
      return i->string;

  return N_("Unknown");
  }
