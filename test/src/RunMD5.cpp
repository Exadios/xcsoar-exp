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

#include "io/FileReader.hxx"
#include "system/Args.hpp"
#include "util/MD5.hpp"
#include "util/PrintException.hxx"

#include <stdio.h>

static void
Feed(Reader &r, MD5 &state)
{
  while (true) {
    char buffer[65536];
    size_t nbytes = r.Read(buffer, sizeof(buffer));
    if (nbytes == 0)
      break;

    state.Append(buffer, nbytes);
  }
}

static void
Feed(Path path, MD5 &state)
{
  FileReader r(path);
  Feed(r, state);
}

int
main(int argc, char **argv)
try {
  Args args(argc, argv, "PATH");
  const auto path = args.ExpectNextPath();
  args.ExpectEnd();

  MD5 md5;
  md5.Initialise();

  Feed(path, md5);

  md5.Finalize();

  char digest[MD5::DIGEST_LENGTH + 1];
  md5.GetDigest(digest);

  puts(digest);
  return EXIT_SUCCESS;
} catch (...) {
  PrintException(std::current_exception());
  return EXIT_FAILURE;
}
