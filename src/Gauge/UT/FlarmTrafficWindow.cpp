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

#include "Gauge/FlarmTrafficWindow.hpp"
#include "Look/FlarmTrafficLook.hpp"
#include "Surveillance/RemoteTarget.hpp"
#include "ui/canvas/Color.hpp"
#include "ui/canvas/Brush.hpp"
#include "ui/canvas/Font.hpp"
#include "Screen/Layout.hpp"
#include "Look/FontDescription.hpp"
#include "Math/Angle.hpp"

#include <iostream>
#include <stdlib.h>

/**
 * \cond NEVER
 */

/**
 * A test class which gives access to FlarmTrafficWindow internal variables.
 */
class TestFlarmTrafficWindow : public FlarmTrafficWindow
  {
public:
  TestFlarmTrafficWindow(const FlarmTrafficLook& look,
                         unsigned h_padding,
                         unsigned v_padding,
                         bool small = false)
    : FlarmTrafficWindow(look, h_padding, v_padding, small)
    {
    }

  TargetList& List()
    {
    return this->traffic;
    }

  int flarm_selection_index() const
    {
    return this->flarm_selection;
    }

  int adsb_selection_index() const
    {
    return this->adsb_selection;
    }

  };

//------------------------------------------------------------------------------
void
populate_targets(FlarmTarget& flarm_target_1,
                 FlarmTarget& flarm_target_2,
                 AdsbTarget&  adsb_target_1,
                 AdsbTarget&  adsb_target_2)
  {
  flarm_target_1.location_available          = true;
  flarm_target_1.track_received              = true;
  flarm_target_1.altitude_available          = true;
  flarm_target_1.climb_rate_received         = true;
  flarm_target_1.climb_rate_avg30s_available = false;
  flarm_target_1.valid = Validity(TimeStamp(FloatDuration(1.0)));
  flarm_target_1.location = GeoPoint(Angle(Angle::DMS(1, 0, 0)),
                                     Angle(Angle::DMS(1, 0, 0)));
  flarm_target_1.distance                    = 1000.0;
  flarm_target_1.track                       = Angle::DMS(100, 0, 0);
  flarm_target_1.speed                       = 50.0;
  flarm_target_1.altitude                    = 1000.0;
  flarm_target_1.relative_altitude           = 1.0;
  flarm_target_1.turn_rate                   = 1.0;
  flarm_target_1.climb_rate                  = 1.0;
  flarm_target_1.relative_north              = 707.0;
  flarm_target_1.relative_east               = 707.0;
  flarm_target_1.id.Arg(0x100000);
  flarm_target_1.alarm_level                 = RemoteTarget::AlarmType::NONE;
  flarm_target_1.climb_rate_avg30s           = 1.0;
  flarm_target_1.stealth                     = false;
  flarm_target_2.location_available          = true;
  flarm_target_2.track_received              = true;
  flarm_target_2.altitude_available          = true;
  flarm_target_2.climb_rate_received         = true;
  flarm_target_2.climb_rate_avg30s_available = false;
  flarm_target_2.valid = Validity(TimeStamp(FloatDuration(1.0)));
  flarm_target_2.location = GeoPoint(Angle(Angle::DMS(1, 0, 0)),
                                     Angle(Angle::DMS(1, 0, 0)));
  flarm_target_2.distance                    = 1000.0;
  flarm_target_2.track                       = Angle::DMS(100, 0, 0);
  flarm_target_2.speed                       = 50.0;
  flarm_target_2.altitude                    = 1000.0;
  flarm_target_2.relative_altitude           = 1.0;
  flarm_target_2.turn_rate                   = 1.0;
  flarm_target_2.climb_rate                  = 1.0;
  flarm_target_2.relative_north              = 707.0;
  flarm_target_2.relative_east               = 707.0;
  flarm_target_2.id.Arg(0x200000);
  flarm_target_2.alarm_level                 = RemoteTarget::AlarmType::NONE;
  flarm_target_2.climb_rate_avg30s           = 1.0;
  flarm_target_2.stealth                     = false;
  adsb_target_1.location_available           = true;
  adsb_target_1.track_received               = true;
  adsb_target_1.altitude_available           = true;
  adsb_target_1.climb_rate_received          = true;
  adsb_target_1.climb_rate_avg30s_available  = false;
  adsb_target_1.valid = Validity(TimeStamp(FloatDuration(1.0)));
  adsb_target_1.location = GeoPoint(Angle(Angle::DMS(1, 0, 0)),
                                     Angle(Angle::DMS(1, 0, 0)));
  adsb_target_1.distance                     = 1000.0;
  adsb_target_1.track                        = Angle::DMS(100, 0, 0);
  adsb_target_1.speed                        = 50.0;
  adsb_target_1.altitude                     = 1000.0;
  adsb_target_1.relative_altitude            = 1.0;
  adsb_target_1.turn_rate                    = 1.0;
  adsb_target_1.climb_rate                   = 1.0;
  adsb_target_1.relative_north               = 707.0;
  adsb_target_1.relative_east                = 707.0;
  adsb_target_1.id.Arg(0x300000);
  adsb_target_1.alarm_level                  = RemoteTarget::AlarmType::NONE;
  adsb_target_1.climb_rate_avg30s            = 1.0;
  adsb_target_2.location_available           = true;
  adsb_target_2.track_received               = true;
  adsb_target_2.altitude_available           = true;
  adsb_target_2.climb_rate_received          = true;
  adsb_target_2.climb_rate_avg30s_available  = false;
  adsb_target_2.valid = Validity(TimeStamp(FloatDuration(1.0)));
  adsb_target_2.location = GeoPoint(Angle(Angle::DMS(1, 0, 0)),
                                     Angle(Angle::DMS(1, 0, 0)));
  adsb_target_2.distance                     = 1000.0;
  adsb_target_2.track                        = Angle::DMS(100, 0, 0);
  adsb_target_2.speed                        = 50.0;
  adsb_target_2.altitude                     = 1000.0;
  adsb_target_2.relative_altitude            = 1.0;
  adsb_target_2.turn_rate                    = 1.0;
  adsb_target_2.climb_rate                   = 1.0;
  adsb_target_2.relative_north               = 707.0;
  adsb_target_2.relative_east                = 707.0;
  adsb_target_2.id.Arg(0x400000);
  adsb_target_2.alarm_level                  = RemoteTarget::AlarmType::NONE;
  adsb_target_2.climb_rate_avg30s            = 1.0;
  }

