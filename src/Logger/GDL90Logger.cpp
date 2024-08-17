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

#include "Logger/GDL90Logger.hpp"
#include "io/FileOutputStream.hxx"
#include "LocalPath.hpp"
#include "time/BrokenDateTime.hpp"
#include "system/Path.hpp"
#include "util/StaticString.hxx"

GDL90Logger::GDL90Logger() noexcept {}
GDL90Logger::~GDL90Logger() noexcept = default;

//------------------------------------------------------------------------------
void
GDL90Logger::WriteLine(OutputStream& /*os*/, std::string_view /*text*/)
  {
//  os.Write(text.data(), text.size());

//  constexpr char newline = '\n';
//  os.Write(&newline, sizeof(newline));
  }

//------------------------------------------------------------------------------
void
GDL90Logger::Log(const void* /*text*/) noexcept
  {
  if (!this->enabled)
    return;

  const std::lock_guard lock{this->mutex};

  try
    {
    this->Start();
//    this->WriteLine(*file, text);
    }
  catch (...)
    {
    }
  }
