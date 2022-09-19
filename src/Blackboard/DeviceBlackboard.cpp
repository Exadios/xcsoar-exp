/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
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

#include "Blackboard/DeviceBlackboard.hpp"
#include "Protection.hpp"
#include "Device/MultipleDevices.hpp"
#include "Simulator.hpp"
#include "RadioFrequency.hpp"

#include <algorithm>

#include "LogFile.hpp"

#ifndef NDEBUG
#include "Components.hpp"
#endif

/**
 * Initializes the DeviceBlackboard
 */
DeviceBlackboard::DeviceBlackboard()
{
#ifndef NDEBUG
#include <stdio.h>
  printf("%s, %d\n", __FILE__, __LINE__);
#endif
  
  // Clear the gps_info and calculated_info
  gps_info.Reset();
  calculated_info.Reset();

  // Set GPS assumed time to system time
  gps_info.UpdateClock();
  gps_info.date_time_utc = BrokenDateTime::NowUTC();
  gps_info.time = TimeStamp{gps_info.date_time_utc.DurationSinceMidnight()};

  std::fill_n(per_device_data, unsigned(NUMDEV), gps_info);

  real_data = simulator_data = replay_data = gps_info;

  simulator.Init(simulator_data);

  real_clock.Reset();
  replay_clock.Reset();
}

/**
 * Sets the location and altitude to loc and alt
 *
 * Called at startup when no gps data available yet
 * @param loc New location
 * @param alt New altitude
 */
void
DeviceBlackboard::SetStartupLocation(const GeoPoint &loc, const double alt)
{
  std::lock_guard<Mutex> lock(mutex);

  if (Calculated().flight.flying)
    return;

  for (unsigned i = 0; i < unsigned(NUMDEV); ++i)
    if (!per_device_data[i].location_available)
      per_device_data[i].SetFakeLocation(loc, alt);

  if (!real_data.location_available)
    real_data.SetFakeLocation(loc, alt);

  if (is_simulator()) {
    simulator_data.SetFakeLocation(loc, alt);
    simulator.Touch(simulator_data);
  }

  ScheduleMerge();
}

/**
 * Stops the replay
 */
void DeviceBlackboard::StopReplay() {
  std::lock_guard<Mutex> lock(mutex);

  replay_data.Reset();

  ScheduleMerge();
}

void
DeviceBlackboard::ProcessSimulation()
{
  if (!is_simulator())
    return;

  std::lock_guard<Mutex> lock(mutex);

  simulator.Process(simulator_data);
  ScheduleMerge();
}

void
DeviceBlackboard::SetSimulatorLocation(const GeoPoint &location)
{
  std::lock_guard<Mutex> lock(mutex);
  NMEAInfo &basic = simulator_data;

  simulator.Touch(basic);
  basic.track = location.Bearing(basic.location).Reciprocal();
  basic.location = location;

  ScheduleMerge();
}

/**
 * Sets the GPS speed and indicated airspeed to val
 *
 * not in use
 * @param val New speed
 */
void
DeviceBlackboard::SetSpeed(double val)
{
  std::lock_guard<Mutex> lock(mutex);
  NMEAInfo &basic = simulator_data;

  simulator.Touch(basic);
  basic.ground_speed = val;

  ScheduleMerge();
}

/**
 * Sets the TrackBearing to val
 *
 * not in use
 * @param val New TrackBearing
 */
void
DeviceBlackboard::SetTrack(Angle val)
{
  std::lock_guard<Mutex> lock(mutex);
  simulator.Touch(simulator_data);
  simulator_data.track = val.AsBearing();

  ScheduleMerge();
}

/**
 * Sets the altitude and barometric altitude to val
 *
 * not in use
 * @param val New altitude
 */
void
DeviceBlackboard::SetAltitude(double val)
{
  std::lock_guard<Mutex> lock(mutex);
  NMEAInfo &basic = simulator_data;

  simulator.Touch(basic);
  basic.gps_altitude = val;

  ScheduleMerge();
}

/**
 * Reads the given derived_info usually provided by the
 * GlideComputerBlackboard and saves it to the own Blackboard
 * @param derived_info Calculated information usually provided
 * by the GlideComputerBlackboard
 */
void
DeviceBlackboard::ReadBlackboard(const DerivedInfo &derived_info)
{
  calculated_info = derived_info;
}

/**
 * Reads the given settings usually provided by the InterfaceBlackboard
 * and saves it to the own Blackboard
 * @param settings ComputerSettings usually provided by the
 * InterfaceBlackboard
 */
void
DeviceBlackboard::ReadComputerSettings(const ComputerSettings &settings)
{
  computer_settings = settings;
}

