/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2023 The XCSoar Project
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
 * \addtogroup System
 * \{
 */

#include "util/Compiler.h"
#include "util/tstring.hpp"
#include "util/NumberParser.hpp"
#include "system/Path.hpp"

#ifdef _UNICODE
#include "system/ConvertPathName.hpp"
#endif

#include <list>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xcsoar-cassert>
#include <iterator>

#ifdef MORE_USAGE
extern void PrintMoreUsage();
#endif

/**
 * A class to encapsulate command line arguments.
 *
 * The arguments are processed as a first on last off stack.
 */
class Args
  {
  std::list<char*> args;  // The args as entered on the command line.
  const char* name;       // The name of the program from the command line.
  const char* usage;      // 

#ifdef _WIN32
  char* cmdline;
#endif

public:
  /**
   * Ctor. Initialize the object.
   * @param argc The argument count.
   * @param argv The argument pointer array.
   * @param usage The correct usage to be printed if necessary.
   */
  Args(int argc, char** argv, const char* usage)
    :name(argv[0]),
     usage(usage)
    {
    assert(name != nullptr);
    assert(usage != nullptr);

    std::copy(argv + 1, argv + argc, std::back_inserter(args));

#ifdef _WIN32
    this->cmdline = nullptr;
#endif
    }

  /**
   * No default copy ctor.
   */
  Args(const Args &other) = delete;

  /**
   * Copy ctor.
   * @param other The rhs.
   */
  Args(Args&& other)
    : name(other.name),
      usage(other.usage)
    {
    std::swap(this->args, other.args);
#ifdef _WIN32
    std::swap(this->cmdline, other.cmdline);
#endif
    }

#ifdef _WIN32
  /**
   * Ctor available for Windows.
   * @param _cmdline The command line from the OS.
   * @param usage The correct usage to be printed if necessary.
   */
  Args(const TCHAR* _cmdline, const char* usage)
    : usage(usage) 
    {
    this->ParseCommandLine(_cmdline);
    }

  /**
   * Dtor for Windows. Clean up.
   */
  ~Args()
    {
    delete[] this->cmdline;
    }

  /**
   * Parse the Windows command line.
   * @param _cmdline The command line from the OS.
   */
  void ParseCommandLine(const char* _cmdline)
    {
    const char* s = _cmdline;
    this->cmdline = new char[strlen(s) + 1];
    char* d = this->cmdline;      // current position in destination buffer
    char* option = this->cmdline;

    this->name = nullptr;
    bool in_qoute = false;
    do
      {
      if (*s == '"')
        in_qoute = !in_qoute;
      else if (*s == '\0' || (!in_qoute && *s == ' '))
        {
        // collapse runs of unqouted ' 's to a single '\0'
        if (d > this->cmdline && *(d-1) != '\0')
          {
          *d++ = '\0';
          // remember potential start position of next option
          option = d;
          }
        }
      else
        {
        *d = *s;
        if (option == d)
          {
          // first quoted blank or non blank character of new option
          // program name is not included in command line on CE
          if (name == nullptr)
            this->name = option;
          else
            this->args.push_back(option);
          }
        d++;
        }
      }
    while (*s++);

    if (this->name == nullptr)
      this->name = "";
    }

#ifdef _UNICODE
  /**
   * Parse the Windows command line in the Unicode case.
   * @param _cmdline The command line from the OS.
   */
  void ParseCommandLine(const TCHAR* _cmdline)
    {
    WideToACPConverter convert(_cmdline);
    this->ParseCommandLine(convert);
    }
#endif
#endif

  /**
   * No default copy operator.
   */
  Args& operator=(const Args& other) = delete;

  /**
   * Print the previously supplied uasage text.
   */
  [[noreturn]]
  void UsageError()
    {
    fprintf(stderr, "Usage: %s %s\n", this->name, this->usage);
#ifdef MORE_USAGE
    PrintMoreUsage();
#endif
    exit(EXIT_FAILURE);
    }

  /**
   * Are there any arguments?
   * @return If no arguments then true.
   */
  bool IsEmpty() const
    {
    return this->args.empty();
    }

  /**
   * Pop of the top of the argument stack. Adjust the stack pointer.
   *
   * There is an explicit assumption that the stack is not empty at this
   * point. If it is then we are in undefined territory!
   * @return The top argument.
   */
  const char* GetNext()
    {
    assert(!this->IsEmpty());

    const char* p = this->args.front();
    Skip();
    return p;
    }

  /**
   * Pop of the top of the argument stack and throw it away.
   */
  void Skip()
    {
    this->args.pop_front();
    }

  /**
   * Take a peek at the top of the stack. Do not adjust the stack pointer.
   * @return The top argument if it exists or nullptr otherwise.
   */
  const char* PeekNext() const
    {
    return this->IsEmpty() ? nullptr : this->args.front();
    }

  /**
   * Pop off the top of the argument stack. If the stack is empty print the
   * usage text and exit the program!
   * @return The top argument.
   */
  const char* ExpectNext()
    {
    if (this->IsEmpty())
      this->UsageError();

    return this->GetNext();
    }

  /**
   * Pop off the top argument stack which is expected to be a valid integer.
   * If the top of the stack cannot be evaluated as an integer then print
   * the usage text and exit the program!
   * @return The evaluated integer.
   */
  int ExpectNextInt()
    {
    const char* p = this->ExpectNext();
    assert(p != nullptr);

    char* endptr;
    int result = ParseInt(p, &endptr);
    if (p == endptr)
      this->UsageError();

    return result;
    }

  /**
   * Pop off the top argument stack which is expected to be a valid floating
   * point number.
   * If the top of the stack cannot be evaluated as a floating point then print
   * the usage text and exit the program!
   * @return The evaluated integer.
   */
  double ExpectNextDouble()
    {
    const char* p = this->ExpectNext();
    assert(p != nullptr);

    char* endptr;
    double result = ParseDouble(p, &endptr);
    if (p == endptr)
      this->UsageError();

    return result;
    }

  /**
   * Pop off the top argument stack which is treated as a string.
   * @return The string.
   */
  tstring ExpectNextT()
    {
    const char* p = this->ExpectNext();
    assert(p != nullptr);

#ifdef _UNICODE
    PathName convert(p);
    return tstring(((Path)convert).c_str());
#else
    return tstring(p);
#endif
    }

  /**
   * Pop off the top argument stack which is treated as a path.
   * @return The path.
   */
#ifdef _UNICODE
  AllocatedPath ExpectNextPath()
    {
    const char* p = this->ExpectNext();
    assert(p != nullptr);

    return AllocatedPath(PathName(p));
    }
#else
  Path ExpectNextPath()
    {
    const char* p = this->ExpectNext();
    assert(p != nullptr);

    return Path(p);
    }
#endif

  /**
   * If there are no more arguments on the stack then NOP. Otherwise print the
   * usage text and exit the program!
   */
  void ExpectEnd()
    {
    if (!this->IsEmpty())
      this->UsageError();
    }
  };

/**
 * \}
 */

