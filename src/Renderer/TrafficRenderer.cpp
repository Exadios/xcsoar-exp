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

#include "TrafficRenderer.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "Look/TrafficLook.hpp"
#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/Color.hpp"
#include "GliderLink/Traffic.hpp"
#include "Math/Screen.hpp"
#include "util/Macros.hpp"
#include "Asset.hpp"

void
TrafficRenderer::Draw(Canvas &canvas, const TrafficLook &traffic_look,
                      const RemoteTarget &traffic, const Angle angle,
                      const TargetColor color, const PixelPoint pt) noexcept
{
  // Create point array that will form that arrow polygon
  BulkPixelPoint arrow[] = {
    { -4,  6 },
    {  0, -8 },
    {  4,  6 },
    {  0,  3 },
  };

  // Select brush depending on AlarmLevel
  switch (traffic.alarm_level) {
  case RemoteTarget::AlarmType::LOW:
  case RemoteTarget::AlarmType::INFO_ALERT:
    canvas.Select(traffic_look.warning_brush);
    break;
  case RemoteTarget::AlarmType::IMPORTANT:
  case RemoteTarget::AlarmType::URGENT:
    canvas.Select(traffic_look.alarm_brush);
    break;
  case RemoteTarget::AlarmType::NONE:
    if (traffic.relative_altitude > (const RoughAltitude)50) {
      canvas.Select(traffic_look.safe_above_brush);
    } else if (traffic.relative_altitude > (const RoughAltitude)-50) {
      canvas.Select(traffic_look.warning_in_altitude_range_brush);
    } else {
      canvas.Select(traffic_look.safe_below_brush);
    }
    break;
  }

  // Select black pen
  canvas.SelectBlackPen();

  // Rotate and shift the arrow to the right position and angle
  PolygonRotateShift(arrow, pt, angle, Layout::Scale(100U));

  // Draw the arrow
  canvas.DrawPolygon(arrow, ARRAY_SIZE(arrow));

  switch (color) {
  case TargetColor::GREEN:
    canvas.Select(traffic_look.team_pen_green);
    break;
  case TargetColor::BLUE:
    canvas.Select(traffic_look.team_pen_blue);
    break;
  case TargetColor::YELLOW:
    canvas.Select(traffic_look.team_pen_yellow);
    break;
  case TargetColor::MAGENTA:
    canvas.Select(traffic_look.team_pen_magenta);
    break;
  default:
    return;
  }

  canvas.SelectHollowBrush();
  canvas.DrawCircle(pt, Layout::FastScale(11u));
}



void
TrafficRenderer::Draw(Canvas &canvas, const TrafficLook &traffic_look,
                      [[maybe_unused]] const GliderLinkTraffic &traffic,
                      const Angle angle, const PixelPoint pt) noexcept
{
  // Create point array that will form that arrow polygon
  BulkPixelPoint arrow[] = {
    { -4, 6 },
    { 0, -8 },
    { 4, 6 },
    { 0, 3 },
  };

  canvas.Select(traffic_look.safe_above_brush);

  // Select black pen
  if (IsDithered())
    canvas.Select(Pen(Layout::FastScale(2), COLOR_BLACK));
  else
    canvas.SelectBlackPen();

  // Rotate and shift the arrow to the right position and angle
  PolygonRotateShift(arrow, pt, angle, Layout::Scale(100U));

  // Draw the arrow
  canvas.DrawPolygon(arrow, ARRAY_SIZE(arrow));

  canvas.SelectHollowBrush();
  canvas.DrawCircle(pt, Layout::FastScale(11u));
}
