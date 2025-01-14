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

#include "Logger/GDL90Logger.hpp"
#include "io/FileOutputStream.hxx"
#include "LocalPath.hpp"
#include "time/BrokenDateTime.hpp"
#include "system/Path.hpp"
#include "util/StaticString.hxx"

#ifndef NDEBUG
#include <iostream>
#endif

GDL90Logger::GDL90Logger() noexcept {}
GDL90Logger::~GDL90Logger() noexcept = default;

//------------------------------------------------------------------------------
void
GDL90Logger::WriteMessage(OutputStream& os, const std::span<const std::byte> s)
  {
  os.Write(s.data(), s.size());
  }

//------------------------------------------------------------------------------
void
GDL90Logger::Log(const std::span<const std::byte> s) noexcept
  {
  if (!this->enabled)
    return;

#ifndef NDEBUG
  std::cout << __FILE__ << ", " << __LINE__ << "\n";
#endif

  const std::lock_guard lock{this->mutex};

  try
    {
    this->Start();
    this->WriteMessage(*this->file, s);
    }
  catch (...)
    {
    }
  }