void
DeviceBlackboard::ExpireWallClock()
{
  std::lock_guard<Mutex> lock(mutex);
  if (!Basic().alive)
    return;

  bool modified = false;
  for (unsigned i = 0; i < unsigned(NUMDEV); ++i) {
    NMEAInfo &basic = per_device_data[i];
    if (!basic.alive)
      continue;

    basic.ExpireWallClock();
    if (!basic.alive)
      modified = true;
  }

  if (modified)
    ScheduleMerge();
}

void
DeviceBlackboard::ScheduleMerge()
{
#ifndef NDEBUG
#include "LogFile.hpp"
  LogFormat("%s, %d: %lu", __FILE__, __LINE__,
            ::device_blackboard->RealState(0).adsb.traffic.list.size());
#endif
  TriggerMergeThread();
}

void
DeviceBlackboard::Merge()
{
  /*
   * The system wide DeviceBlackboard object is pointed to by
   * ::device_blackboard.
   * First merge the data from the devices present in the
   * ::device_blackboard \ref per_device_data into
   * ::device_blackboard \ref real_data.
   * Then, if ::device_blackboard \ref replay_data is alive,
   * copy that data to ::device_blackboard \ref gps_info.
   * If ::device_blackboard \ref simulator_data is alive 
   * then copy that data to ::device_blackboard \ref gps_info.
   * Otherwise copy the \ref real_data gathered above to \ref gps_info.
   *
   * With respect to the semantics of the \ref real_data being processed
   * the merge from the \per_device_info is done from lowest configured 
   * device to the highest using NMEAInfo::Complement. This has the effect
   * of selecting the value of each domain from the lowest number device
   * for further processing.
   */

  /*
   * The output of this function is ::device_blackboard->gps_info.
   */
  NMEAInfo &basic = SetBasic();

#ifndef NDEBUG
#include "LogFile.hpp"
  LogFormat("%s, %d: %lu", __FILE__, __LINE__,
            ::device_blackboard->RealState(0).adsb.traffic.list.size());
#endif

  real_data.Reset();  // Start with no data in real_data.
  for (unsigned i = 0; i < unsigned(NUMDEV); ++i) {
    if (!per_device_data[i].alive)
      continue;

    /*
     * Merge input data to real_data. By the semantics of NMEAInfo::Complement
     * each variable of a domain is taken from the first available device
     * that provides variables of that domain.
     */
#ifndef NDEBUG
#include "LogFile.hpp"
    LogFormat("%s, %d: %lu", __FILE__, __LINE__,
              per_device_data[0].adsb.traffic.list.size());
#endif
    per_device_data[i].UpdateClock();
    per_device_data[i].Expire();
#ifndef NDEBUG
#include "LogFile.hpp"
    LogFormat("%s, %d: %lu", __FILE__, __LINE__,
              per_device_data[0].adsb.traffic.list.size());
#endif
    real_data.Complement(per_device_data[i]); 
#ifndef NDEBUG
#include "LogFile.hpp"
    LogFormat("%s, %d: %lu", __FILE__, __LINE__,
              real_data.adsb.traffic.list.size());
#endif
  }

  real_clock.Normalise(real_data);

  /*
   * Select one of replay, simulator or real data to pass on for further
   * processing.
   */
  if (replay_data.alive) {
    replay_data.Expire();
    basic = replay_data;    // Copy replay_data to gps_info

    /* WrapClock operates on the replay_data copy to avoid feeding
       back BrokenDate modifications to the NMEA parser, as this would
       trigger its time warp checks */
    replay_clock.Normalise(basic);
  } else if (simulator_data.alive) {
    simulator_data.UpdateClock();
    simulator_data.Expire();
    basic = simulator_data;   // Copy simulator_data to gps_info
  } else {
    basic = real_data;    // Copy real_data to gps_info
  }
}

void
DeviceBlackboard::SetBallast(double fraction, double overload,
                             OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutBallast(fraction, overload, env);
}

void
DeviceBlackboard::SetBugs(double bugs, OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutBugs(bugs, env);
}

void
DeviceBlackboard::SetQNH(AtmosphericPressure qnh, OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutQNH(qnh, env);
}

void
DeviceBlackboard::SetMC(double mc, OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutMacCready(mc, env);
}

void
DeviceBlackboard::SetActiveFrequency(RadioFrequency frequency,
                                     const TCHAR *name,
                                     OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutActiveFrequency(frequency, name, env);
}

void
DeviceBlackboard::SetStandbyFrequency(RadioFrequency frequency,
                                      const TCHAR *name,
                                      OperationEnvironment &env)
{
  if (devices != nullptr)
    devices->PutStandbyFrequency(frequency, name, env);
}
