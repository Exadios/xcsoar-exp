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

#include "thread/Mutex.hxx"

#include <memory>
#include <span>

class FileOutputStream;

class RawInputDataLogger
  {
protected:
  Mutex mutex;
  std::unique_ptr<FileOutputStream> file;

  bool enabled = false;

public:
  RawInputDataLogger() noexcept;
  virtual ~RawInputDataLogger() noexcept;

  bool IsEnabled() const noexcept
    {
    return enabled;
    }

  /**
   * Enable this logger.
   */
  void Enable() noexcept
    {
    enabled = true;
    }

  /**
   * Invert the current enabled state of this logger.
   */
  void ToggleEnabled() noexcept
    {
    enabled = !enabled;
    }

  /**
   * Logs NMEA string to log file
   * @param data The data to be logged. This data
   */
  virtual void Log(const std::span<const std::byte> s) noexcept = 0;

protected:
  /**
   * Initialize the class by creating and opening the file.
   */
  void Start();

  /**
   * Provide the "file extension" of the logger file.
   * @return The extension only in ASCII
   */
  virtual const char* ExtName() const noexcept = 0;

  };
