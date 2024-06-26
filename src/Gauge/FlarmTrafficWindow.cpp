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

#include "FlarmTrafficWindow.hpp"
#include "FLARM/Traffic.hpp"
#include "FLARM/Friends.hpp"
#include "ADSB/Traffic.hpp"
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
#include "LogFile.hpp"
#endif

FlarmTrafficWindow::FlarmTrafficWindow(const FlarmTrafficLook &_look,
                                       unsigned _h_padding,
                                       unsigned _v_padding,
                                       bool _small) noexcept
  :look(_look),
   h_padding(_h_padding), v_padding(_v_padding),
   small(_small)
{
  flarm_data.Clear();
  adsb_data.Clear();
  data_modified.Clear();
  this->flarm_status.Clear();
  this->adsb_status.Clear();
}

bool
FlarmTrafficWindow::WarningMode() const noexcept
{
  assert(warning < (int)flarm_data.list.size());
  assert(warning < 0 || flarm_data.list[warning].IsDefined());
  assert(warning < 0 || flarm_data.list[warning].HasAlarm());

  return warning >= 0;
}

void
FlarmTrafficWindow::OnResize(PixelSize new_size) noexcept
{
  PaintWindow::OnResize(new_size);

  const unsigned half_width = new_size.width / 2;
  const unsigned half_height = new_size.height / 2;

  // Calculate Radar size
  radius = std::min(half_width - h_padding, half_height - v_padding);
  radar_mid.x = half_width;
  radar_mid.y = half_height;
}

void
FlarmTrafficWindow::SetTarget(int i) noexcept
{
  assert(i < (int)flarm_data.list.size());
  assert(i < 0 || flarm_data.list[i].IsDefined());

  if (selection == i)
    return;

  selection = i;
  Invalidate();
}

/**
 * Tries to select the next target, if impossible selection = -1
 */
void
FlarmTrafficWindow::NextTarget() noexcept
{
  // If warning is displayed -> prevent selector movement
  if (WarningMode())
    return;

  assert(selection < (int)flarm_data.list.size());

  const FlarmTraffic *traffic;
  if (selection >= 0)
    traffic = flarm_data.NextTraffic(&flarm_data.list[selection]);
  else
    traffic = NULL;

  if (traffic == NULL)
    traffic = flarm_data.FirstTraffic();

  SetTarget(traffic);
}

/**
 * Tries to select the previous target, if impossible selection = -1
 */
void
FlarmTrafficWindow::PrevTarget() noexcept
{
  // If warning is displayed -> prevent selector movement
  if (WarningMode())
    return;

  assert(selection < (int)flarm_data.list.size());

  const FlarmTraffic *traffic;
  if (selection >= 0)
    traffic = flarm_data.PreviousTraffic(&flarm_data.list[selection]);
  else
    traffic = NULL;

  if (traffic == NULL)
    traffic = flarm_data.LastTraffic();

  SetTarget(traffic);
}

/**
 * Checks whether the selection is still on the valid target and if not tries
 * to select the next one
 */
void
FlarmTrafficWindow::UpdateSelector(const FlarmId id,
                                   const PixelPoint pt) noexcept
{
  // Update #selection
  if (!id.IsDefined())
    SetTarget(-1);
  else
    SetTarget(id);

  // If we don't have a valid selection and we can't find
  // a target close to to the PixelPoint we select the next one
  // on the internal list
  if (selection < 0 && (
      pt.x < 0 || pt.y < 0 ||
      !SelectNearTarget(pt, radius * 2)) )
    NextTarget();
}

/**
 * Iterates through the traffic array, finds the target with the highest
 * alarm level and saves it to "warning".
 */
void
FlarmTrafficWindow::UpdateWarnings() noexcept
{
  const FlarmTraffic *alert = flarm_data.FindMaximumAlert();
  warning = alert != NULL
    ? (int)flarm_data.TrafficIndex(alert)
    : - 1;
}

/**
 * This should be called when the radar needs to be repainted
 * @param new_direction The new aircraft heading or track.
 * @param new_flarm_data The new list of Flarm targets.
 * @param new_adsb_data The list of new ADSB targets.
 * @param new_settings The new team codes.
 * @param flarm_status Present status of the Flarm.
 * @param adsb_status Present status of the ADSB.
 */