//------------------------------------------------------------------------------
bool
Test_List_Basic_Semantics(TestFlarmTrafficWindow& window)
  {
  TargetList& list = window.List();
  FlarmTarget flarm_target_1;
  FlarmTarget flarm_target_2;
  AdsbTarget  adsb_target_1;
  AdsbTarget  adsb_target_2;

  populate_targets(flarm_target_1,
                   flarm_target_2,
                   adsb_target_1,
                   adsb_target_2);
  list.AddFlarmTarget(flarm_target_1);
  list.AddFlarmTarget(flarm_target_2);
  list.AddAdsbTarget(adsb_target_1);
  list.AddAdsbTarget(adsb_target_2);

  window.SetTarget(&flarm_target_1);
  const RemoteTarget* rt;
  rt = window.GetTarget();
  if (rt == nullptr)
    {
    std::cerr << "GetTarget() should not be nullptr\n";
    return false;
    }
  const FlarmTarget* ft;
  ft = dynamic_cast<const FlarmTarget*>(rt);
  if (ft == nullptr)
    {
    std::cerr << "GetTarget() should be a FlarmTarget\n";
    return false;
    }
  const AdsbTarget* at;
  at = dynamic_cast<const AdsbTarget*>(rt);
  if (at != nullptr)
    {
    std::cerr << "GetTarget() should not be a AdsbTarget\n";
    return false;
    }
  if (rt->id.Arg() != 0x100000)
    {
    std::cerr << "First target id = 0x" << std::hex << rt->id.Arg()
              << std::dec << " but should be 0x100000\n";
    return false;
    }
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "{flarm_selection, adsb_selection} != {0, -1}\n";
    return false;
    }
//---
  window.NextTarget();
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if (rt->id.Arg() != 0x200000)
    {
    std::cerr << "Target id = 0x" << std::hex << rt->id.Arg()
              << std::dec << " but should be 0x200000\n";
    return false;
    }
  if ((window.flarm_selection_index() != 1) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "{flarm_selection, adsb_selection} != {1, -1}\n";
    return false;
    }
//---
  window.NextTarget();
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if (rt->id.Arg() != 0x300000)
    {
    std::cerr << "Target id = 0x" << std::hex << rt->id.Arg()
              << std::dec << " but should be 0x300000\n";
    return false;
    }
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 0))
    {
    std::cerr << "{flarm_selection, adsb_selection} != {-1, 0}\n";
    return false;
    }
//---
  window.NextTarget();
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if (rt->id.Arg() != 0x400000)
    {
    std::cerr << "Target id = 0x" << std::hex << rt->id.Arg()
              << std::dec << " but should be 0x400000\n";
    return false;
    }
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 1))
    {
    std::cerr << "{flarm_selection, adsb_selection} != {0, -1}\n";
    return false;
    }
//---
  return true;
  }

