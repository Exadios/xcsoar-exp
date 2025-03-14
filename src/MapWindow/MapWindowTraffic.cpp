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

#include "MapWindow.hpp"
#include "ui/canvas/Icon.hpp"
#include "Screen/Layout.hpp"
#include "Formatter/UserUnits.hpp"
#include "Look/TrafficLook.hpp"
#include "Renderer/TextInBox.hpp"
#include "Renderer/TrafficRenderer.hpp"
#include "Surveillance/TargetFriends.hpp"
#include "Tracking/SkyLines/Data.hpp"
#include "util/StringCompare.hxx"

#ifndef NDEBUG
#include "LogFile.hpp"
#endif

//------------------------------------------------------------------------------
void
MapWindow::DrawFLARMTraffic(Canvas& canvas,
                            const PixelPoint aircraft_pos) const
  {
  // Return if surveillance icons on moving map are disabled
  if (!this->GetMapSettings().show_flarm_on_map)
    return;

  // Return if surveillance data is not available
  const TargetList& targets = this->Basic().target_data.traffic;
  if (targets.IsEmpty())
    return;

  const WindowProjection& projection = this->render_projection;

  // if zoomed in too far out, dont draw traffic since it will be too close to
  // the glider and so will be meaningless (serves only to clutter, cant help
  // the pilot)
  if (projection.GetMapScale() > 7300)
    return;

  canvas.Select(*this->traffic_look.font);

  // Circle through the surveillance targets
  for (auto it = targets.flarm_list.begin(),
            end = targets.flarm_list.end();
       it != end;
       ++it)
    {
    const FlarmTarget* target = it;

    if (!target->location_available)
      continue;

    // Save the location of the surveillance target
    GeoPoint target_loc = target->location;

    // Points for the screen coordinates for the icon, name and average climb
    PixelPoint sc, sc_name, sc_av;

    // If surveillance target not on the screen, move to the next one
    if (auto p = projection.GeoToScreenIfVisible(target_loc))
      sc = *p;
    else
      continue;

    // Draw the name 16 points below the icon
    sc_name = sc;
    sc_name.y -= Layout::Scale(20);

    // Draw the average climb value above the icon
    sc_av = sc;
    sc_av.y += Layout::Scale(5);

    TextInBoxMode mode;
    mode.shape = LabelShape::OUTLINED;

    // JMW TODO enhancement: decluttering of target altitudes (sort by max lift)

    int dx = sc_av.x - aircraft_pos.x;
    int dy = sc_av.y - aircraft_pos.y;

    // only draw labels if not close to aircraft
    if (dx * dx + dy * dy > Layout::Scale(30 * 30))
      {
      // If target callsign/name available draw it to the canvas
      if (target->HasName() && !StringIsEmpty(target->name))
        ::TextInBox(canvas, target->name, sc_name, mode, this->GetClientRect());

      if (target->climb_rate_avg30s >= 0.1)
        {
        // If average climb data available draw it to the canvas
        TCHAR label_avg[100];
        ::FormatUserVerticalSpeed(target->climb_rate_avg30s,
                                  label_avg, false);
        ::TextInBox(canvas, label_avg, sc_av, mode, GetClientRect());
        }
      }

    auto color = TargetFriends::GetFriendColor(target->id);
    TrafficRenderer::DrawFlarm(canvas,
                               traffic_look,
                               *target,
                               target->track - projection.GetScreenAngle(),
                               color,
                               sc);
    }
  }

//------------------------------------------------------------------------------
void
MapWindow::DrawADSBTraffic(Canvas& canvas,
                           const PixelPoint aircraft_pos) const
  {
  // Return if surveillance icons on moving map are disabled
  if (!this->GetMapSettings().show_flarm_on_map)
    return;

  // Return if surveillance data is not available
  const TargetList& targets = this->Basic().target_data.traffic;
  if (targets.IsEmpty())
    return;

  const WindowProjection& projection = this->render_projection;

  // if zoomed in too far out, dont draw traffic since it will be too close to
  // the glider and so will be meaningless (serves only to clutter, cant help
  // the pilot)
  if (projection.GetMapScale() > 7300)
    return;

  canvas.Select(*this->traffic_look.font);

  // Circle through the surveillance targets
  for (auto it = targets.adsb_list.begin(),
            end = targets.adsb_list.end();
       it != end;
       ++it)
    {
    const AdsbTarget* target = it;

    if (!target->location_available)
      continue;

    // Save the location of the surveillance target
    GeoPoint target_loc = target->location;

    // Points for the screen coordinates for the icon, name and average climb
    PixelPoint sc, sc_name, sc_av;

    // If surveillance target not on the screen, move to the next one
    if (auto p = projection.GeoToScreenIfVisible(target_loc))
      sc = *p;
    else
      continue;

    // Draw the name 16 points below the icon
    sc_name = sc;
    sc_name.y -= Layout::Scale(20);

    // Draw the average climb value above the icon
    sc_av = sc;
    sc_av.y += Layout::Scale(5);

    TextInBoxMode mode;
    mode.shape = LabelShape::OUTLINED;

    // JMW TODO enhancement: decluttering of target altitudes (sort by max lift)

    int dx = sc_av.x - aircraft_pos.x;
    int dy = sc_av.y - aircraft_pos.y;

    // only draw labels if not close to aircraft
    if (dx * dx + dy * dy > Layout::Scale(30 * 30))
      {
      // If target callsign/name available draw it to the canvas
      if (target->HasName() && !StringIsEmpty(target->name))
        ::TextInBox(canvas, target->name, sc_name, mode, this->GetClientRect());

      if (target->climb_rate_avg30s >= 0.1)
        {
        // If average climb data available draw it to the canvas
        TCHAR label_avg[100];
        ::FormatUserVerticalSpeed(target->climb_rate_avg30s,
                                  label_avg, false);
        ::TextInBox(canvas, label_avg, sc_av, mode, GetClientRect());
        }
      }

    TrafficRenderer::DrawAdsb(canvas,
                              traffic_look, 
                              *target,
                              target->track - projection.GetScreenAngle(),
                              sc);
    }
  }

