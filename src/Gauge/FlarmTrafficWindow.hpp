/*
  Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2023 The XCSoar Project
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

#include "ui/window/PaintWindow.hpp"
#include "Surveillance/TargetList.hpp"
#include "Surveillance/TargetStatus.hpp"
#include "TeamCode/Settings.hpp"
#include "Math/FastRotation.hpp"

#include <cstdint>

class Color;
class Brush;
struct FlarmTrafficLook;

/**
 * A Window which renders FLARM traffic.
 */
class FlarmTrafficWindow : public PaintWindow
  {
  protected:
    const FlarmTrafficLook &look;

    /**
     * The distance of the biggest circle in meters.
     */
    double distance = 2000;

    /**
     * The index in the traffic list of the current selection.
     */
    int selection = -1;
    int warning = -1;
    PixelPoint radar_mid;

    /**
     * The minimum distance between the window boundary and the biggest
     * circle in pixels.
     */
    const unsigned h_padding, v_padding;

    /**
     * The radius of the biggest circle in pixels.
     */
    unsigned radius;

    const bool small;

    PixelPoint sc[TargetList::MAX_COUNT];

    bool enable_north_up = false;
    Angle heading = Angle::Zero();
    TargetStatus target_status;
    FastRotation fr;
    FastIntegerRotation fir;
    TargetList traffic;
    Validity data_modified;
    TeamCodeSettings settings;

  public:
    enum class SideInfoType : uint8_t
      {
      RELATIVE_ALTITUDE,
      VARIO,
      } side_display_type = SideInfoType::VARIO;

  public:
    FlarmTrafficWindow(const FlarmTrafficLook &_look,
                       unsigned _h_padding, unsigned _v_padding,
                       bool _small = false) noexcept;

  public:
    [[gnu::pure]]
    bool WarningMode() const noexcept;

    /**
     * Give the currently selected target.
     * @return The currently selected target.
     */
    const TargetPtr GetTarget() const noexcept
      {
      return this->traffic.list[selection];
      }

    /**
     * Set the quick access to the currently selected target.
     * @param target The current target. May be nullptr in which case the
     *               \ref RemoteTarget currently pointed to is released here
     *               and may, if conditions elsewhere allow, cause the 
     *               \ref RemoteTarget previously reference to destruct.
     */
    void SetTarget(const TargetPtr target) noexcept
      {
      if (target == nullptr)
        this->selection = -1;
      else
        {
        this->selection = IndexTarget(target->id);
        this->Invalidate();
        }
      }

    /**
     * Set the quick access to the currently selected target.
     * @param id The id of the current target. If no target having id is
     *           currently in the list then the \ref RemoteTarget currently
     *           pointed to is released here and may, if conditions elsewhere
     *           allow, cause the \ref RemoteTarget previously reference to
     *           destruct.
     */
    void SetTarget(const TargetId& id) noexcept
      {
      SetTarget(this->traffic.FindTraffic(id));
      }

    void NextTarget() noexcept;
    void PrevTarget() noexcept;
    bool SelectNearTarget(PixelPoint p, int max_distance) noexcept;

    void SetDistance(double _distance) noexcept
      {
      distance = _distance;
      this->Invalidate();
      }

    /**
     * This function paints the TrafficRadar onto the given canvas
     * @param canvas The canvas to paint on
     */
    void Paint(Canvas& canvas) noexcept;

  protected:
    [[gnu::pure]]
    double RangeScale(double d) const noexcept;

    /**
     * Checks whether the selection is still on the valid target and if not
     * tries to select the next one.
     * @param id The target we think we are on.
     * @param pt The point at which we think the target might be at.
     */
    void UpdateSelector(TargetId id, PixelPoint pt) noexcept;
    void UpdateWarnings() noexcept;

    /**
     * This should be called when the radar needs to be repainted
     * @param new_direction The new aircraft heading or track.
     * @param new_traffic The new list of remote targets.
     * @param new_settings The new team codes.
     * @param target_status Present status of the remote system.
     */
    void Update(Angle new_direction,
                const TargetList& new_traffic,
                const TeamCodeSettings& new_settings,
                TargetStatus target_status) noexcept;
    void PaintRadarNoTraffic(Canvas& canvas) const noexcept;
    void PaintRadarNoGo(Canvas& canvas) const noexcept;
    void PaintRadarTarget(Canvas& canvas,
                          const RemoteTarget& traffic,
                          unsigned i) noexcept;
    void PaintRadarTraffic(Canvas& canvas) noexcept;

    /**
     * Paint a single remote target onto a canvas which is small - smaller
     * that the full screen canvas.
     * @param canvas The small canvas
     * @param traffic The remote target
     * @param i
     * @param text_color
     * @param arrow_brush The brush to use to paint the target outline.
     */
    void PaintTargetInfoSmall(Canvas& canvas,
                              const RemoteTarget& traffic,
                              unsigned i,
                              const Color& text_color,
                              const Brush& arrow_brush) noexcept;

    void PaintRadarPlane(Canvas& canvas) const noexcept;
    void PaintRadarBackground(Canvas& canvas) const noexcept;
    void PaintNorth(Canvas& canvas) const noexcept;

    /**
     * Find the index in \ref TargetList::list for the given id.
     * @param id The \ref TargetId of the desired record.
     * @return The index in the list or -1 if not found.
     */
    int IndexTarget(const TargetId& id) const noexcept;

  protected:
    /* virtual methods from class Window */
    void OnResize(PixelSize new_size) noexcept override;

    /* virtual methods from class PaintWindow */
    void OnPaint(Canvas &canvas) noexcept override;

  /* Temporary debug routines. */
  private:
#ifndef NDEBUG
    void WalkTheList() const noexcept;
#endif

  };