//------------------------------------------------------------------------------
bool
Test_List_Dynamic_Semantics_1(TestFlarmTrafficWindow& window)
  {
  TargetList& list = window.List();
  FlarmTarget flarm_target_1;
  FlarmTarget flarm_target_2;
  AdsbTarget  adsb_target_1;
  AdsbTarget  adsb_target_2;
  const RemoteTarget* rt;
  const FlarmTarget*  ft;
//  const AdsbTarget*   at;

  populate_targets(flarm_target_1,
                   flarm_target_2,
                   adsb_target_1,
                   adsb_target_2);
#if 0
  list.AddFlarmTarget(flarm_target_1);
  list.AddFlarmTarget(flarm_target_2);
  list.AddAdsbTarget(adsb_target_1);
  list.AddAdsbTarget(adsb_target_2);
#endif

  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Both flarm_selection and adsb_selection should be -1 but are not!\n";
    return false;
    }

  list.AddFlarmTarget(flarm_target_1);
  window.NextTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "flarm_selection should be == 0 but == " 
              << window.flarm_selection_index()
              << "\n";
    return false;
    }
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if ((ft = dynamic_cast<const FlarmTarget*>(rt)) == nullptr)
    {
    std::cerr << "Looking for a FlarmTarget but got nullptr\n";
    return false;
    }
  if (ft->id.Arg() != 0x100000)
    {
    std::cerr << "Expecting Id 0x100000 but got something else\n";
    return false;
    }
  window.NextTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {0, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  list.AddFlarmTarget(flarm_target_2);
  window.NextTarget();
  if ((window.flarm_selection_index() != 1) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {1, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  window.NextTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {1, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  return true;
  }

//------------------------------------------------------------------------------
bool
Test_List_Dynamic_Semantics_2(TestFlarmTrafficWindow& window)
  {
  TargetList& list = window.List();
  FlarmTarget flarm_target_1;
  FlarmTarget flarm_target_2;
  AdsbTarget  adsb_target_1;
  AdsbTarget  adsb_target_2;
  const RemoteTarget* rt;
//  const FlarmTarget*  ft;
  const AdsbTarget*   at;

  populate_targets(flarm_target_1,
                   flarm_target_2,
                   adsb_target_1,
                   adsb_target_2);
#if 0
  list.AddFlarmTarget(flarm_target_1);
  list.AddFlarmTarget(flarm_target_2);
  list.AddAdsbTarget(adsb_target_1);
  list.AddAdsbTarget(adsb_target_2);
#endif

  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Both flarm_selection and adsb_selection should be -1 but are not!\n";
    return false;
    }

  list.AddAdsbTarget(adsb_target_1);
  window.NextTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 0))
    {
    std::cerr << "adsb_selection should be == 0 but == " 
              << window.adsb_selection_index()
              << "\n";
    return false;
    }
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if ((at = dynamic_cast<const AdsbTarget*>(rt)) == nullptr)
    {
    std::cerr << "Looking for a AdsbTarget but got nullptr\n";
    return false;
    }
  if (at->id.Arg() != 0x300000)
    {
    std::cerr << "Expecting Id 0x300000 but got something else\n";
    return false;
    }
  window.NextTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 0))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {-1, 0} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  list.AddAdsbTarget(adsb_target_2);
  window.NextTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {-1, 1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  window.NextTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 0))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {-1, 0} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  return true;
  }

