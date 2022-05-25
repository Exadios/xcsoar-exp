/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#ifndef XCSOAR_SKYECHO_DEVICE_HPP
#define XCSOAR_SKYECHO_DEVICE_HPP

/** \file
 * The uAvionix SkyEcho 2 ADS-B transceiver
 * @see https://www.uavionix.com.au/skyecho2/
 */

#include "util/AllocatedArray.hxx"
#include "util/Compiler.h"
#include "tchar.h"
#include "Device/Driver.hpp"
#include "Device/SettingsMap.hpp"
#include "Geo/GeoPoint.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <span>

class Port;
struct Declaration;
class OperationEnvironment;
class RecordedFlightList;
struct RecordedFlightInfo;
class NMEAInputLine;

/**
 * The uAvionix SkyEcho 2 ADS-B transceiver.
 */
class SkyEchoDevice: public AbstractDevice
  {
public:
  /**
   * Ctor.
   * @param port The input port associated with this device.
   */
  SkyEchoDevice(Port &port)
    :port(port)
    {
    this->Initfcs();
    }

  /**
   * Receive data from the SkyEcho.
   * @param s The input vector. This vector contains byte stuffing.
   * @param info The information received from the SkyEcho including 
   *             the traffic list.
   */
  bool DataReceived(std::span<const std::byte> s,
                    struct NMEAInfo &info) noexcept override;

  /* virtual methods from class Device */
  void LinkTimeout() override;

  /**
   * Attempt to restart the device.
   * Where's this from?!
   */
  void Restart(OperationEnvironment &env);

private:
  /**
   * GDL 90 packet types.
   */
  enum class MessageID : uint8_t
    {
    HEARTBEAT       = 0,
    INITIALIZATION  = 2,
    UPLINK          = 7,
    ALTITUDEQFE     = 9,
    OWNSHIPREPORT   = 10,
    OWNSHIPALTITUDE = 11,
    TRAFFICREPORT   = 20,
    BASICREPORT     = 30,
    LONGREPORT      = 31,
    AHRS            = 0x65,
    };

  /**
   * The CRC table computed at ctor time.
   * \todo pfb: Investigate generating this table at compile time.
   */
  uint16_t crc16Table[256]; // Table for FCS.

  /**
   * A structure to hold raw own ship and target data for a limited time.
   */
  struct TrafficStruct
    {
    TrafficStruct()
      {
      this->Reset();
      }

    /**
     * Reset the validity of the own ship data.
     */
    void Reset(void)
      {
      this->nav_integrity = 0;  // Initially there is no nav integrity
      this->nav_accuracy  = 0;  // or accuracy.
      }

    /**
     * True if this target is transmitting a traffic alert. See Table 8
     * of the GDL Data Interface Specification.
     */
    bool traffic_alert;

    /**
     * The received address type. See 3.5.1.2 of the GDL Data Interface
     * Specification.
     */
    unsigned short addr_type;

    /**
     * The target address (aka "hex address") See Table 8 of the GDL Data
     * Interface Specification.
     */
    unsigned int address;

    /**
     * Target latitude in semicircles. See 3.5.1.3 of the GDL Data Interface
     * Specification.
     */
    int phi;

    /**
     * Target longitude in semicircles. See 3.5.1.3 of the GDL Data Interface
     * Specification.
     */
    int lambda;

    /**
     * Target offset altitude. See 3.5.1.4 of the GDL Data Interface
     * Specification.
     */
    unsigned int altitude;

    /**
     * Misc indicators. See 3.5.1.5 of the GDL Data Interface Specification.
     */
    unsigned char misc;

    /**
     * Target nav integrity (NIC). See 3.5.1.6 of the GDL Data Interface
     * Specification.
     */
    unsigned char nav_integrity;

    /**
     * Target nav accuracy (NACp). See 3.5.1.6 of the GDL Data Interface
     * Specification.
     */
    unsigned char nav_accuracy;

    /**
     * Target horizontal velocity. See 3.5.1.7 of the GDL Data Interface
     * Specification.
     */
    short horiz_vel;

    /**
     * Target vertical velocity. See 3.5.1.8 of the GDL Data Interface
     * Specification.
     */
    short vert_vel;

    /**
     * Either target track or heading. See 3.5.1.9 of the GDL Data Interface
     * Specification.
     */
    short track;

    /**
     * Target type. See 3.5.1.10 of the GDL Data Interface Specification.
     */
    unsigned char emitter;

    /**
     * Target ASCII call sign. See 3.5.1.11 of the GDL Data Interface
     * Specification.
     */
    char call_sign[8];

    /**
     * Emergency or priority code. See 3.5.1.12 of the GDL Data Interface
     * Specification.
     */
    unsigned char priority;
    };

  /**
   * The current temporary own ship data.
   */
  TrafficStruct own_ship;

  /**
   * The current temporary target data;
   */
  TrafficStruct target;

  /**
   * Decoded own ship.
   */
  GeoPoint own_ship_position;

  /**
   * Decoded own ship.
   */
  double own_ship_altitude;

  /**
   * Generate the FCS table.
   */
  void Initfcs();

  /**
   * Calculate the CRC of the input vector and compare.
   * @param s The input vector.
   * @return If the CRC computed on s is equal to the FCS of s then true.
   */
  bool Checkfcs(std::span<const std::byte> s);

  /**
   * Check the packet consistency. Do this before anything else.
   * @param s The packet to check.
   * @return If the packet is within spec. then true.
   */
  bool IsPacketConsistent(std::span<const std::byte> s) const;

  /**
   * Cut the UDP packet into a number of ADSB packets. It is assumed that
   * a single ADSB packet does not traverse across multiple UDP packets.
   * @param s The UDP packet containing one or more ADSB packets.
   * @return All the found ADSB packets. There may be zero ADSB packets 
   *         returned.
   */
   std::vector<std::span<const std::byte> >CutPacket(std::span<const std::byte> s) const;

  /**
   * Compute the signed value of a raw unsigned integer.
   * @param x The raw input.
   * @param n the number of significant bits of x.
   * @return The signed representation of n.
   */
  int TwosComplement(unsigned int x, int n);

  /**
   * Parse the input into a temporary form.
   * @param s The input (and destuffed) vector.
   * @param t The output structure into to which s is parsed.
   */
  void Traffic(std::span<const std::byte> s, TrafficStruct &t);
    
  /**
   * Time since midnight from HEARTBEAT.
   */
  unsigned long int time_of_day;

  /**
   * The Port device receiving our data.
   */
  Port &port;
  };

/**
 * The registration of this driver to the XCSoar system.
 */
extern const struct DeviceRegister skyecho_driver;

#endif  //XCSOAR_SKYECHO_DEVICE_HPP
