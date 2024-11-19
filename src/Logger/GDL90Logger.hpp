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

#pragma once

/**
 * \file
 * \addtogroup Logger
 * \{
 */

#include "thread/Mutex.hxx"
#include "Logger/RawInputDataLogger.hpp"

#include <memory>

class FileOutputStream;
class OutputStream;

/**
 * A logger to record incomming GDL90 packets.
 */
class GDL90Logger : public RawInputDataLogger
  {
public:
  GDL90Logger() noexcept;
  ~GDL90Logger() noexcept;

  /**
   * Log packets to log file. If this logger is not enabled then do nothing.
   * @param s Packets
   */
  void Log(const std::span<const std::byte> s) noexcept override final;

protected:
//  void Start();

  /**
   * The file extension for GDL90 log files.
   * @return "gdl90"
   */
  const char* ExtName() const noexcept override final
    {
    return "gdl90";
    }

private:
  /**
   * Write an entry to our file.
   * data The current message)
   */
  void WriteMessage(OutputStream& f, const std::span<const std::byte> s);

  };

/** \} */
