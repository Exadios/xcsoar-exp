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

/**
 * \cond NEVER
 */
#include "Surveillance/TargetList.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"

//------------------------------------------------------------------------------
void
A_OK(bool status)
  {
  if (status == false)
    {
    std::cout << "Something failed\n";
    exit(1);
    }
  }

//------------------------------------------------------------------------------
bool
FillOutFlarmTargetList(TargetList& list, FlarmTarget& target)
  {
  list.Clear();
  if (list.IsEmpty() == false)
    {
    std::cout << "dst should be empty but is not.\n";
    return false;
    }

  target.location_available          = true;
  target.track_received              = true;
  target.speed_received              = true;
  target.altitude_available          = true;
  target.climb_rate_received         = true;
  target.climb_rate_avg30s_available = true;
  FloatDuration duration(8 * 60 * 60 * 64);  // 08:00
  TimeStamp timestamp(duration);
  target.valid                       = Validity(timestamp);
  Angle::DMS phi(31, 0, 0, true), lambda(115);
  target.location                    = GeoPoint(Angle(lambda), Angle(phi));
  target.distance                    = RoughDistance(1000);
  target.track                       = Angle(Angle::DMS(10));
  target.speed                       = RoughSpeed(5);
  target.altitude                    = RoughAltitude(1000);
  target.relative_altitude           = RoughAltitude(10);
  target.turn_rate                   = 0;
  target.climb_rate                  = 0;
  target.relative_north              = 1000;
  target.relative_east               = 1000;
  target.id                          = TargetId(long(0x10000));
  target.alarm_level                 = RemoteTarget::AlarmType::NONE;
  target.climb_rate_avg30s           = 0;
  target.stealth                     = false;
  target.turn_rate_received          = false;
  target.turn_rate                   = 0.0;

  list.modified                      = Validity(timestamp);
  list.new_traffic                   = Validity(timestamp);
  return true;
  }

//------------------------------------------------------------------------------
bool
FillOutAdsbTargetList(TargetList& list, AdsbTarget& target)
  {
  list.Clear();
  if (list.IsEmpty() == false)
    {
    std::cout << "dst should be empty but is not.\n";
    return false;
    }

  target.location_available          = true;
  target.track_received              = true;
  target.speed_received              = true;
  target.altitude_available          = true;
  target.climb_rate_received         = true;
  target.climb_rate_avg30s_available = true;
  FloatDuration duration(8 * 60 * 60 * 64);  // 08:00
  TimeStamp timestamp(duration);
  target.valid                       = Validity(timestamp);
  Angle::DMS phi(31, 0, 0, true), lambda(115);
  target.location                    = GeoPoint(Angle(lambda), Angle(phi));
  target.distance                    = RoughDistance(1000);
  target.track                       = Angle(Angle::DMS(10));
  target.speed                       = RoughSpeed(5);
  target.altitude                    = RoughAltitude(1000);
  target.relative_altitude           = RoughAltitude(10);
  target.turn_rate                   = 0;
  target.climb_rate                  = 0;
  target.relative_north              = 2000;
  target.relative_east               = 2000;
  target.id                          = TargetId(long(0x20000));
  target.alarm_level                 = RemoteTarget::AlarmType::NONE;
  target.climb_rate_avg30s           = 0;
  target.turn_rate                   = 0.0;

  list.modified                      = Validity(timestamp);
  list.new_traffic                   = Validity(timestamp);
  return true;
  }

