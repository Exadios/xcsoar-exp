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

#include <cstdint>

/**
 * Calculates the checksum for the specified line (without the
 * asterisk and the newline character).
 *
 * @param p a NULL terminated string
 */
[[gnu::pure]]
static inline uint8_t
NMEAChecksum(const char *p)
{
  uint8_t checksum = 0;

  /* skip the dollar sign at the beginning (the exclamation mark is
     used by CAI302 */
  if (*p == '$' || *p == '!')
    ++p;

  while (*p != 0)
    checksum ^= *p++;

  return checksum;
}

/**
 * Calculates the checksum for the specified line (without the
 * asterisk and the newline character).
 *
 * @param p a string
 * @param length the number of characters in the string
 */
[[gnu::pure]]
static inline uint8_t
NMEAChecksum(const char *p, unsigned length)
{
  uint8_t checksum = 0;

  unsigned i = 0;

  /* skip the dollar sign at the beginning (the exclamation mark is
     used by CAI302 */
  if (length > 0 && (*p == '$' || *p == '!')) {
    ++i;
    ++p;
  }

  for (; i < length; ++i)
    checksum ^= *p++;

  return checksum;
}

/**
 * Verify the NMEA checksum at the end of the specified string,
 * separated with an asterisk ('*').
 * @param p NULL terminated string starting with a '$' and ending with the
 *          three character checksum field ('*cc'). Do not forget to lose 
 *          any '\n' that may be after the checksum field.
 */
[[gnu::pure]]
bool
VerifyNMEAChecksum(const char *p);

/**
 * Caclulates the checksum of the specified string, and appends it at
 * the end, preceded by an asterisk ('*').
 */
void
AppendNMEAChecksum(char *p);