void
FlarmTrafficWindow::Update(Angle new_direction,
                           const TrafficList &new_flarm_data,
                           const AdsbTrafficList &new_adsb_data,
                           const TeamCodeSettings &new_settings,
                           FlarmStatus flarm_status,
                           AdsbStatus  adsb_status) noexcept
  {
  static constexpr Angle min_heading_delta = Angle::Degrees(2);
  /**
   * \todo Modify test to include ADSB data for consideration.
   */
#ifndef NDEBUG
#include "LogFile.hpp"
//  LogFormat("%s, %d: %d", __FILE__, __LINE__, new_adsb_data.modified.ToInteger());
#endif
  if ((new_flarm_data.modified == this->data_modified)               &&
      (new_adsb_data.modified  == this->data_modified)               &&
      (this->heading - new_direction).Absolute() < min_heading_delta &&
      this->flarm_status.gps       == flarm_status.gps               &&
      this->adsb_status.gps        == adsb_status.gps)
    {
//#ifndef NDEBUG
//#include "LogFile.hpp"
//    LogFormat("%s, %d", __FILE__, __LINE__);
//#endif
    /* no change - don't redraw */
    return;
    }

#ifndef NDEBUG
#include "LogFile.hpp"
//    LogFormat("%s, %d", __FILE__, __LINE__);
#endif
  FlarmId selection_id;
  PixelPoint pt;
  if (!small && selection >= 0)
    {
    selection_id = flarm_data.list[selection].id;
    pt = sc[selection];
    }
  else
    {
    selection_id.Clear();
    pt.x = -100;
    pt.y = -100;
    }

  /* \todo pfb: Refactor to avoid this silliness. */
  if (new_flarm_data.modified == new_adsb_data.modified)
    this->data_modified = new_flarm_data.modified;
  else
    if (new_flarm_data.modified != this->data_modified)
      this->data_modified = new_flarm_data.modified;
    else
      this->data_modified = new_adsb_data.modified;
  heading = new_direction;
  this->fr = -heading;
  this->fir = heading;
  this->flarm_data = new_flarm_data;
  this->adsb_data  = new_adsb_data;
  this->settings = new_settings;
  this->flarm_status = flarm_status;
  this->adsb_status  = adsb_status;

  this->UpdateWarnings();
  this->UpdateSelector(selection_id, pt);

  this->Invalidate();
  }

/**
 * Returns the distance to the own plane in pixels
 * @param d Distance in meters to the own plane
 */
double
FlarmTrafficWindow::RangeScale(double d) const noexcept
{
  d /= distance;
  return std::min(d, 1.) * radius;
}

