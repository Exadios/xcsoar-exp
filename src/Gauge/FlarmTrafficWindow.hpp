/*
  Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
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
     * The distance of the biggest circle in meters. That is to say the
     * maximum distance that can be currently plotted on the radar.
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
    /**
     * Test if at least one target is issuing a warning.
     * @return If there is at least one target issuing a warning then true.
     */
    bool WarningMode() const noexcept;

    /**
     * Give the currently selected target.
     * @return The currently selected target or nullptr if none selected.
     */
    const FlarmTarget* GetTarget() const noexcept
      {
      return (this->selection >= 0) ? 
        &this->traffic.flarm_list[selection] :
        nullptr;
      }

    /**
     * Set the quick access to the currently selected target.
     * @param target The current target. May be nullptr in which case the
     *               \ref RemoteTarget currently pointed to is released here
     *               and may, if conditions elsewhere allow, cause the 
     *               \ref RemoteTarget previously reference to destruct.
     */
    void SetTarget(const FlarmTarget* target) noexcept
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
      SetTarget(this->traffic.FindFlarmTraffic(id));
      }

    /**
     * If a warning is currently being displayed then do nothing otherwise
     * advance the index, \ref this->selection, to the next FLARM target
     * wrapping around if necessary. If the FLARM list is empty then
     * \ref this->selection is set to -1.
     */
    void NextTarget() noexcept;

    /**
     * If a warning is currently being displayed then do nothing otherwise
     * retard the index, \ref this->selection, to the previous FLARM target
     * wrapping around if necessary. If the FLARM list is empty then
     * \ref this->selection is set to -1.
     */
    void PrevTarget() noexcept;

    bool SelectNearTarget(PixelPoint p, int max_distance) noexcept;

    void SetDistance(double distance) noexcept
      {
      this->distance = distance;
      this->Invalidate();
      }

    /**
     * This function paints the TrafficRadar onto the given canvas
     * @param canvas The \ref Canvas representing the radar.
     */
    void Paint(Canvas& canvas) noexcept;

  protected:
    /**
     * The distance to the own plane.
     * @param d Distance in meters to the own plane.
     * @return The distance to own plane in pixels.
     */
    [[gnu::pure]]
    double RangeScale(double d) const noexcept;

    /**
     * Checks whether the selection is still on the valid target and if not
     * tries to select the next one.
     * @param id The target we think we are on.
     * @param pt The point at which we think the target might be at.
     */
    void UpdateSelector(TargetId id, PixelPoint pt) noexcept;

    /**
     * Iterates through the FLARM traffic list, finds the target with 
     * the highest alarm level and saves it to \ref this->warning. If
     * no warnings sre currently active then \ref this->warning is set
     * to -1.
     */
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

    /**
     * Paints the text "No Traffic" on the large radar display.
     * @param canvas The canvas representing the large FLARM window.
     */
    void PaintRadarNoTraffic(Canvas& canvas) const noexcept;

    /**
     * Paints the text "No Go" on the large radar display.
     * @param canvas The canvas representing the large FLARM window.
     */
    void PaintRadarNoGo(Canvas& canvas) const noexcept;

    /**
     * Paint a FLARM target with warning and alert properties.
     * @param canvas The radar canvas.
     * @param target The FLARM target to paint. No check is made to check
     *               that this is really a FLARM target. It is up to the
     *               caller to make sure that it is.
     * @param i The index in the FLARM target list.
     */
    void PaintRadarFlarmTarget(Canvas& canvas,
                               const RemoteTarget& target,
                               unsigned i) noexcept;

    /**
     * Paint an ADSB target.
     * @param canvas The radar canvas.
     * @param target The ADSB target to paint. No check is made to check
     *               that this is really a ADSB target. It is up to the
     *               caller to make sure that it is.
     * @param i The index in the ADSB target list.
     */
    void PaintRadarAdsbTarget(Canvas& canvas,
                              const RemoteTarget& target,
                              unsigned i) noexcept;

    /**
     * Paint all targets on a radar display
     * @param canvas The canavas representing the radar display.
     */
    void PaintRadarTraffic(Canvas& canvas) noexcept;

    /**
     * Paint a single remote target onto a canvas which is small - smaller
     * that the full screen canvas.
     * @param canvas The small canvas
     * @param target The remote target
     * @param i
     * @param text_color
     * @param arrow_brush The brush to use to paint the target outline.
     */
    void PaintTargetInfoSmall(Canvas& canvas,
                              const RemoteTarget& target,
                              unsigned i,
                              const Color& text_color,
                              const Brush& arrow_brush) noexcept;

    /**
     * Paint a plane symbol in the middle of the radar on the given canvas
     * @param canvas The \ref Canvas representing the radar.
     */
    void PaintRadarPlane(Canvas& canvas) const noexcept;


    /**
     * Paint the radar's fixed furnature.
     * @param canvas The \ref Canvas representing the radar.
     */
    void PaintRadarBackground(Canvas& canvas) const noexcept;

    /**
     * Paint the "N" symbol for north on the radar.
     * @param canvas The \ref Canvas representing the radar.
     */
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
