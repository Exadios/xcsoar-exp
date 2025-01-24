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
 */

#include "Device/Driver/SkyEcho-2/GDL90.hpp"
#include <cstddef>
#include <bitset>

/**
 * Refer to document "GDL 90 Data Interface Specification", 560-1058-00 Rev A,
 * June 5, 2007, published by Garmin. Individual references below can be found
 * in this document.
 */

/**
 * The Heartbeat Message which is sent unconditionally once per second.
 * \note See Table 3.
 */
struct HeartbeatMessage_Debug : public HeartbeatMessage
  {
  /**
   * Ctor.
   */
  HeartbeatMessage_Debug()
    {
    this->image[0] = this->image[1] = std::byte{0};
    }

  /**
   * Modify the GPS Position Valid status.
   * @param arg The value of the GPS Position Valid bit, true or false.
   */
  void GPSPosValid(bool arg) noexcept;

  /**
   * Modify the Maintenance Required status.
   * @param arg The value of the Maintenance Required bit, true or false.
   */
  void MaintenanceRequired(bool arg) noexcept;

  /**
   * Modify Ident status.
   * @param arg The value of the IDENT talkback bit, true or false.
   */
  void Ident(bool arg) noexcept;

  /**
   * Modify the Address Type status.
   * @param arg If transmitting anonymous addresses then true.
   */
  void AddressType(bool arg) noexcept;

  /**
   * Modify the GPS Battery Low status.
   * @param arg The value of the GPS Battery Low status, true or false.
   */
  void GPSBatteryLow(bool arg) noexcept;

  /**
   * Modify the RATCS status. RATCS stands for "Receiving ATC Services",
   * see 3.1.1.f.
   * @param arg The value of the RATCS, true or false.
   */
  void RATCS(bool arg) noexcept;

  /**
   * Modify the UAT Time Stamp Bit 16
   * @param seconds Seconds since midnight.
   */
  void TimeStampBit16(unsigned int seconds) noexcept;

  /**
   * Modify the CSA Requested status.
   * @param arg The value of the CSA Requested status, true or false.
   */
  void CSARequested(bool arg) noexcept;

  /**
   * Modify the CSA Not Available status.
   * @param arg The value of the CSA Not Available status, true or false.
   */
  void CSANotAvailable(bool) noexcept;

  /**
   * Modify the UTC OK status.
   * @param arg The value of the UTC OK status, true or false.
   */
  void UTC_OK(bool arg) noexcept;

  /**
   * Modify the Time Stamp (full 16 bits).
   * @param ts Time Stamp - seconds since midnight.
   */
  void TimeStamp(unsigned int ts) noexcept;
  };

struct OwnShipReportMessage_Debug : public OwnShipReportMessage
  {
  /**
   * Ctor.
   */
  OwnShipReportMessage_Debug()
    {
    this->body.image[0] = std::byte{0};
    this->body.image[1] = std::byte{10};
    }
  };

struct TrafficReportMessage_Debug : public TrafficReportMessage
  {
  };