//------------------------------------------------------------------------------
bool
Test_1()
  {
  /*
   * Test basic operations of flarm target lists.
   */
  TargetList src, dst;
  FlarmTarget target;

  if (FillOutFlarmTargetList(src, target) == false)
    return false;

  FlarmTarget* sp = src.AddFlarmTarget(target);
  if (sp == nullptr)
    {
    std::cout << "Could not add target 0x10000 to list\n";
    return false;
    }

  dst = src;

  if ((dst.modified != src.modified) || (dst.new_traffic != src.new_traffic))
    {
    std::cout << "dst != src\n";
    return false;
    }

  if (src.GetActiveTrafficCount() != dst.GetActiveTrafficCount())
    {
    std::cout << "GetActiveTrafficCount not equal\n";
    return false;
    }

  if (src.flarm_list.size() != dst.flarm_list.size())
    {
    std::cout << "List sizes are not the same\n";
    return false;
    }

  RemoteTarget* dp = dst.FindTraffic(TargetId(long(0x10000)));
  if (dp == nullptr)
    {
    std::cout << "Target 0x10000 not found in dst\n";
    return false;
    }

  if (dynamic_cast<FlarmTarget*>(dp) == nullptr)
    {
    std::cout << "Could not cast to FlarmTarget*\n";
    return false;
    }
  if (dynamic_cast<AdsbTarget*>(dp) != nullptr)
    {
    std::cout << "Inappropriate cast to AdsbPointer\n";
    return false;
    }

  if ((sp->location_available          != dp->location_available)          ||
      (sp->track_received              != dp->track_received)              ||
      (sp->speed_received              != dp->speed_received)              ||
      (sp->altitude_available          != dp->altitude_available)          ||
      (sp->climb_rate_received         != dp->climb_rate_received)         ||
      (sp->climb_rate_avg30s_available != dp->climb_rate_avg30s_available) ||
      (sp->valid                       != dp->valid)                       ||
      (sp->location                    != dp->location)                    ||
      (sp->distance                    != dp->distance)                    ||
      (sp->track                       != dp->track)                       ||
      (sp->speed                       != dp->speed)                       ||
      (sp->altitude                    != dp->altitude)                    ||
      (sp->relative_altitude           != dp->relative_altitude)           ||
      (sp->turn_rate                   != dp->turn_rate)                   ||
      (sp->climb_rate                  != dp->climb_rate)                  ||
      (sp->relative_north              != dp->relative_north)              ||
      (sp->relative_east               != dp->relative_east)               ||
      (sp->id                          != dp->id)                          ||
      (sp->alarm_level                 != dp->alarm_level)                 ||
      (sp->climb_rate_avg30s           != dp->climb_rate_avg30s))
    {
    std::cout << "Contents of src target not equal contents of dst target\n";
    return false;
    }


  return true;
  }

//------------------------------------------------------------------------------
bool
Test_2()
  {
  /*
   * Test copying and type cast for flarm list.
   */
  TargetList src, dst;
  FlarmTarget target;

  if (FillOutFlarmTargetList(src, target) == false)
    return false;

  FlarmTarget* sp = src.AddFlarmTarget(target);
  if (sp == nullptr)
    {
    std::cout << "Could not add target 0x10000 to list\n";
    return false;
    }
 
  dst = src;

  RemoteTarget* dp = dst.FindTraffic(TargetId(long(0x10000)));
  if (dp == nullptr)
    {
    std::cout << "Target 0x10000 not found in dst\n";
    return false;
    }

  if (dynamic_cast<FlarmTarget*>(dp) == nullptr)
    {
    std::cout << "Could not cast to FlarmTarget*\n";
    return false;
    }
  if (dynamic_cast<AdsbTarget*>(dp) != nullptr)
    {
    std::cout << "Inappropriate cast to AdsbTarget\n";
    return false;
    }

  while (sp->valid.IsValid() == true)
    sp->Refresh(TimeStamp(FloatDuration((8 * 60 * 60 + 5) * 64))); // 08:00:05

  if (sp->valid.IsValid() == true)
    {
    std::cout << "sp is valid in Test_2()\n";
    return false;
    }

  return true;
  }

