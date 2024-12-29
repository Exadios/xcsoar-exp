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
 * \addtogroup Renderer
 * \{
 */

#include "Surveillance/Color.hpp"

struct PixelPoint;
class Canvas;
struct TrafficLook;
struct FlarmTarget;
struct AdsbTarget;
struct GliderLinkTraffic;
class Angle;

/**
 * This namespace contains functions to render remote traffic on the main
 * map.
 */
namespace TrafficRenderer
  {
  /**
   * Draw a FLARM target.
   * @param canvas The canvas representing the map display.
   * @param traffic_look The visual attributes of the target.
   * @param traffic The target.
   * @param angle The heading or track angle of the #traffic.
   * @param color The color to make the #traffic.
   * @param pt The pixel location of #traffic
   */
  void
  DrawFlarm(Canvas &canvas,
            const TrafficLook &traffic_look,
            const FlarmTarget &traffic,
            const Angle angle,
            const TargetColor color,
            const PixelPoint pt) noexcept;

  /**
   * Draw an ADSB target.
   * @param canvas The canvas representing the map display.
   * @param traffic_look The visual attributes of the target.
   * @param traffic The target.
   * @param angle The heading or track angle of the #traffic.
   * @param color The color to make the #traffic.
   * @param pt The pixel location of #traffic
   */
  void
  DrawAdsb(Canvas &canvas,
           const TrafficLook &traffic_look,
           const AdsbTarget &traffic,
           const Angle angle,
           const PixelPoint pt) noexcept;

  /**
   * Draw a target obtained via the \ref GliderLink (i.e. SkyLines) system.
   * @param canvas The canvas representing the map display.
   * @param traffic_look The visual attributes of the target.
   * @param traffic The target.
   * @param angle The heading or track angle of the #traffic.
   * @param color The color to make the #traffic.
   * @param pt The pixel location of #traffic
   */
  void
  DrawGliderLink(Canvas &canvas,
                 const TrafficLook &traffic_look,
                 const GliderLinkTraffic &traffic,
                 const Angle angle,
                 const PixelPoint pt) noexcept;
  }

/** \} */
