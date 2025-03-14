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

#include <tchar.h>

template<class T>
class LineReader {
public:
  virtual ~LineReader() {}

  /**
   * Read one line, and returns a it as a null-terminated string
   * (without the trailing line feed character).  The returned buffer
   * is writable, and may be modified by the caller while parsing it.
   * It is Invalidated by the next call.  After the last line has been
   * read, this method returns nullptr.
   *
   * Throws on error.
   */
  virtual T *ReadLine() = 0;

  /**
   * Determins the size of the file.  Returns -1 if the size is
   * unknown.
   */
  [[gnu::pure]]
  virtual long GetSize() const {
    return -1;
  }

  /**
   * Determins the current position within the file.  Returns -1 if
   * this is unknown.
   */
  [[gnu::pure]]
  virtual long Tell() const {
    return -1;
  }
};

class TLineReader : public LineReader<TCHAR> {};

#ifdef _UNICODE
class NLineReader : public LineReader<char> {};
#else
class NLineReader : public TLineReader {};
#endif
