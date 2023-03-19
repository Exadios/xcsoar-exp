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

#include "Surveillance/RemoteTarget.hpp"
#include <memory>

/**
 * \file
 * \addtogroup
 * \{
 */

class RemoteTarget;
class RemoteTargetComputer;

typedef std::unique_ptr<RemoteTargetComputer> RemoteTargetComputerPtr;

/**
 * The purpose of this class is to provide a Factory that will generate 
 * a \ref RemoteTargetComputer of the correct type to suit a given 
 * \ref RemoteTarget.
 */
class RemoteTargetComputerFactory
  {
public:
  /**
   * Ctor.
   */
  RemoteTargetComputerFactory();

  /**
   * Create the computer
   * @param target The target which is the object of the created computer.
   * @return The correct computer or nullptr if target pointer is unknown.
   */
  RemoteTargetComputer* Create(const TargetPtr target);
  };

/**
 * \}
 */
