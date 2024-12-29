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

#include "TrafficRenderer.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "Look/TrafficLook.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"
#include "Surveillance/Color.hpp"
#include "GliderLink/Traffic.hpp"
#include "Math/Screen.hpp"
#include "util/Macros.hpp"
#include "Asset.hpp"

#include <iostream>

//------------------------------------------------------------------------------
void
TrafficRenderer::DrawFlarm(Canvas& canvas,
                           const TrafficLook& traffic_look,
                           const FlarmTarget& traffic,
                           const Angle angle,
                           const TargetColor color,
                           const PixelPoint pt) noexcept
  {
  // Create point array that will form that arrow polygon
  BulkPixelPoint arrow[] = 
    {
    { -4,  6 },
    {  0, -8 },
    {  4,  6 },
    {  0,  3 },
    };

  // Select brush depending on AlarmLevel
  switch (traffic.alarm_level)
    {
    case RemoteTarget::AlarmType::LOW:
    case RemoteTarget::AlarmType::INFO_ALERT:
      canvas.Select(traffic_look.warning_brush);
      break;
    case RemoteTarget::AlarmType::IMPORTANT:
    case RemoteTarget::AlarmType::URGENT:
      canvas.Select(traffic_look.alarm_brush);
      break;
    case RemoteTarget::AlarmType::NONE:
      if (traffic.relative_altitude > (const RoughAltitude)50)
      {
      canvas.Select(traffic_look.safe_above_brush);
      }
    else if (traffic.relative_altitude > (const RoughAltitude)-50)
      {
      canvas.Select(traffic_look.warning_in_altitude_range_brush);
      }
    else
      {
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

  switch (color)
    {
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

//------------------------------------------------------------------------------
void
TrafficRenderer::DrawAdsb(Canvas& canvas,
                          const TrafficLook& traffic_look,
                          [[maybe_unused]] const AdsbTarget& traffic,
                          const Angle angle,
                          const PixelPoint pt) noexcept
  {
  // Create ATC style glyph
  PixelPoint glyph[2][2];
  glyph[0][0].x = -4;
  glyph[0][0].y =  0;
  glyph[0][1].x =  4;
  glyph[0][1].y =  0;
  glyph[1][0].x =  0;
  glyph[1][0].y =  0;
  glyph[1][1].x =  0;
  glyph[1][1].y = -10;

  // Rotate and shift the glyph
  for (int j = 0; j < 2; j++)
    for (int k = 0; k < 2; k++)
      PointRotateShift(glyph[j][k],
                       pt,
                       angle,
                       Layout::Scale(100u));

  canvas.Select(traffic_look.safe_above_brush);

  // Select black pen
  if (IsDithered())
    canvas.Select(Pen(Layout::FastScale(2), COLOR_BLACK));
  else
    canvas.SelectBlackPen();

#if 0
  // Rotate and shift the arrow to the right position and angle
  PolygonRotateShift(glyph, pt, angle, Layout::Scale(100U));

  // Draw the arrow
  canvas.DrawPolygon(glyph, ARRAY_SIZE(glyph));
#else
  // Draw the lines
  for (int j = 0; j < 2; j++)
    canvas.DrawLine(glyph[j][0], glyph[j][1]);
#endif

//  canvas.SelectHollowBrush();
//  canvas.DrawCircle(pt, Layout::FastScale(11u));
  }

//------------------------------------------------------------------------------
void
TrafficRenderer::DrawGliderLink(Canvas& canvas,
                                const TrafficLook& traffic_look,
                                [[maybe_unused]] const GliderLinkTraffic& traffic,
                                const Angle angle,
                                const PixelPoint pt) noexcept
  {
  // Create point array that will form that arrow polygon
  BulkPixelPoint arrow[] =
    {
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