//------------------------------------------------------------------------------
bool
Test_3()
  {
  /*
   * Test basic operations of adsb target lists.
   */
  TargetList src, dst;
  AdsbTarget target;

  if (FillOutAdsbTargetList(src, target) == false)
    return false;

  AdsbTarget* sp = src.AddAdsbTarget(target);
  if (sp == nullptr)
    {
    std::cout << "Could not add target 0x10000 to list\n";
    return false;
    }

  dst = src;

  if ((dst.modified != src.modified) || (dst.new_traffic != src.new_traffic))
    {
    std::cout << "dst != src\n";
    return false;
    }

  if (src.GetActiveTrafficCount() != dst.GetActiveTrafficCount())
    {
    std::cout << "GetActiveTrafficCount not equal\n";
    return false;
    }

  if (src.flarm_list.size() != dst.flarm_list.size())
    {
    std::cout << "List sizes are not the same\n";
    return false;
    }

  RemoteTarget* dp = dst.FindTraffic(TargetId(long(0x20000)));
  if (dp == nullptr)
    {
    std::cout << "Target 0x10000 not found in dst\n";
    return false;
    }

  if (dynamic_cast<AdsbTarget*>(dp) == nullptr)
    {
    std::cout << "Could not cast to AdsbTarget*\n";
    return false;
    }
  if (dynamic_cast<FlarmTarget*>(dp) != nullptr)
    {
    std::cout << "Inappropriate cast to FlarmTarget\n";
    return false;
    }

  if ((sp->location_available          != dp->location_available)          ||
      (sp->track_received              != dp->track_received)              ||
      (sp->speed_received              != dp->speed_received)              ||
      (sp->altitude_available          != dp->altitude_available)          ||
      (sp->climb_rate_received         != dp->climb_rate_received)         ||
      (sp->climb_rate_avg30s_available != dp->climb_rate_avg30s_available) ||
      (sp->valid                       != dp->valid)                       ||
      (sp->location                    != dp->location)                    ||
      (sp->distance                    != dp->distance)                    ||
      (sp->track                       != dp->track)                       ||
      (sp->speed                       != dp->speed)                       ||
      (sp->altitude                    != dp->altitude)                    ||
      (sp->relative_altitude           != dp->relative_altitude)           ||
      (sp->turn_rate                   != dp->turn_rate)                   ||
      (sp->climb_rate                  != dp->climb_rate)                  ||
      (sp->relative_north              != dp->relative_north)              ||
      (sp->relative_east               != dp->relative_east)               ||
      (sp->id                          != dp->id)                          ||
      (sp->alarm_level                 != dp->alarm_level)                 ||
      (sp->climb_rate_avg30s           != dp->climb_rate_avg30s))
    {
    std::cout << "Contents of src target not equal contents of dst target\n";
    return false;
    }

  return true;
  }

//------------------------------------------------------------------------------
bool
Test_4()
  {
  /*
   * Test copying and type cast for adsb list.
   */

  TargetList src, dst;
  AdsbTarget target;

  if (FillOutAdsbTargetList(src, target) == false)
    return false;

  AdsbTarget* sp = src.AddAdsbTarget(target);
  if (sp == nullptr)
    {
    std::cout << "Could not add target 0x10000 to list\n";
    return false;
    }
 
  dst = src;

  RemoteTarget* dp = dst.FindTraffic(TargetId(long(0x20000)));
  if (dp == nullptr)
    {
    std::cout << "Target 0x10000 not found in dst\n";
    return false;
    }

  if (dynamic_cast<AdsbTarget*>(dp) == nullptr)
    {
    std::cout << "Could not cast to AdsbTarget*\n";
    return false;
    }
  if (dynamic_cast<FlarmTarget*>(dp) != nullptr)
    {
    std::cout << "Inappropriate cast to FlarmTarget\n";
    return false;
    }

  while (sp->valid.IsValid() == true)
    sp->Refresh(TimeStamp(FloatDuration((8 * 60 * 60 + 5) * 64))); // 08:00:05

  if (sp->valid.IsValid() == true)
    {
    std::cout << "sp is valid in Test_4()\n";
    return false;
    }

  return true;
  }

