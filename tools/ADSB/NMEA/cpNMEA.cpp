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

#include <iostream>

#include <string.h>

char read_buffer[1024];

//------------------------------------------------------------------------------
void Usage(const char* argv0)
  {
  std::cout << argv0 << " -t <target id> -t <target id> ... \n";
  }

//------------------------------------------------------------------------------
int main(int /*argc*/, const char** /* * argv[]*/)
  {
  while ((std::cin.good() == true) && (std::cout.good() == true))
    {
    if ((std::cin.getline(::read_buffer, 1024)).good() != true)
      continue;
    if (strstr(::read_buffer, "$PFLAA") != NULL)
      {
      ;
      }
    else
      std::cout << ::read_buffer << "\n";
    }

  return 0;
  }