//------------------------------------------------------------------------------
void
MapWindow::DrawGLinkTraffic([[maybe_unused]] Canvas& canvas,
                            [[maybe_unused]] const PixelPoint aircraft_pos) const
  {
#ifdef ANDROID
#if 0 // Not implemented for Android yet.

/*
 * \todo
 * Implement #MapWindow::DrawGLinkTraffic() for Android.
 */

  // Return if surveillance icons on moving map are disabled
  if (!this->GetMapSettings().show_flarm_on_map)
    return;

  const GliderLinkTrafficList& traffic = this->Basic().glink_data.traffic;
  if (traffic.IsEmpty())
    return;

  const MoreData& basic = this->Basic();

  const WindowProjection& projection = this->render_projection;

  canvas.Select(*(this->traffic_look.font));

  // Circle through the GliderLink targets
  for (const auto& traf : traffic.list)
    {

    // Save the location of the target
    GeoPoint target_loc = traf.location;

    // Points for the screen coordinates for the icon, name and average climb
    PixelPoint sc, sc_name, sc_av, sc_alt;

    // If GliderLink target not on the screen, move to the next one
    if (auto p = projection.GeoToScreenIfVisible(target_loc))
      sc = *p;
    else
      continue;

    // Draw the callsign above the icon
    sc_name = sc;
    sc_name.x -= Layout::Scale(10);
    sc_name.y -= Layout::Scale(15);

    // Draw the average climb to the right of the icon
    sc_av = sc;
    sc_av.x += Layout::Scale(10);
    sc_av.y -= Layout::Scale(8);

    // Location of altitude label
    sc_alt = sc;
    sc_alt.x -= Layout::Scale(10);
    sc_alt.y -= Layout::Scale(0);

    TextInBoxMode mode;
    mode.shape = LabelShape::OUTLINED;
    mode.align = TextInBoxMode::Alignment::RIGHT;

    // If callsign/name available draw it to the canvas
    if (traf.HasName() && !StringIsEmpty(traf.name))
      ::TextInBox(canvas, traf.name, sc_name,
                  mode, this->GetClientRect());

    if (traf.climb_rate_received)
      {

      // If average climb data available draw it to the canvas
      TCHAR label_avg[100];
      ::FormatUserVerticalSpeed(traf.climb_rate,
                                label_avg, false);
      mode.align = TextInBoxMode::Alignment::LEFT;
      ::TextInBox(canvas, label_avg, sc_av, mode, this->GetClientRect());
      }

    // use GPS altitude to be consistent with GliderLink
    if(basic.gps_altitude_available && traf.altitude_received &&
       fabs(double(traf.altitude) - basic.gps_altitude) >= 100.0)
      {
      // If average climb data available draw it to the canvas
      TCHAR label_alt[100];
      double alt = (double(traf.altitude) - basic.gps_altitude) / 100.0;
      ::FormatRelativeUserAltitude(alt, label_alt, false);

      mode.align = TextInBoxMode::Alignment::RIGHT;
      ::TextInBox(canvas, label_alt, sc_alt, mode, GetClientRect());
      }

    /* Following line does not compile. Find out what we are trying to
     * do here and code to suit.
     */
    auto color = TargetFriends::GetFriendColor(traf->id);
    TrafficRenderer::DrawFlarm(canvas,
                               traffic_look,
                               traf,
                               traf.track - projection.GetScreenAngle(),
                               color,
                               sc);
    }
#endif // 0
#endif // Android
  }

//------------------------------------------------------------------------------
void
MapWindow::DrawTeammate(Canvas& canvas) const
  {
  const TeamInfo &teamcode_info = this->Calculated();

  if (teamcode_info.teammate_available)
    {
    if (auto p = this->render_projection.GeoToScreenIfVisible(teamcode_info.teammate_location))
      this->traffic_look.teammate_icon.Draw(canvas, *p);
    }
  }

#ifdef HAVE_SKYLINES_TRACKING

//------------------------------------------------------------------------------
void
MapWindow::DrawSkyLinesTraffic(Canvas& canvas) const
  {
  if (DisplaySkyLinesTrafficMapMode::OFF == GetMapSettings().skylines_traffic_map_mode ||
      this->skylines_data == nullptr)
    return;

  canvas.Select(*(this->traffic_look.font));

  const std::lock_guard lock{skylines_data->mutex};
  for (auto& i : skylines_data->traffic)
    {
    if (auto p = render_projection.GeoToScreenIfVisible(i.second.location))
      {
      this->traffic_look.teammate_icon.Draw(canvas, *p);
      if (DisplaySkyLinesTrafficMapMode::SYMBOL_NAME == this->GetMapSettings().skylines_traffic_map_mode)
        {
        const auto name_i = this->skylines_data->user_names.find(i.first);
        const TCHAR *name = name_i != this->skylines_data->user_names.end() ?
                              name_i->second.c_str() :
                              _T("");

        StaticString<128> buffer;
        buffer.Format(_T("%s [%um]"), name, i.second.altitude);

        ::TextInBoxMode mode;
        mode.shape = LabelShape::OUTLINED;

        // Draw the name 16 points below the icon
        p->y -= Layout::Scale(10);
        ::TextInBox(canvas, buffer, *p, mode, GetClientRect());
        }
      }
    }
  }

#endif