//------------------------------------------------------------------------------
bool
Test_5()
  {
  /*
   * Test targets in both flarm and adsb list.
   */
  TargetList uut;
  AdsbTarget adsb_target;
  FlarmTarget flarm_target;

  FillOutAdsbTargetList(uut, adsb_target);
  FillOutFlarmTargetList(uut, flarm_target);
  AdsbTarget*  ap = uut.AddAdsbTarget(adsb_target);
  FlarmTarget* fp = uut.AddFlarmTarget(flarm_target);
  if ((ap == nullptr) || (fp == nullptr))
    {
    std::cout << "Either Adsb or Flarm target failed to insert\n.";
    return false;
    }

  /* Is the count correct? */
  unsigned tc = uut.GetActiveTrafficCount();
  if (tc != 2)
    {
    std::cout << "Wrong active traffic count (" << tc << ")\n";
    return false;
    }

  /* Are the returned RemoteTarget correctly FlarmTarget and AdsbTarget? */
  RemoteTarget* ff = uut.FindTraffic(TargetId((long)0x10000));
  RemoteTarget* fa = uut.FindTraffic(TargetId((long)0x20000));
  if ((fa == nullptr) || (ff == nullptr))
    {
    std::cout << "Either flarm or adsb target not found\n";
    return false;
    }

  if (dynamic_cast<FlarmTarget*>(ff) == nullptr)
    {
    std::cout << "Found target was not a FlarmTarget*\n";
    return false;
    }
  if (dynamic_cast<AdsbTarget*>(fa) == nullptr)
    {
    std::cout << "Found target was not an AdsbTarget*\n";
    return false;
    }

  /* Does FirstTraffic and LastTraffic() work and does it return the correct
   * RemoteTarget? */
  const RemoteTarget* ft;
  const RemoteTarget* lt;
  if ((ft = uut.FirstTraffic()) == nullptr)
    {
    std::cout << "FirstTraffic() returns nullptr\n";
    return false;
    }
  if((lt = uut.LastTraffic()) == nullptr)
    {
    std::cout << "LastTraffic() returns nullptr\n";
    return false;
    }
  if (ft->id.Arg() != 0x10000)
    {
    std::cout << "Incorrect id for FirstTraffic()\n";
    return false;
    }
  if (lt->id.Arg() != 0x20000)
    {
    std::cout << "Incorrect id for LastTraffic()\n";
    return false;
    }

  /* Start in flarm and end in adsb. */
  if (dynamic_cast<const FlarmTarget*>(ft) == nullptr)
    {
    std::cout << "ft should be a flarm target\n";
    return false;
    }
  lt = uut.NextTraffic(ft);
  if (dynamic_cast<const AdsbTarget*>(lt) == nullptr)
    {
    std::cout << "NextTraffic() should be adsb but is not\n";
    return false;
    }
  if ((ft = uut.FirstTraffic()) == nullptr)
    {
    std::cout << "FirstTraffic() returns nullptr\n";
    return false;
    }
  if((lt = uut.LastTraffic()) == nullptr)
    {
    std::cout << "LastTraffic() returns nullptr\n";
    return false;
    }
  /* Start in adsb and end in flarm. */
  ft = uut.PreviousTraffic(lt);
  if (dynamic_cast<const FlarmTarget*>(ft) == nullptr)
    {
    std::cout << "PreviousTraffic() should be flarm but was not\n";
    return false;
    }

  return true;
  }

//------------------------------------------------------------------------------
int
main()
  {
  A_OK(Test_1());
  A_OK(Test_2());
  A_OK(Test_3());
  A_OK(Test_4());
  A_OK(Test_5());

  std::cout << "All went as planned.\n";
  return 0;
  }

/**
 * \endcond
 */
