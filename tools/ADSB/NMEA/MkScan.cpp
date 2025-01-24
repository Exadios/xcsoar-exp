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

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cstring>

#include <string.h>

struct Target
  {
  unsigned int id;
  int          count;

  Target()
    : id(0),
      count(0)
    {
    }

  bool operator==(const Target& rhs)
    {
    if (this->id == rhs.id)
      return true;
    return false;
    }
  };
 
std::vector<Target> target;
char read_buffer[1024];

//------------------------------------------------------------------------------
bool
TargetCompare(const Target& t1, const Target& t2)
  {
  if (t2.count < t1.count)
    return true;
  return false;
  }

//------------------------------------------------------------------------------
int main(int argc, const char* argv[])
  {
  bool gnuplot = false;

  if ((argc == 2) && (strcmp(argv[1], "-p") == 0))
    gnuplot = true;
      
  while (std::cin.good())
    {
    if ((std::cin.getline(read_buffer, 1024)).good() == false)
      continue;
    
    if (strstr(read_buffer, "$PFLAA") == NULL)
      continue; // Don't bother with other records.
    
    std::vector<std::string> tokens;

    for (char* t = std::strtok(read_buffer, ",");
         t != nullptr;
         t = std::strtok(nullptr, ","))
      {
      tokens.push_back(std::string(t));
      }

    unsigned int id = std::stoi(std::string("0X") + tokens[6], nullptr, 16);
    std::vector<Target>::iterator found;
    Target ref;
    ref.id = id;
    found = find(::target.begin(), ::target.end(), ref);
      if (found != ::target.end())
        found->count++;
      else
        {
        ref.count++;
        ::target.push_back(ref);
        }
    }

  std::sort(::target.begin(), ::target.end(), ::TargetCompare);

  if (gnuplot == false)
    for (size_t i = 0; i < ::target.size(); i++)
      std::cout << "id = 0x" << std::uppercase << std::hex << ::target[i].id
                << std::dec << ", count = " << ::target[i].count << "\n";
  else
    {
    std::ofstream commands("tmp.cmd", std::ios::trunc);

    commands << "set datafile separator comma\n";
    commands << "set xtics format \"\"\n";
    for (size_t i = 0; i < ::target.size(); i++)
      {
      commands << "set xtics add (\"" << std::hex << ::target[i].id
               << "\" " << std::dec << i << ")\n";
      }
    commands << "plot \"tmp.csv\" with boxes\n";
    commands << "pause mouse\n";
    commands.close();
    
    std::ofstream csv("tmp.csv", std::ios::trunc);

    for (size_t i = 0; i < ::target.size(); i++)
      csv << i << "," << ::target[i].count << "\n";
    csv.close();
    }


  return 0;
  }
