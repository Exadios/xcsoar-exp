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

#include "GraphicsTTY.hpp"

#include <errno.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

LinuxGraphicsTTY::LinuxGraphicsTTY() noexcept
  :fd(open(path, O_RDWR | O_NOCTTY | O_CLOEXEC))
{
  if (fd < 0) {
    fprintf(stderr, "Warning: failed to open %s: %s\n",
            path, strerror(errno));
    return;
  }

  if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0)
    fprintf(stderr, "Warning: failed to set graphics mode on %s: %s\n",
            path, strerror(errno));
}

LinuxGraphicsTTY::~LinuxGraphicsTTY() noexcept
{
  if (fd >= 0) {
    ioctl(fd, KDSETMODE, KD_TEXT);
    close(fd);
  }
}