/**
 * Paints a "No Traffic" sign on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarNoTraffic(Canvas &canvas) const noexcept
{
  if (small)
    return;

  const TCHAR* str = _("No Traffic");
  canvas.Select(look.no_traffic_font);
  PixelSize ts = canvas.CalcTextSize(str);
  canvas.SetTextColor(look.default_color);
  canvas.DrawText(radar_mid - PixelSize{ts.width / 2, radius / 2}, str);
}

/**
 * Paints a "No Go" sign on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarNoGo(Canvas &canvas) const noexcept
  {
  if (small)
    return;

  const TCHAR* str = _("No Go");
  canvas.Select(look.no_traffic_font);
  PixelSize ts = canvas.CalcTextSize(str);
  canvas.SetTextColor(look.default_color);
  canvas.DrawText(radar_mid - PixelSize{ts.width / 2, this->radius / 2}, str);
  }

[[gnu::const]]
static const Pen *
FlarmColorPen(const FlarmTrafficLook &look, FlarmColor color) noexcept
{
  switch (color) {
  case FlarmColor::NONE:
    break;

  case FlarmColor::GREEN:
    return &look.team_pen_green;

  case FlarmColor::BLUE:
    return &look.team_pen_blue;

  case FlarmColor::YELLOW:
    return &look.team_pen_yellow;

  case FlarmColor::MAGENTA:
    return &look.team_pen_magenta;

  case FlarmColor::COUNT:
    assert(false);
    gcc_unreachable();
  }

  return nullptr;
}

/**
 * Paints the traffic symbols on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarTarget(Canvas &canvas,
                                     const FlarmTraffic &traffic,
                                     unsigned i) noexcept
{
  // Save relative East/North
  DoublePoint2D p(traffic.relative_east, -traffic.relative_north);

  // Calculate the distance in pixels
  double scale = RangeScale(traffic.distance);

  // Don't display distracting, far away targets in WarningMode
  if (WarningMode() && !traffic.HasAlarm() && scale == radius)
    return;

  // x and y are not between 0 and 1 (distance will be handled via scale)
  if (!traffic.distance.IsZero()) {
    p.x /= traffic.distance;
    p.y /= traffic.distance;
  } else {
    p.x = 0;
    p.y = 0;
  }

  if (!enable_north_up) {
    // Rotate x and y to have a track up display
    p = fr.Rotate(p);
  }

  // Calculate screen coordinates
  sc[i].x = radar_mid.x + iround(p.x * scale);
  sc[i].y = radar_mid.y + iround(p.y * scale);

  const Color *text_color;
  const Pen *target_pen, *circle_pen;
  const Brush *target_brush = nullptr, *arrow_brush;
  unsigned circles = 0;

  // Set the arrow color depending on alarm level
  switch (traffic.alarm_level) {
  case FlarmTraffic::AlarmType::LOW:
  case FlarmTraffic::AlarmType::INFO_ALERT:
    text_color = &look.default_color;
    target_pen = circle_pen = &look.warning_pen;
    target_brush = &look.warning_brush;
    arrow_brush = &look.default_brush;
    circles = 1;
    break;
  case FlarmTraffic::AlarmType::IMPORTANT:
  case FlarmTraffic::AlarmType::URGENT:
    text_color = &look.default_color;
    target_pen = circle_pen = &look.alarm_pen;
    target_brush = &look.alarm_brush;
    arrow_brush = &look.default_brush;
    circles = 2;
    break;
  case FlarmTraffic::AlarmType::NONE:
  default:
    if (WarningMode()) {
      text_color = &look.passive_color;
      target_pen = &look.passive_pen;
      arrow_brush = &look.passive_brush;
    } else {
      // Search for team color
      const FlarmColor team_color = FlarmFriends::GetFriendColor(traffic.id);

      // If team color found -> draw a colored circle around the target
      if (team_color != FlarmColor::NONE) {
        circle_pen = FlarmColorPen(look, team_color);
        circles = 1;
      } else {
        // unnecessary - prevents "may be used uninitialized" compiler warning
        circle_pen = &look.default_pen;
      }

      if (!small && static_cast<unsigned> (selection) == i) {
        text_color = &look.selection_color;
        target_brush = arrow_brush = &look.selection_brush;
        target_pen = &look.selection_pen;
      } else {
        if (traffic.IsPassive()) {
          text_color = &look.passive_color;
          target_pen = &look.passive_pen;
          arrow_brush = &look.passive_brush;
        } else {
          text_color = &look.default_color;
          target_pen = &look.default_pen;
          arrow_brush = &look.default_brush;
        }
      }
    }
    break;
  }

  if (circles > 0) {
    canvas.SelectHollowBrush();
    canvas.Select(*circle_pen);
    canvas.DrawCircle(sc[i], Layout::FastScale(small ? 8 : 16));
    if (circles == 2)
      canvas.DrawCircle(sc[i], Layout::FastScale(small ? 10 : 19));
  }

  // Create an arrow polygon
  BulkPixelPoint Arrow[4];
  if (small) {
    Arrow[0].x = -3;
    Arrow[0].y = 4;
    Arrow[1].x = 0;
    Arrow[1].y = -5;
    Arrow[2].x = 3;
    Arrow[2].y = 4;
    Arrow[3].x = 0;
    Arrow[3].y = 2;
  } else {
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
                     traffic.track - (enable_north_up ?
                                      Angle::Zero() : heading),
                     Layout::Scale(100u));

  // Select pen and brush
  canvas.Select(*target_pen);
  if (target_brush == nullptr)
    canvas.SelectHollowBrush();
  else
    canvas.Select(*target_brush);

  // Draw the polygon
  canvas.DrawPolygon(Arrow, 4);

  if (small) {
    if (!WarningMode() || traffic.HasAlarm())
      PaintTargetInfoSmall(canvas, traffic, i, *text_color, *arrow_brush);

    return;
  }

  // if warning exists -> don't draw vertical speeds
  if (WarningMode())
    return;

  // if vertical speed to small or negative -> skip this one
  if (side_display_type == SideInfoType::VARIO &&
      (!traffic.climb_rate_avg30s_available ||
       traffic.climb_rate_avg30s < 0.5 ||
       traffic.IsPowered()))
      return;

  // Select font
  canvas.SetBackgroundTransparent();
  canvas.Select(look.side_info_font);

  // Format string
  TCHAR tmp[10];

  if (side_display_type == SideInfoType::VARIO)
    FormatUserVerticalSpeed(traffic.climb_rate_avg30s, tmp, false);
  else
    FormatRelativeUserAltitude(traffic.relative_altitude, tmp, true);

  PixelSize sz = canvas.CalcTextSize(tmp);
  const PixelPoint tp{
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

void
FlarmTrafficWindow::PaintTargetInfoSmall(Canvas &canvas,
                                         const FlarmTraffic &traffic,
                                         unsigned i,
                                         const Color &text_color,
                                         const Brush &arrow_brush) noexcept
{
  const short relalt =
      iround(Units::ToUserAltitude(traffic.relative_altitude) / 100);

  // if (relative altitude is other than zero)
  if (relalt == 0)
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

  unsigned dist = Layout::FastScale(traffic.HasAlarm() ? 12 : 8);

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
  for (int j = 0; j < 3; j++) {
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

/**
 * Paints the traffic symbols on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarTraffic(Canvas &canvas) noexcept
  {
  if (this->flarm_data.IsEmpty() && this->adsb_data.IsEmpty())
    {
    if ((this->flarm_status.available == false ||
         this->flarm_status.gps == FlarmStatus::GPSStatus::NONE) &&
        (this->adsb_status.available == false ||
         this->adsb_status.gps == AdsbStatus::GPSStatus::NO_GO))
      this->PaintRadarNoGo(canvas);
    else
      this->PaintRadarNoTraffic(canvas);
    return;
    }

  // Iterate through the Flarm traffic (normal traffic)
  for (unsigned i = 0; i < flarm_data.list.size(); ++i) {
    const FlarmTraffic &traffic = flarm_data.list[i];

    if (!traffic.HasAlarm() &&
        static_cast<unsigned> (selection) != i)
      PaintRadarTarget(canvas, traffic, i);
  }

  // And iterate through the ADSB traffic
  for (unsigned i = 0; i < this->adsb_data.list.size(); ++i)
    {
    /* \todo pfb: Refactor to remove this kludge */
    const AdsbTraffic &traffic = this->adsb_data.list[i];
    FlarmTraffic fe;
    AdsbConvert(traffic, fe);
    if ((double)fe.distance < 15000.0) /* As a temporary measure do not paint
                                          distant targets. */
      PaintRadarTarget(canvas, fe, i);
    }

  if (selection >= 0) {
    const FlarmTraffic &traffic = flarm_data.list[selection];

    if (!traffic.HasAlarm())
      PaintRadarTarget(canvas, traffic, selection);
  }

  if (!WarningMode())
    return;

  // Iterate through the Flarm traffic (alarm traffic)
  for (unsigned i = 0; i < flarm_data.list.size(); ++i) {
    const FlarmTraffic &traffic = flarm_data.list[i];

    if (traffic.HasAlarm())
      PaintRadarTarget(canvas, traffic, i);
  }
}

