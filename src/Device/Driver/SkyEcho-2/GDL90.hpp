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

#include <cstddef>
#include <bitset>
#include <string>
#include <array>

/**
 * Refer to document "GDL 90 Data Interface Specification", 560-1058-00 Rev A,
 * June 5, 2007, published by Garmin. Individual references below can be found
 * in this document.
 */

/**
 * The Heartbeat Message which is sent unconditionally once per second.
 * \note See Table 3.
 */
struct HeartbeatMessage
  {
  std::array<std::byte, 7> image;

  /**
   * Populate this->image from elsewhere.
   */
  void ReadIn(const std::array<std::byte, 7>& image) noexcept;

  /**
   * Examine the GPS Position Valid status.
   * @return If the GPS Position is currently valid then true, false otherwise.
   */
  bool GPSPosValid() const noexcept;

  /**
   * Examine the Maintenance Required status.
   * @return Maintenance Required status, true or false.
   */
  bool MaintenanceRequired() const noexcept;

  /**
   * Examine the Ident status.
   * @return The IDENT talkback status, true or false.
   */
  bool Ident() const noexcept;

  void AddressType(bool arg) noexcept;

  /**
   * Examine the Address Type status.
   * @return If transmitting anonymous addresses then true, false otherwise.
   */
  bool AddressType() const noexcept;

  /**
   * Examine the GPS Battery Low status.
   * @return GPS Battery Low, true or false.
   */
  bool GPSBatteryLow() const noexcept;

  /**
   * Examine the RATCS status.
   * @return RATCS status, true or false.
   */
  bool RATCS() const noexcept;

  /**
   * Examine the UAT Time Stamp Bit 16.
   * @return Zero or 2^16.
   */
  unsigned int TimeStampBit16() const noexcept;

  /**
   * Examine the CSA Requested status.
   * @return CSA Requested, true or false.
   */
  bool CSARequested() const noexcept;

  /**
   * Examine the CSA Not Available status.
   * @return CSA Not Available, true or false
   */
  bool CSANotAvailable() const noexcept;

  /**
   * Examine the UTC OK status.
   * @return UTC OK, true or false.
   */
  bool UTC_OK() const noexcept;

  /**
   * Examine the Time Stamp (full 16 bits).
   * @return The Time Stamp - seconds since midnight.
   */
  unsigned int TimeStamp() const noexcept;
  };

static_assert(sizeof(HeartbeatMessage::image) == 7);

/**
 * Traffic Report which is sent as required.
 * \note See table 8.
 */
struct TrafficReport
  {
  std::array<std::byte, 28> image;

  /**
   * Populate this->image from elsewhere.
   */
  void ReadIn(const std::array<std::byte, 28>& image) noexcept;

  /**
   * Examine the Traffic Alert Status.
   * @return Traffic Alert Status: If and alert is in effect then true.
   */
  bool TrafficAlertStatus() const noexcept;

  /**
   * Examine the Address Type.
   * @return The Address Type as detailed in section 3.5.1.2.
   */
  unsigned int AddressType() const noexcept;

  /**
   * Examine the Participant Address.
   * @return A unique address of the type given by \ref AddressType.
   */
  unsigned int ParticipantAddress() const noexcept;

  /**
   * Examine the Latitude.
   * @return The Latitude in degrees, [-180.000, 180.000].
   */
  double Latitude() const noexcept;

  /**
   * Examine the Longitude.
   * @return The Longitude in degrees, [-180.000, 180.000].
   */
  double Longitude() const noexcept;

  /**
   * Examine the Altitude.
   * @return The Altitude in feet, [-1000, 101400].
   */
  double Altitude() const noexcept;

  /**
   * Examine the Miscellaneous Indicators.
   * @return The bit field which may be decoded by referring to 
   * section 3.5.1.5.
   */
  std::bitset<4> MiscellaneousIndicators() const noexcept;

  /**
   * Examine the Integrity status
   * @return The Integrity which may be decoded by referring to
   * section 3.5.1.6.
   */
  unsigned int Integrity() const noexcept;

  /**
   * Examine the Accuracy status.
   * @return The Integrity which may be decoded by referring to
   * section 3.5.1.6.
   */
  unsigned int Accuracy() const noexcept;

  /**
   * Examine the Horizontal Velocity.
   * @return The Horizontal Velocity in knots, [0, 999].
   */
  double HorizontalVelocity() const noexcept;

  /**
   * Examine the Horizontal Velocity Available status.
   * @return If available then true. See section 3.5.1.7.
   */
  bool HorizontalVelocityAvailable() const noexcept;

  /**
   * Examine the Vertical Velocity.
   * @return The Vertical Velocity in feet per minute [-32576, 32576].
   */
  double VerticalVelocity() const noexcept;

  /**
   * Examine the Track / Heading.
   * @return The Track or Heading in degrees. Whether this return represents
   *         track or heading is determined by the result given by
   *         \ref MiscellaneousIndicators. See section 3.5.1.9.
   */
  double TrackHeading() const noexcept;

  /**
   * Examine the Emitter Category.
   * @return The category given in section 3.1.5.10.
   */
  unsigned int EmitterCategory() const noexcept;

  /**
   * Examine the Call Sign.
   * @return A non null terminated array of ASCII characters.
   */
  std::string CallSign() const noexcept;

  /**
   * Examine the Emergency / Priority Code.
   * @return A code given by section 3.1.5.12
   */
  unsigned int EmergencyPriorityCode() const noexcept;
  };

/**
 * Own Ship Report Message transmitted unconditionally once per second.
 * See section 3.4.
 */
struct OwnShipReportMessage
  {
  TrafficReport body;

  /**
   * Populate this->image from elsewhere.
   */
  void ReadIn(const std::array<std::byte, 28>& image) noexcept;
  };

static_assert(sizeof(OwnShipReportMessage::body) == 28);

/**
 * Traffic Report Message. A number of these are transmitted, within each 
 * one second time frame, one per target detected. See section 3.5.
 */
struct TrafficReportMessage
  {
  TrafficReport body;

  /**
   * Populate this->image from elsewhere.
   */
  void ReadIn(const std::array<std::byte, 28>& image) noexcept;
  };

static_assert(sizeof(TrafficReportMessage::body) == 28);

/**
 * Own Ship Geo Altitude Message is transmitted once every second when a
 * geometric altitude is available. See section 3.8.
 */
struct OwnShipGeoAltitudeMessage
  {
  std::array<std::byte, 5> image;

  /**
   * Populate this->image from elsewhere.
   */
  void ReadIn(const std::array<std::byte, 5>& image) noexcept;

  /**
   * Examine the OwnShip Geo Altitude.
   * @return The altitude in feet, [-81920, 81920]. See section 3.8.
   */
  double OwnShipGeoAltitude() const noexcept;

  /**
   * Examine the Vertical Warning status.
   * @return The status, true if set. See section 3.8.
   */
  bool VerticalWarning() const noexcept;

  /**
   * Examine the Vertical Figure of Merit.
   * @return FoM in meters, [0, 32766]. See section 3.8.
   */
  double VerticalFigureofMerit() const noexcept;

  /**
   * Examine the VFOM not available status.
   * @return Availability, true if not available.
   */
  bool VFOMNotAvailable() const noexcept;

  /**
   * Examine the VFOM High status.
   * @return If VFOM > 32766 meters then true.
   */
  bool VFOMHigh() const noexcept;
  };

static_assert(sizeof(OwnShipGeoAltitudeMessage::image) == 5);