//------------------------------------------------------------------------------
bool
Test_List_Dynamic_Semantics_3(TestFlarmTrafficWindow& window)
  {
  TargetList& list = window.List();
  FlarmTarget flarm_target_1;
  FlarmTarget flarm_target_2;
  AdsbTarget  adsb_target_1;
  AdsbTarget  adsb_target_2;
  const RemoteTarget* rt;
  const FlarmTarget*  ft;
//  const AdsbTarget*   at;

  populate_targets(flarm_target_1,
                   flarm_target_2,
                   adsb_target_1,
                   adsb_target_2);
#if 0
  list.AddFlarmTarget(flarm_target_1);
  list.AddFlarmTarget(flarm_target_2);
  list.AddAdsbTarget(adsb_target_1);
  list.AddAdsbTarget(adsb_target_2);
#endif

  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Both flarm_selection and adsb_selection should be -1 but are not!\n";
    return false;
    }

  list.AddFlarmTarget(flarm_target_1);
  window.PrevTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "flarm_selection should be == 0 but == " 
              << window.flarm_selection_index()
              << "\n";
    return false;
    }
  if ((rt = window.GetTarget()) == nullptr)
    {
    std::cerr << "GetTarget() == nullptr but should not be.\n";
    return false;
    }
  if ((ft = dynamic_cast<const FlarmTarget*>(rt)) == nullptr)
    {
    std::cerr << "Looking for a FlarmTarget but got nullptr\n";
    return false;
    }
  if (ft->id.Arg() != 0x100000)
    {
    std::cerr << "Expecting Id 0x100000 but got something else\n";
    return false;
    }
  window.PrevTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {0, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  list.AddFlarmTarget(flarm_target_2);
  window.PrevTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {0, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  window.PrevTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index()  != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {0, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  list.AddAdsbTarget(adsb_target_1);
  window.PrevTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 0))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {-1, 0} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  list.AddAdsbTarget(adsb_target_2);
  window.NextTarget();
  if ((window.flarm_selection_index() != -1) ||
      (window.adsb_selection_index()  != 1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection} == {-1, 1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }
  window.NextTarget();
  if ((window.flarm_selection_index() != 0) ||
      (window.adsb_selection_index() != -1))
    {
    std::cerr << "Expecting {flarm_selection, adsb_selection == {0, -1} but got {"
              << window.flarm_selection_index() << ", "
              << window.adsb_selection_index()  << "}\n";
    return false;
    }

  return true;
  }

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
int main()
  {
  FlarmTrafficLook look;
  Color red(255, 0, 0);
  Color green(0, 255, 0);
  Color blue(0, 0, 255);
  Color white(255, 255, 255);
  Color black(0, 0, 0);
  Brush default_brush;
  Brush red_brush(red);
  Brush green_brush(green);
  Brush blue_brush(blue);
  Brush white_brush(white);
  Brush black_brush(black);
  
  look.warning_color      = red;
  look.alarm_color        = red;
  look.default_color      = green;
  look.passive_color      = green;
  look.selection_color    = blue;
  look.background_color   = white;
  look.radar_color        = white;
  look.warning_brush      = red_brush;
  look.alarm_brush        = red_brush;
  look.default_brush      = black_brush;
  look.passive_brush      = green_brush;
  look.selection_brush    = blue_brush;
  look.radar_brush        = black_brush;
  look.team_brush_green   = green_brush;
  look.team_brush_blue    = blue_brush;
  look.team_brush_yellow  = Brush(Color(255, 255, 0));
  look.team_brush_magenta = Brush(Color(255, 0, 255));
  look.warning_pen        = Pen(Pen::SOLID, 1, red);
  look.alarm_pen          = Pen(Pen::SOLID, 1, red);
  look.default_pen        = Pen(Pen::SOLID, 1, black);
  look.passive_pen        = Pen(Pen::SOLID, 1, green);
  look.selection_pen      = Pen(Pen::SOLID, 1, blue);
  look.team_pen_green     = Pen(Pen::SOLID, 2, green);
  look.team_pen_blue      = Pen(Pen::SOLID, 2, blue);
  look.team_pen_yellow    = Pen(Pen::SOLID, 2, Color(255, 255, 0));
  look.team_pen_magenta   = Pen(Pen::SOLID, 2, Color(255, 0, 255));
  look.plane_pen          = Pen(Pen::SOLID, 1, black);
  look.radar_pen          = Pen(Pen::SOLID, 1, black);
  look.unit_fraction_pen  = Pen(Pen::SOLID, 1, black);
  look.label_font.Load(FontDescription(Layout::FontScale(12)));
  look.side_info_font.Load(FontDescription(Layout::FontScale(12)));
  look.no_traffic_font.Load(FontDescription(Layout::FontScale(12)));
  look.info_values_font.Load(FontDescription(Layout::FontScale(12)));
  look.info_units_font.Load(FontDescription(Layout::FontScale(12)));
  look.info_labels_font.Load(FontDescription(Layout::FontScale(12)));
  look.call_sign_font.Load(FontDescription(Layout::FontScale(12)));

    {
    TestFlarmTrafficWindow flarm_traffic_window(look, 0, 0, false);

    A_OK(Test_List_Basic_Semantics(flarm_traffic_window));
    }
    {
    TestFlarmTrafficWindow flarm_traffic_window(look, 0, 0, false);
    A_OK(Test_List_Dynamic_Semantics_1(flarm_traffic_window));
    }
    {
    TestFlarmTrafficWindow flarm_traffic_window(look, 0, 0, false);
    A_OK(Test_List_Dynamic_Semantics_2(flarm_traffic_window));
    }
    {
    TestFlarmTrafficWindow flarm_traffic_window(look, 0, 0, false);
    A_OK(Test_List_Dynamic_Semantics_3(flarm_traffic_window));
    }
  

  std::cout << "All went as planned.\n";

  return 0;
  }

/**
 * \endcond
 */
