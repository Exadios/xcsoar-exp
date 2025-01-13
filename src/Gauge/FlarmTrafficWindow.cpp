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

#include "FlarmTrafficWindow.hpp"
#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/TargetFriends.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "Formatter/UserUnits.hpp"
#include "Units/Units.hpp"
#include "Math/Screen.hpp"
#include "Language/Language.hpp"
#include "util/Macros.hpp"
#include "Look/FlarmTrafficLook.hpp"

#include <algorithm>

#include <xcsoar-cassert>
#include <stdio.h>

#ifdef ENABLE_OPENGL
#include "ui/canvas/opengl/Scope.hpp"
#endif

#ifndef NDEBUG
#include "Blackboard/DeviceBlackboard.hpp"
#include "Components.hpp"
#endif

//------------------------------------------------------------------------------
FlarmTrafficWindow::FlarmTrafficWindow(const FlarmTrafficLook &look,
                                       unsigned h_padding,
                                       unsigned v_padding,
                                       bool small) noexcept
  : look(look),
    h_padding(h_padding), v_padding(v_padding),
    small(small)
  {
  this->traffic.Clear();
  this->data_modified.Clear();
  this->target_status.Clear();
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficWindow::WarningMode() const noexcept
  {
  assert(warning < (int)this->traffic.flarm_list.size());
  assert(warning < 0 || this->traffic.flarm_list[warning].IsDefined());
  assert(warning < 0 || this->traffic.flarm_list[warning].HasAlarm());

  return this->warning >= 0;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::OnResize(PixelSize new_size) noexcept
  {
  PaintWindow::OnResize(new_size);

  const unsigned half_width = new_size.width / 2;
  const unsigned half_height = new_size.height / 2;

  // Calculate Radar size
  this->radius = std::min(half_width - h_padding, half_height - v_padding);
  radar_mid.x = half_width;
  radar_mid.y = half_height;
  }

//-----------------------------------------------------------------------------
bool
FlarmTrafficWindow::NextFlarmTarget() noexcept
  {
  assert(this->flarm_selection < (int)this->traffic.flarm_list.size());

  const RemoteTarget* target;
  if (this->flarm_selection >= 0)
    target = this->traffic.NextFlarmTraffic(&this->traffic.flarm_list[this->flarm_selection]);
  else
    {
    target = nullptr;
    this->flarm_selection = -1;
    }

  if (nullptr == target)
    {
    if (this->flarm_selection >= 0)
      {
      this->SetFlarmTarget(dynamic_cast<const FlarmTarget*>(target)); 
      /*
       * At this point we have attempted to retrieve a target which is beyond
       * the current end of the list, i.e. no FLARM target exists. A call to 
       * this->SetFlarmTarget(nullptr) will set this->flarm_selection = -1
       * which will signify that we are finished with the FLARM targets for now.
       */
      return false;
      }
    else
      {
      target = this->traffic.FirstFlarmTraffic();
      this->flarm_selection = 0;
      this->SetFlarmTarget(static_cast<const FlarmTarget*>(target));
      }
    }
  /*
   * If there are no FLARM targets then \ref SetFlarmTarget will be called
   * with nullptr resulting in this->flarm_selection will be set to -1.
   */
  this->SetFlarmTarget(dynamic_cast<const FlarmTarget*>(target));
  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficWindow::PrevFlarmTarget() noexcept
  {
  assert(this->flarm_selection < (int)this->traffic.flarm_list.size());

  const FlarmTarget* target;
  if (this->flarm_selection >= 0)
    target = this->traffic.PreviousFlarmTraffic(&this->traffic.flarm_list[this->flarm_selection]);
  else
    {
    target = nullptr;
    this->flarm_selection = -1;
    }

  if (nullptr == target)
    {
    if (this->flarm_selection >= 0)
      {
      this->SetFlarmTarget(target);
      /*
       * At this point we have attempted to retrieve a target which is before
       * the current beginning of the list, i.e. no FLARM target exists. A
       * call to this->SetFlarmTarget(nullptr) will set 
       * this->flarm_selection = -1 which will signify that we are finished
       * with the FLARM targets for now.
       */
      return false;
      }
    else
      {
      target = this->traffic.LastFlarmTraffic();
      this->flarm_selection = (int)this->traffic.flarm_list.size() - 1;
      this->SetFlarmTarget(target);
      }
    }
  return true;
  }

//-----------------------------------------------------------------------------
bool
FlarmTrafficWindow::NextAdsbTarget() noexcept
  {
  assert(this->adsb_selection < (int)this->traffic.adsb_list.size());

  const AdsbTarget* target;
  if (this->adsb_selection >= 0)
    target = this->traffic.NextAdsbTraffic(&this->traffic.adsb_list[this->adsb_selection]);
  else
    {
    target = nullptr;
    this->adsb_selection = -1;
    }

  if (nullptr == target)
    {
    if (this->adsb_selection >= 0)
      {
      this->SetAdsbTarget(dynamic_cast<const AdsbTarget*>(target)); 
      /*
       * At this point we have attempted to retrieve a target which is beyond
       * the current end of the list, i.e. no ADSB target exists. A call to 
       * this->SetAdsbTarget(nullptr) will set this->adsb_selection = -1
       * which will signify that we are finished with the ADSB targets for now.
       */
      return false;
      }
    else
      {
      target = this->traffic.FirstAdsbTraffic();
      this->adsb_selection = 0;
      this->SetAdsbTarget(static_cast<const AdsbTarget*>(target));
      }
    }
  /*
   * If there are no ADSB targets then \ref SetAdsbTarget will be called
   * with nullptr resulting in this->adsb_selection will be set to -1.
   */
  this->SetAdsbTarget(dynamic_cast<const AdsbTarget*>(target));
  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficWindow::PrevAdsbTarget() noexcept
  {
  assert(this->adsb_selection < (int)this->traffic.adsb_list.size());

  const AdsbTarget* target;
  if (this->adsb_selection >= 0)
    target = this->traffic.PreviousAdsbTraffic(&this->traffic.adsb_list[this->adsb_selection]);
  else
    {
    target = nullptr;
    this->adsb_selection = -1;
    }

  if (nullptr == target)
    {
    if (this->adsb_selection >= 0)
      {
      this->SetAdsbTarget(target);
      /*
       * At this point we have attempted to retrieve a target which is before
       * the current beginning of the list, i.e. no ADSB target exists. A
       * call to this->SetAdsbTarget(nullptr) will set 
       * this->adsb_selection = -1 which will signify that we are finished
       * with the ADSB targets for now.
       */
      return false;
      }
    else
      {
      target = this->traffic.LastAdsbTraffic();
      this->adsb_selection = (int)this->traffic.adsb_list.size() - 1;
      this->SetAdsbTarget(target);
      }
    }
  return true;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::NextTarget() noexcept
  {
  // If warning is displayed -> prevent selector movement
  if (WarningMode())
    return;
  if (this->flarm_selection >= 0)
    {
    if (false == this->NextFlarmTarget())
      {
      this->flarm_selection = -1;
      const AdsbTarget*  fa = this->traffic.FirstAdsbTraffic();
      if (fa != nullptr)
        this->NextAdsbTarget();
      }
    }
  else if (this->adsb_selection >= 0)
    {
    if (false == this->NextAdsbTarget())
      {
      this->adsb_selection = -1;
      const FlarmTarget* ff = this->traffic.FirstFlarmTraffic();
      if (ff != nullptr)
        this->NextFlarmTarget();
      }
    }
  if ((-1 == this->flarm_selection) && (-1 == this->adsb_selection))
    {
    /* If there are both FLARM and ADSB targets available this logic will
     * the FLARM targets.
     */
    const AdsbTarget*  fa = this->traffic.FirstAdsbTraffic();
    this->SetAdsbTarget(fa);

    const FlarmTarget* ff = this->traffic.FirstFlarmTraffic();
    this->SetFlarmTarget(ff);
    if ((-1 == this->flarm_selection) && (-1 == this->adsb_selection))
      {
      if (this->traffic.FirstFlarmTraffic() != nullptr)
        {
        this->flarm_selection = 0;
        }
      else if (this->traffic.FirstAdsbTraffic() != nullptr)
        {
        this->adsb_selection = 0;
        }
      }
    }
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PrevTarget() noexcept
  {
  // If warning is displayed -> prevent selector movement
  if (WarningMode())
    return;
  this->PrevAdsbTarget();
  if ((-1 == this->flarm_selection) && (-1 == this->adsb_selection))
    {
    const FlarmTarget* fa = this->traffic.LastFlarmTraffic();
    this->SetFlarmTarget(fa);
    if ((-1 == this->flarm_selection) && (-1 == this->adsb_selection))
      {
      if (this->traffic.LastAdsbTraffic() != nullptr)
        {
        this->adsb_selection = 0;
        }
      else if (this->traffic.LastFlarmTraffic() != nullptr)
        {
        this->flarm_selection = 0;
        }
      }
    }
  else if (this->flarm_selection > 0)
  /*
   * If only one FLARM target stay at the start of the Flarm list. Otherwise
   * advance to the previous target.
   */
    this->PrevFlarmTarget();
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::UpdateSelector(const TargetId id,
                                   const PixelPoint pt) noexcept
  {
  // Update #selection
  if (!id.IsDefined())
    this->SetFlarmTarget(nullptr);
  else
    this->SetFlarmTarget(id);

  // If we don't have a valid selection and we can't find
  // a target close to to the PixelPoint we select the next one
  // on the internal list
  if (this->flarm_selection < 0 && this->adsb_selection < 0 &&
      (pt.x < 0 ||
       pt.y < 0 ||
       !SelectNearTarget(pt, this->radius * 2)) )
    this->NextFlarmTarget();
  if (this->flarm_selection < 0) 
    {
    if (this->adsb_selection < 0)
      {
      // No flarm selection, try ADSB.
      if (false == this->traffic.adsb_list.empty())
        this->adsb_selection = 0;
      }
    else
      {
      if ((TargetList::AdsbListType::size_type)this->adsb_selection >=
          this->traffic.adsb_list.size())
        this->adsb_selection = this->traffic.adsb_list.size() - 1;
      }
    }
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::UpdateWarnings() noexcept
  {
  const FlarmTarget* alert = this->traffic.FindMaximumAlert();
  this->warning = (alert != nullptr) ?
                    (int)this->IndexFlarmTarget(alert->id) :
                    - 1;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::Update(Angle new_direction,
                           const TargetList& new_traffic,
                           const TeamCodeSettings& new_settings,
                           TargetStatus target_status) noexcept
  {
  static constexpr Angle min_heading_delta = Angle::Degrees(2);
  if ((new_traffic.modified == this->data_modified)                  &&
      (this->heading - new_direction).Absolute() < min_heading_delta &&
      this->target_status.gps       == target_status.gps)
    {
    /* no change - don't redraw */
    return;
    }

  TargetId selection_id;
  PixelPoint pt;
  if (!this->small && this->flarm_selection >= 0)
    {
    selection_id = this->traffic.flarm_list[this->flarm_selection].id;
    pt = sc[this->flarm_selection];
    }
  else
    {
    selection_id.Clear();
    pt.x = -100;
    pt.y = -100;
    }

  this->data_modified = new_traffic.modified;
  this->heading = new_direction;
  this->fr = -heading;
  this->fir = heading;
  this->traffic = new_traffic;
  this->settings = new_settings;
  this->target_status = target_status;

  this->UpdateWarnings();
  this->UpdateSelector(selection_id, pt);

  this->Invalidate();
  }

//------------------------------------------------------------------------------
double
FlarmTrafficWindow::RangeScale(double d) const noexcept
  {
  d /= this->distance;
  return std::min(d, 1.) * this->radius;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarNoTraffic(Canvas &canvas) const noexcept
  {
  if (this->small)
    return;

  const TCHAR* str = _("No Traffic");
  canvas.Select(look.no_traffic_font);
  PixelSize ts = canvas.CalcTextSize(str);
  canvas.SetTextColor(look.default_color);
  canvas.DrawText(radar_mid - PixelSize{ts.width / 2, this->radius / 2}, str);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarNoGo(Canvas &canvas) const noexcept
  {
  if (this->small)
    return;

  const TCHAR* str = _("No Go");
  canvas.Select(look.no_traffic_font);
  PixelSize ts = canvas.CalcTextSize(str);
  canvas.SetTextColor(look.default_color);
  canvas.DrawText(radar_mid - PixelSize{ts.width / 2, this->radius / 2}, str);
  }

//------------------------------------------------------------------------------
[[gnu::const]]
static const Pen *
FlarmColorPen(const FlarmTrafficLook &look, TargetColor color) noexcept
  {
  switch (color)
    {
    case TargetColor::NONE:
      break;

    case TargetColor::GREEN:
      return &look.team_pen_green;

    case TargetColor::BLUE:
      return &look.team_pen_blue;

    case TargetColor::YELLOW:
      return &look.team_pen_yellow;

    case TargetColor::MAGENTA:
      return &look.team_pen_magenta;

    case TargetColor::COUNT:
      assert(false);
      gcc_unreachable();
    }

  return nullptr;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarFlarmTarget(Canvas& canvas,
                                          const RemoteTarget& target,
                                          unsigned i) noexcept
  {
  // Save relative East/North
  DoublePoint2D p(target.relative_east, -target.relative_north);

  // Calculate the distance in pixels
  double scale = this->RangeScale(target.distance);

  // Don't display distracting, far away targets in WarningMode
  if (this->WarningMode() && !target.HasAlarm() && scale == this->radius)
    return;

  // x and y are not between 0 and 1 (distance will be handled via scale)
  if (!target.distance.IsZero())
    {
    p.x /= target.distance;
    p.y /= target.distance;
    }
  else
    {
    p.x = 0;
    p.y = 0;
    }

  if (!enable_north_up)
    {
    // Rotate x and y to have a track up display
    p = fr.Rotate(p);
    }

  // Calculate screen coordinates
  sc[i].x = radar_mid.x + iround(p.x * scale);
  sc[i].y = radar_mid.y + iround(p.y * scale);

  const Color* text_color;
  const Pen* target_pen;
  const Pen* circle_pen;
  const Brush* target_brush = nullptr;
  const Brush* arrow_brush;
  unsigned circles = 0;

  // Set the arrow color depending on alarm level
  switch (target.alarm_level)
    {
    case RemoteTarget::AlarmType::LOW:
    case RemoteTarget::AlarmType::INFO_ALERT:
      text_color = &look.default_color;
      target_pen = circle_pen = &look.warning_pen;
      target_brush = &look.warning_brush;
      arrow_brush = &look.default_brush;
      circles = 1;
      break;
    case RemoteTarget::AlarmType::IMPORTANT:
    case RemoteTarget::AlarmType::URGENT:
      text_color = &look.default_color;
      target_pen = circle_pen = &look.alarm_pen;
      target_brush = &look.alarm_brush;
      arrow_brush = &look.default_brush;
      circles = 2;
      break;
    case RemoteTarget::AlarmType::NONE:
    default:
      if (WarningMode())
        {
        text_color = &look.passive_color;
        target_pen = &look.passive_pen;
        arrow_brush = &look.passive_brush;
        }
      else
        {
        // Search for team color
        const TargetColor team_color = TargetFriends::GetFriendColor(target.id);

        // If team color found -> draw a colored circle around the target
        if (team_color != TargetColor::NONE)
          {
          circle_pen = FlarmColorPen(look, team_color);
          circles = 1;
          }
        else
          {
          // unnecessary - prevents "may be used uninitialized" compiler warning
          circle_pen = &look.default_pen;
          }

        if (!this->small && static_cast<unsigned> (this->flarm_selection) == i)
          {
          text_color = &look.selection_color;
          target_brush = arrow_brush = &look.selection_brush;
          target_pen = &look.selection_pen;
          }
        else
          {
          if (target.IsPassive())
            {
            text_color = &look.passive_color;
            target_pen = &look.passive_pen;
            arrow_brush = &look.passive_brush;
            }
          else
            {
            text_color = &look.default_color;
            target_pen = &look.default_pen;
            arrow_brush = &look.default_brush;
            }
          }
        }
      break;
    }

  if (circles > 0)
    {
    canvas.SelectHollowBrush();
    canvas.Select(*circle_pen);
    canvas.DrawCircle(sc[i], Layout::FastScale(this->small ? 8 : 16));
    if (2 == circles)
      canvas.DrawCircle(sc[i], Layout::FastScale(this->small ? 10 : 19));
    }

  // Create an arrow polygon
  BulkPixelPoint Arrow[4];
  if (this->small)
    {
    Arrow[0].x = -3;
    Arrow[0].y = 4;
    Arrow[1].x = 0;
    Arrow[1].y = -5;
    Arrow[2].x = 3;
    Arrow[2].y = 4;
    Arrow[3].x = 0;
    Arrow[3].y = 2;
    }
  else
    {
    Arrow[0].x = -6;
    Arrow[0].y = 8;
    Arrow[1].x = 0;
    Arrow[1].y = -10;
    Arrow[2].x = 6;
    Arrow[2].y = 8;
    Arrow[3].x = 0;
    Arrow[3].y = 5;
    }

  // Rotate and shift the arrow
  PolygonRotateShift(Arrow, sc[i],
                     target.track - (enable_north_up ?
                                      Angle::Zero() : heading),
                     Layout::Scale(100u));

  // Select pen and brush
  canvas.Select(*target_pen);
  if (nullptr == target_brush)
    canvas.SelectHollowBrush();
  else
    canvas.Select(*target_brush);

  // Draw the polygon
  canvas.DrawPolygon(Arrow, 4);

  if (this->small)
    {
    if (!WarningMode() || target.HasAlarm())
      PaintTargetInfoSmall(canvas, target, i, *text_color, *arrow_brush);

    return;
    }

  // if warning exists -> don't draw vertical speeds
  if (WarningMode())
    return;

  // if vertical speed to small or negative -> skip this one
  if (SideInfoType::VARIO == side_display_type &&
      (!target.climb_rate_avg30s_available ||
       target.climb_rate_avg30s < 0.5 ||
       target.IsPowered()))
    return;

  // Select font
  canvas.SetBackgroundTransparent();
  canvas.Select(look.side_info_font);

  // Format string
  TCHAR tmp[10];

  if (SideInfoType::VARIO == side_display_type)
    FormatUserVerticalSpeed(target.climb_rate_avg30s, tmp, false);
  else
    FormatRelativeUserAltitude(target.relative_altitude, tmp, true);

  PixelSize sz = canvas.CalcTextSize(tmp);
  const PixelPoint tp
    {
    sc[i].x + int(Layout::FastScale(11u)),
    sc[i].y - int(sz.height / 2),
    };

  // Draw vertical speed shadow
  canvas.SetTextColor(COLOR_WHITE);
  canvas.DrawText({tp.x + 1, tp.y + 1}, tmp);
  canvas.DrawText({tp.y - 1, tp.y - 1}, tmp);

  // Select color
  canvas.SetTextColor(*text_color);

  // Draw vertical speed
  canvas.DrawText(tp, tmp);
  }


//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarAdsbTarget(Canvas& canvas,
                                          const RemoteTarget& target,
                                          unsigned i) noexcept
  {
  // Save relative East/North
  DoublePoint2D p(target.relative_east, -target.relative_north);

  // Calculate the distance in pixels
  double scale = this->RangeScale(target.distance);

  // Don't display distracting, far away targets in WarningMode
  if (this->WarningMode() && !target.HasAlarm() && scale == this->radius)
    return;

  // x and y are not between 0 and 1 (distance will be handled via scale)
  if (!target.distance.IsZero())
    {
    p.x /= target.distance;
    p.y /= target.distance;
    }
  else
    {
    p.x = 0;
    p.y = 0;
    }

  if (!enable_north_up)
    {
    // Rotate x and y to have a track up display
    p = fr.Rotate(p);
    }

  // Calculate screen coordinates
  sc[i].x = radar_mid.x + iround(p.x * scale);
  sc[i].y = radar_mid.y + iround(p.y * scale);

  const Color* text_color;
  const Pen* target_pen;
  const Pen* circle_pen;
  const Brush* target_brush = nullptr;
  const Brush* arrow_brush;
  unsigned circles = 0;

  // Set the arrow color depending on alarm level
  switch (target.alarm_level)
    {
    case RemoteTarget::AlarmType::LOW:
    case RemoteTarget::AlarmType::INFO_ALERT:
      text_color = &look.default_color;
      target_pen = circle_pen = &look.warning_pen;
      target_brush = &look.warning_brush;
      arrow_brush = &look.default_brush;
      circles = 1;
      break;
    case RemoteTarget::AlarmType::IMPORTANT:
    case RemoteTarget::AlarmType::URGENT:
      text_color = &look.default_color;
      target_pen = circle_pen = &look.alarm_pen;
      target_brush = &look.alarm_brush;
      arrow_brush = &look.default_brush;
      circles = 2;
      break;
    case RemoteTarget::AlarmType::NONE:
    default:
      if (WarningMode())
        {
        text_color = &look.passive_color;
        target_pen = &look.passive_pen;
        arrow_brush = &look.passive_brush;
        }
      else
        {
        // Search for team color
        const TargetColor team_color = TargetFriends::GetFriendColor(target.id);

        // If team color found -> draw a colored circle around the target
        if (team_color != TargetColor::NONE)
          {
          circle_pen = FlarmColorPen(look, team_color);
          circles = 1;
          }
        else
          {
          // unnecessary - prevents "may be used uninitialized" compiler warning
          circle_pen = &look.default_pen;
          }

        if (!this->small && static_cast<unsigned> (this->adsb_selection) == i)
          {
          text_color = &look.selection_color;
          target_brush = arrow_brush = &look.selection_brush;
          target_pen = &look.selection_pen;
          }
        else
          {
          if (target.IsPassive())
            {
            text_color = &look.passive_color;
            target_pen = &look.passive_pen;
            arrow_brush = &look.passive_brush;
            }
          else
            {
            text_color = &look.default_color;
            target_pen = &look.default_pen;
            arrow_brush = &look.default_brush;
            }
          }
        }
      break;
    }

  if (circles > 0)
    {
    canvas.SelectHollowBrush();
    canvas.Select(*circle_pen);
    canvas.DrawCircle(sc[i], Layout::FastScale(this->small ? 8 : 16));
    if (2 == circles)
      canvas.DrawCircle(sc[i], Layout::FastScale(this->small ? 10 : 19));
    }

  // Create an ATC style glyph
  PixelPoint glyph[2][2];
  if (this->small)
    {
    glyph[0][0].x = -2;
    glyph[0][0].y =  0;
    glyph[0][1].x =  2;
    glyph[0][1].y =  0;
    glyph[1][0].x =  0;
    glyph[1][0].y =  0;
    glyph[1][1].x =  0;
    glyph[1][1].y = -5;
    }
  else
    {
    glyph[0][0].x = -4;
    glyph[0][0].y =  0;
    glyph[0][1].x =  4;
    glyph[0][1].y =  0;
    glyph[1][0].x =  0;
    glyph[1][0].y =  0;
    glyph[1][1].x =  0;
    glyph[1][1].y = -10;
    }

  // Rotate and shift the arrow
  for (int j = 0; j < 2; j++)
    {
    for (int k = 0; k < 2; k++)
      {
      PointRotateShift(glyph[j][k],
                       sc[i],
                       target.track -
                         (enable_north_up ? Angle::Zero() : heading),
                       Layout::Scale(100u));
      }
    }

  // Select pen and brush
  canvas.Select(*target_pen);
  if (nullptr == target_brush)
    canvas.SelectHollowBrush();
  else
    canvas.Select(*target_brush);

  // Draw the lines
  for (int j = 0; j < 2; j++)
    canvas.DrawLine(glyph[j][0], glyph[j][1]);

  if (this->small)
    {
    if (!WarningMode() || target.HasAlarm())
      PaintTargetInfoSmall(canvas, target, i, *text_color, *arrow_brush);

    return;
    }

  return;
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintTargetInfoSmall(Canvas& canvas,
                                         const RemoteTarget& target,
                                         unsigned i,
                                         const Color& text_color,
                                         const Brush& arrow_brush) noexcept
  {
  const short relalt =
    iround(Units::ToUserAltitude(target.relative_altitude) / 100);

  // if (relative altitude is other than zero)
  if (0 == relalt)
    return;

  // Write the relativ altitude devided by 100 to the Buffer
  StaticString<10> buffer;
  const auto relalt_s = buffer.Format(_T("%d"), abs(relalt));

  // Select font
  canvas.SetBackgroundTransparent();
  canvas.Select(look.side_info_font);
  canvas.SetTextColor(text_color);

  // Calculate size of the output string
  PixelSize tsize = canvas.CalcTextSize(relalt_s);

  unsigned dist = Layout::FastScale(target.HasAlarm() ? 12 : 8);

  // Draw string
  canvas.DrawText(sc[i].At(dist, -int(tsize.height / 2)), relalt_s);

  // Set target_brush for the up/down arrow
  canvas.Select(arrow_brush);
  canvas.SelectNullPen();

  // Prepare the triangular polygon
  BulkPixelPoint triangle[4];
  triangle[0].x = 0;
  triangle[0].y = -4;
  triangle[1].x = 3;
  triangle[1].y = 0;
  triangle[2].x = -3;
  triangle[2].y = 0;

  // Flip = -1 for arrow pointing downwards
  short flip = 1;
  if (relalt < 0)
    flip = -1;

  // Shift the arrow to the right position
  for (int j = 0; j < 3; j++)
    {
    triangle[j].x = Layout::FastScale(triangle[j].x);
    triangle[j].y = Layout::FastScale(triangle[j].y);

    triangle[j] = sc[i].At(dist + triangle[j].x + int(tsize.width / 2),
                           flip * (triangle[j].y - int(tsize.height / 2)));
    }
  triangle[3].x = triangle[0].x;
  triangle[3].y = triangle[0].y;

  // Draw the arrow
  canvas.DrawTriangleFan(triangle, 4);

  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarTraffic(Canvas &canvas) noexcept
  {
  /**
   * \todo Reimplement the Radar NoGo display. (Issue 5)
   */
  if (this->traffic.IsEmpty())
    {
    this->PaintRadarNoTraffic(canvas);
    return;
    }

  // Iterate through the FLARM traffic (normal traffic).
  for (unsigned i = 0; i < this->traffic.flarm_list.size(); ++i)
    {
    const FlarmTarget& target = this->traffic.flarm_list[i];

    if (!target.HasAlarm() &&
        static_cast<unsigned> (this->flarm_selection) != i)
      this->PaintRadarFlarmTarget(canvas, target, i);
    }

  // Iterate through the ADSB traffic (normal traffic).
  for (unsigned i = 0; i < this->traffic.adsb_list.size(); ++i)
    {
    const AdsbTarget& target = this->traffic.adsb_list[i];

    // Exclude ADSB targets that share a hex id with a presently displayed
    // FLARM target.
    if (this->traffic.FindFlarmTraffic(target.id))
      continue;

    /*
     * At this time the maximum outer range ring on the radar canvas is
     * 10kM. If this is extended 
     */
    if (target.distance < RoughDistance(20000.0))
      this->PaintRadarAdsbTarget(canvas, target, i);
    }

  if (this->flarm_selection >= 0)
    {
    const FlarmTarget& target = this->traffic.flarm_list[this->flarm_selection];

    if (!target.HasAlarm())
      this->PaintRadarFlarmTarget(canvas, target, this->flarm_selection);
    }

  if (!this->WarningMode())
    return;

  // Iterate through the Flarm traffic (alarm traffic)
  for (unsigned i = 0; i < this->traffic.flarm_list.size(); ++i)
    {
    const FlarmTarget& target = this->traffic.flarm_list[i];

    if (target.HasAlarm())
      this->PaintRadarFlarmTarget(canvas, target, i);
    }
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarPlane(Canvas &canvas) const noexcept
  {
  canvas.Select(look.plane_pen);

  PixelPoint p1(Layout::FastScale(this->small ? 5 : 10),
                -Layout::FastScale(this->small ? 1 : 2));
  PixelPoint p2(-p1.x, p1.y);

  if (enable_north_up)
    {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
    }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);

  p2 = { 0, Layout::FastScale(this->small ? 3 : 6) };
  p1 = { 0, -p2.y };

  if (enable_north_up)
    {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
    }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);

  p1.x = Layout::FastScale(this->small ? 2 : 4);
  p1.y = p1.x;
  p2 = { -p1.x, p1.y };

  if (enable_north_up)
    {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
    }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);
  }

//------------------------------------------------------------------------------
[[gnu::const]]
static PixelPoint
iround(DoublePoint2D p) noexcept
  {
  return {iround(p.x), iround(p.y)};
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintNorth(Canvas &canvas) const noexcept
  {
  DoublePoint2D p(0, -1);
  if (!enable_north_up)
    {
    p = fr.Rotate(p);
    }

  canvas.SetTextColor(look.background_color);
  canvas.Select(look.radar_pen);
  canvas.Select(look.radar_brush);
  canvas.SetBackgroundTransparent();
  canvas.Select(look.label_font);

  const PixelPoint q = radar_mid + iround(p * this->radius);

  PixelSize s = canvas.CalcTextSize(_T("N"));
  canvas.DrawCircle(q, s.height * 0.65);
  canvas.DrawText(q - s / 2u, _T("N"));
  }

//------------------------------------------------------------------------------
static void
DrawCircleLabel(Canvas &canvas, PixelPoint p,
                tstring_view text) noexcept
  {
  const auto size = canvas.CalcTextSize(text);
  p.x -= size.width / 2;
  p.y -= size.height * 3 / 4;

  canvas.DrawText(p, text);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::PaintRadarBackground(Canvas &canvas) const noexcept
  {
  canvas.SelectHollowBrush();
  canvas.Select(look.radar_pen);
  canvas.SetTextColor(look.radar_color);

  // Paint circles
  canvas.DrawCircle(radar_mid, this->radius);
  canvas.DrawCircle(radar_mid, this->radius / 2);

  PaintRadarPlane(canvas);

  if (this->small)
    return;

  // Paint zoom strings
  canvas.Select(look.label_font);
  canvas.SetBackgroundOpaque();
  canvas.SetBackgroundColor(look.background_color);

  TCHAR distance_string[10];
  FormatUserDistanceSmart(this->distance, distance_string,
                          ARRAY_SIZE(distance_string), 1000);
  DrawCircleLabel(canvas,
                  radar_mid + PixelSize{0u, this->radius},
                  distance_string);

  FormatUserDistanceSmart(this->distance / 2, distance_string,
                          ARRAY_SIZE(distance_string), 1000);
  DrawCircleLabel(canvas, radar_mid + PixelSize{0u, this->radius / 2},
                  distance_string);

  canvas.SetBackgroundTransparent();

  PaintNorth(canvas);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::Paint(Canvas& canvas) noexcept
  {
  assert(this->flarm_selection < (int)this->traffic.flarm_list.size());
  assert(this->flarm_selection < 0 || this->traffic.flarm_list[this->flarm_selection].IsDefined());
  assert(this->warning < (int)this->traffic.flarm_list.size());
  assert(this->warning < 0 || this->traffic.flarm_list[this->warning].IsDefined());
  assert(this->warning < 0 || this->traffic.flarm_list[this->warning].HasAlarm());

  this->PaintRadarBackground(canvas);
  this->PaintRadarTraffic(canvas);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficWindow::OnPaint(Canvas &canvas) noexcept
  {
#ifdef ENABLE_OPENGL
  if (this->small)
    {
    const ScopeAlphaBlend alpha_blend;

    canvas.SelectBlackPen();
    canvas.Select(Brush(look.background_color.WithAlpha(0xd0)));
    canvas.DrawCircle(radar_mid, this->radius);

    }
  else
#endif
    canvas.Clear(look.background_color);

  Paint(canvas);
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficWindow::SelectNearTarget(PixelPoint p, int max_distance) noexcept
  {
  int min_distance = 99999;
  int min_id = -1;

  for (unsigned i = 0; i < this->traffic.flarm_list.size(); ++i)
    {
    // If FLARM target does not exist -> next one
    if (!this->traffic.flarm_list[i].IsDefined())
      continue;

    int distance_sq = (p - sc[i]).MagnitudeSquared();

    if (distance_sq > min_distance
        || distance_sq > max_distance * max_distance)
      continue;

    min_distance = distance_sq;
    min_id = i;
    }

  if (min_id >= 0)
    {
    TargetId t_id((long int)min_id);
    this->SetFlarmTarget(t_id);
    }

  return min_id >= 0;
  }

//------------------------------------------------------------------------------
int
FlarmTrafficWindow::IndexFlarmTarget(const TargetId& id) const noexcept
  {
  /*
   * So, plainly, this is not the most efficient in terms of time to find
   * this index. But this function is not called often so this is good
   * enough.
   */
  for (size_t i = 0; i < this->traffic.flarm_list.size(); i++)
    {
    if (this->traffic.flarm_list[i].id == id)
      return i;
    }
  return -1;
  }

//------------------------------------------------------------------------------
int
FlarmTrafficWindow::IndexAdsbTarget(const TargetId& id) const noexcept
  {
  /*
   * So, plainly, this is not the most efficient in terms of time to find
   * this index. But this function is not called often so this is good
   * enough.
   */
  for (size_t i = 0; i < this->traffic.adsb_list.size(); i++)
    {
    if (this->traffic.adsb_list[i].id == id)
      return i;
    }
  return -1;
  }
