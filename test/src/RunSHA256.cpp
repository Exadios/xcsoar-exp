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

#include "lib/sodium/SHA256.hxx"
#include "system/Args.hpp"
#include "io/FileReader.hxx"
#include "util/PrintException.hxx"

#include <stdio.h>

static void
Feed(Reader &r, SHA256State &state)
{
  while (true) {
    std::byte buffer[65536];
    size_t nbytes = r.Read(buffer, sizeof(buffer));
    if (nbytes == 0)
      break;

    state.Update(std::span<const std::byte>{buffer, nbytes});
  }
}

static void
Feed(Path path, SHA256State &state)
{
  FileReader r(path);
  Feed(r, state);
}

static void
HexPrint(std::span<const std::byte> b) noexcept
{
  for (const std::byte i : b)
    printf("%02x", (unsigned)i);
}

int
main(int argc, char **argv)
try {
  Args args(argc, argv, "PATH");
  const auto path = args.ExpectNextPath();
  args.ExpectEnd();

  SHA256State state;
  Feed(path, state);

  HexPrint(state.Final());
  printf("\n");

  return EXIT_SUCCESS;
} catch (...) {
  PrintException(std::current_exception());
  return EXIT_FAILURE;
}