/**
 * Paint a plane symbol in the middle of the radar on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarPlane(Canvas &canvas) const noexcept
{
  canvas.Select(look.plane_pen);

  PixelPoint p1(Layout::FastScale(small ? 5 : 10),
                -Layout::FastScale(small ? 1 : 2));
  PixelPoint p2(-p1.x, p1.y);

  if (enable_north_up) {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
  }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);

  p2 = { 0, Layout::FastScale(small ? 3 : 6) };
  p1 = { 0, -p2.y };

  if (enable_north_up) {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
  }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);

  p1.x = Layout::FastScale(small ? 2 : 4);
  p1.y = p1.x;
  p2 = { -p1.x, p1.y };

  if (enable_north_up) {
    p1 = fir.Rotate(p1);
    p2 = fir.Rotate(p2);
  }

  canvas.DrawLine(radar_mid + p1, radar_mid + p2);
}

[[gnu::const]]
static PixelPoint
iround(DoublePoint2D p) noexcept
{
  return {iround(p.x), iround(p.y)};
}

/**
 * Paints the radar circle on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintNorth(Canvas &canvas) const noexcept
{
  DoublePoint2D p(0, -1);
  if (!enable_north_up) {
    p = fr.Rotate(p);
  }

  canvas.SetTextColor(look.background_color);
  canvas.Select(look.radar_pen);
  canvas.Select(look.radar_brush);
  canvas.SetBackgroundTransparent();
  canvas.Select(look.label_font);

  const PixelPoint q = radar_mid + iround(p * radius);

  PixelSize s = canvas.CalcTextSize(_T("N"));
  canvas.DrawCircle(q, s.height * 0.65);
  canvas.DrawText(q - s / 2u, _T("N"));
}

static void
DrawCircleLabel(Canvas &canvas, PixelPoint p,
                tstring_view text) noexcept
{
  const auto size = canvas.CalcTextSize(text);
  p.x -= size.width / 2;
  p.y -= size.height * 3 / 4;

  canvas.DrawText(p, text);
}

/**
 * Paints the radar circle on the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::PaintRadarBackground(Canvas &canvas) const noexcept
{
  canvas.SelectHollowBrush();
  canvas.Select(look.radar_pen);
  canvas.SetTextColor(look.radar_color);

  // Paint circles
  canvas.DrawCircle(radar_mid, radius);
  canvas.DrawCircle(radar_mid, radius / 2);

  PaintRadarPlane(canvas);

  if (small)
    return;

  // Paint zoom strings
  canvas.Select(look.label_font);
  canvas.SetBackgroundOpaque();
  canvas.SetBackgroundColor(look.background_color);

  TCHAR distance_string[10];
  FormatUserDistanceSmart(distance, distance_string,
                          ARRAY_SIZE(distance_string), 1000);
  DrawCircleLabel(canvas, radar_mid + PixelSize{0u, radius}, distance_string);

  FormatUserDistanceSmart(distance / 2, distance_string,
                          ARRAY_SIZE(distance_string), 1000);
  DrawCircleLabel(canvas, radar_mid + PixelSize{0u, radius / 2},
                  distance_string);

  canvas.SetBackgroundTransparent();

  PaintNorth(canvas);
}

/**
 * This function paints the TrafficRadar onto the given canvas
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::Paint(Canvas &canvas) noexcept
{
  assert(selection < (int)flarm_data.list.size());
  assert(selection < 0 || flarm_data.list[selection].IsDefined());
  assert(warning < (int)flarm_data.list.size());
  assert(warning < 0 || flarm_data.list[warning].IsDefined());
  assert(warning < 0 || flarm_data.list[warning].HasAlarm());

  PaintRadarBackground(canvas);
  PaintRadarTraffic(canvas);
}

/**
 * This function is called when the Radar needs repainting.
 * @param canvas The canvas to paint on
 */
void
FlarmTrafficWindow::OnPaint(Canvas &canvas) noexcept
{
#ifdef ENABLE_OPENGL
  if (small) {
    const ScopeAlphaBlend alpha_blend;

    canvas.SelectBlackPen();
    canvas.Select(Brush(look.background_color.WithAlpha(0xd0)));
    canvas.DrawCircle(radar_mid, radius);

  } else
#endif
    canvas.Clear(look.background_color);

  Paint(canvas);
}

bool
FlarmTrafficWindow::SelectNearTarget(PixelPoint p, int max_distance) noexcept
{
  int min_distance = 99999;
  int min_id = -1;

  for (unsigned i = 0; i < flarm_data.list.size(); ++i) {
    // If FLARM target does not exist -> next one
    if (!flarm_data.list[i].IsDefined())
      continue;

    int distance_sq = (p - sc[i]).MagnitudeSquared();

    if (distance_sq > min_distance
        || distance_sq > max_distance * max_distance)
      continue;

    min_distance = distance_sq;
    min_id = i;
  }

  if (min_id >= 0)
    SetTarget(min_id);

  return min_id >= 0;
}
