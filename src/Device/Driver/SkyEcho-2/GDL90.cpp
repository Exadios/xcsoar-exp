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

#include "Device/Driver/SkyEcho-2/GDL90.hpp"

//------------------------------------------------------------------------------
void
HeartbeatMessage::ReadIn(const std::array<std::byte, 7>& image) noexcept
  {
  this->image = image;
  }

//------------------------------------------------------------------------------
bool 
HeartbeatMessage::GPSPosValid() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[7] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool
HeartbeatMessage::MaintenanceRequired() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[6] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool
HeartbeatMessage::Ident() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[5] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool
HeartbeatMessage::AddressType() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[4] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool
HeartbeatMessage::GPSBatteryLow() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[3] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool 
HeartbeatMessage::RATCS() const noexcept
  {
  std::bitset<8> status1(std::to_integer<unsigned int>(image[1])); 
  if (status1[2] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
unsigned int 
HeartbeatMessage::TimeStampBit16() const noexcept
  {
  std::bitset<8> status2(std::to_integer<unsigned int>(image[2])); 
  if (status2[7] == 0)
    return 0;
  else
    return 1 << 15;
  }  

//------------------------------------------------------------------------------
bool 
HeartbeatMessage::CSARequested() const noexcept
  {
  std::bitset<8> status2(std::to_integer<unsigned int>(image[2])); 
  if (status2[6] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool 
HeartbeatMessage::CSANotAvailable() const noexcept
  {
  std::bitset<8> status2(std::to_integer<unsigned int>(image[2])); 
  if (status2[5] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
bool 
HeartbeatMessage::UTC_OK() const noexcept
  {
  std::bitset<8> status2(std::to_integer<unsigned int>(image[2])); 
  if (status2[0] == 0)
    return false;
  else
    return true;
  }

//------------------------------------------------------------------------------
unsigned int 
HeartbeatMessage::TimeStamp() const noexcept
  {
  return (std::to_integer<unsigned int>(image[5]) << 8) + 
          std::to_integer<unsigned int>(image[4]);
  }

//------------------------------------------------------------------------------
void
TrafficReport::ReadIn(const std::array<std::byte, 28>& image) noexcept
  {
  this->image = image;
  }

//------------------------------------------------------------------------------
bool
TrafficReport::TrafficAlertStatus() const noexcept
  {
  std::bitset<8> status(std::to_integer<unsigned int>(image[1]));
  if (status[4] == 1)
    return true;
  else
    return false;
  }

//------------------------------------------------------------------------------
unsigned int
TrafficReport::AddressType() const noexcept
  {
  return std::to_integer<unsigned int>(image[1]) && 0xf;
  }

//------------------------------------------------------------------------------
unsigned int
TrafficReport::ParticipantAddress() const noexcept
  {
  return std::to_integer<unsigned int>(image[4]) * (2^16) +
         std::to_integer<unsigned int>(image[3]) * (2^8)  +
         std::to_integer<unsigned int>(image[2]);
  }

//------------------------------------------------------------------------------
double 
TrafficReport::Latitude() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
double 
TrafficReport::Longitude() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
double 
TrafficReport::Altitude() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
std::bitset<4> 
TrafficReport::MiscellaneousIndicators() const noexcept
  {
  return std::bitset<4>(0);
  }

//------------------------------------------------------------------------------
unsigned int 
TrafficReport::Integrity() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
unsigned int 
TrafficReport::Accuracy() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
double 
TrafficReport::HorizontalVelocity() const noexcept
  {
  return 0.0;
  }

//------------------------------------------------------------------------------
bool 
TrafficReport::HorizontalVelocityAvailable() const noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
double 
TrafficReport::VerticalVelocity() const noexcept
  {
  return 0.0;
  }

//------------------------------------------------------------------------------
double 
TrafficReport::TrackHeading() const noexcept
  {
  return 0.0;
  }

//------------------------------------------------------------------------------
unsigned int 
TrafficReport::EmitterCategory() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
std::string 
TrafficReport::CallSign() const noexcept
  {
  return std::string("        ");
  }

//------------------------------------------------------------------------------
unsigned int 
TrafficReport::EmergencyPriorityCode() const noexcept
  {
  return 0;
  }

//------------------------------------------------------------------------------
void
OwnShipReportMessage::ReadIn(const std::array<std::byte, 28>& image) noexcept
  {
  this->body.image = image;
  }

//------------------------------------------------------------------------------
void
TrafficReportMessage::ReadIn(const std::array<std::byte, 28>& image) noexcept
  {
  this->body.image = image;
  }

//------------------------------------------------------------------------------
double 
OwnShipGeoAltitudeMessage::OwnShipGeoAltitude() const noexcept
  {
  return 0.0;
  }

//------------------------------------------------------------------------------
bool 
OwnShipGeoAltitudeMessage::VerticalWarning() const noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
double 
OwnShipGeoAltitudeMessage::VerticalFigureofMerit() const noexcept
  {
  return 0.0;
  }

//------------------------------------------------------------------------------
bool 
OwnShipGeoAltitudeMessage::VFOMNotAvailable() const noexcept
  {
  return false;
  }

//------------------------------------------------------------------------------
bool 
OwnShipGeoAltitudeMessage::VFOMHigh() const noexcept
  {
  return false;
  }
