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

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

struct TargetData;
struct NMEAInfo;

/**
 * The purpose of this class is to provide a base class for a computer
 * process \ref RemoteTarget data. This processing is low cost and occurs
 * in the merge thread.
 */ 
class RemoteTargetComputer
  {
public:
  /**
   * Default ctor.
   */
  RemoteTargetComputer();

  /**
   * Do not allow copy ctor.
   */
  RemoteTargetComputer(const RemoteTargetComputer& rhs) = delete;

  /**
   * Do not allow copy operator.
   */
  RemoteTargetComputer& operator=(const RemoteTargetComputer& rhs) = delete;

  /**
   * Virtual dtor for overloaded member functions.
   */
  virtual ~RemoteTargetComputer();

  /**
   * Process each target to produce the vital variables for those targets.
   * @param traffic Currents targets
   * @param last_traffic Targets of the previous interval.
   * @param basic The data corresponding to this particular device.
   */
  virtual void Process(TargetData& traffic,
                       TargetData& last_traffic,
                       NMEAInfo&   basic);

protected:

private:

  };

/**
 * \}
 */
