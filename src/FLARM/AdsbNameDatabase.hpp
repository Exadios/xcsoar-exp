/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2018 The XCSoar Project
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

#ifndef XCSOAR_ADSB_NAME_DATABASE_HPP
#define XCSOAR_ADSB_NAME_DATABASE_HPP

#include "Util/StaticString.hxx"
#include "Util/StaticArray.hxx"
#include "FlarmId.hpp"
#include "Compiler.h"

#include <assert.h>
#include <tchar.h>

class AdsbNameDatabase
  {
public:
  /**
   * Ctor.
   */
  AdsbNameDatabase();

  struct Record
    {
    FlarmId id;
    StaticString<21> name;

    Record() = default;
    Record(FlarmId id, const TCHAR *name)
      :id(id), name(name) {}
    };

private:
   Record data[256];
   unsigned current;

public:
  gcc_pure
  const Record *begin() const
    {
    return this->data;
    }

  gcc_pure
  const Record *end() const
    {
    return &this->data[255];
    }

  gcc_pure
  const TCHAR *Get(FlarmId id) const;

  gcc_pure
  FlarmId Get(const TCHAR *name) const;

  /**
   * Look up all records with the specified name.
   *
   * @param max the maximum size of the given buffer
   * @return the number of items copied to the given buffer
   */
  unsigned Get(const TCHAR *name, FlarmId *buffer, unsigned max) const;

  bool Set(FlarmId id, const TCHAR *name);

protected:
  gcc_pure
  int Find(FlarmId id) const;

  gcc_pure
  int Find(const TCHAR *name) const;
  };

#endif  // XCSOAR_ADSB_NAME_DATABASE_HPP
