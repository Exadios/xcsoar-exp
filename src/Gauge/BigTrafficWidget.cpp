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

#include "BigTrafficWidget.hpp"
#include "Dialogs/Traffic/TrafficDialogs.hpp"
#include "Math/Screen.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "ui/event/KeyCode.hpp"
#include "Form/Button.hpp"
#include "Renderer/SymbolButtonRenderer.hpp"
#include "UIState.hpp"
#include "UIGlobals.hpp"
#include "PageActions.hpp"
#include "Look/Look.hpp"
#include "Profile/Profile.hpp"
#ifdef ENABLE_TEAM_FLYING
#include "Surveillance/Flarm/Friends.hpp"
#endif  // ENABLE_TEAM_FLYING
#include "Surveillance/Color.hpp"
#include "Look/FlarmTrafficLook.hpp"
#include "Gauge/FlarmTrafficWindow.hpp"
#include "Language/Language.hpp"
#include "UIUtil/GestureManager.hpp"
#include "Formatter/UserUnits.hpp"
#include "Renderer/UnitSymbolRenderer.hpp"
#include "Input/InputEvents.hpp"
#include "Interface.hpp"
#include "Asset.hpp"

/**
 * A Window which renders remote (currently FLARM and ADSB) traffic, with
 * user interaction.
 */
class FlarmTrafficControl : public FlarmTrafficWindow
  {
protected:
  bool enable_auto_zoom = true, dragging = false;
  unsigned zoom = 2;
  Angle task_direction = Angle::Degrees(-1);
  GestureManager gestures;

public:
  FlarmTrafficControl(const FlarmTrafficLook &look)
  : FlarmTrafficWindow(look, Layout::Scale(10),
                       Layout::GetMinimumControlHeight() + Layout::Scale(2))
    {
    }

protected:
  void CalcAutoZoom();

public:
  /**
   * Update the Flarm display.
   * @param new_direction The new aircraft track or heading.
   * @param new_traffic_data The new list of remote targets.
   * @param new_settings The new team codes.
   * @param flarm_status Present status of the remote target system.
   */
  void Update(Angle new_direction,
              const TargetList& new_traffic_data,
              const TeamCodeSettings& new_settings,
              TargetStatus traffic_status);

  void UpdateTaskDirection(bool show_task_direction, Angle bearing);

  bool GetNorthUp() const
    {
    return enable_north_up;
    }

  void SetNorthUp(bool enabled);

  void ToggleNorthUp()
    {
    SetNorthUp(!GetNorthUp());
    }

  bool GetAutoZoom() const
    {
    return enable_auto_zoom;
    }

  static unsigned GetZoomDistance(unsigned zoom);

  void SetZoom(unsigned _zoom)
    {
    zoom = _zoom;
    SetDistance(GetZoomDistance(_zoom));
    }

  void SetAutoZoom(bool enabled);

  void ToggleAutoZoom()
    {
    SetAutoZoom(!GetAutoZoom());
    }

  bool CanZoomOut() const
    {
    return zoom < 4;
    }

  bool CanZoomIn() const
    {
    return zoom > 0;
    }

  void ZoomOut();
  void ZoomIn();

  void SwitchData();
  void OpenDetails();

protected:
  void PaintTrafficInfo(Canvas &canvas) const;
  void PaintFlarmTrafficInfo(Canvas &canvas) const;
  void PaintAdsbTrafficInfo(Canvas &canvas) const;
  void PaintClimbRate(Canvas &canvas, PixelRect rc, double climb_rate) const;
  void PaintDistance(Canvas &canvas, PixelRect rc, double distance) const;
  void PaintRelativeAltitude(Canvas &canvas, PixelRect rc,
                             double relative_altitude) const;
  void PaintID(Canvas& canvas,
               PixelRect rc,
               const RemoteTarget& traffic) const;
  void PaintTaskDirection(Canvas &canvas) const;

  void StopDragging()
    {
    if (!dragging)
      return;

    dragging = false;
    ReleaseCapture();
    }

protected:
  bool OnMouseGesture(const TCHAR* gesture);

  /* virtual methods from class Window */
  void OnCreate() noexcept override;
  bool OnMouseMove(PixelPoint p, unsigned keys) noexcept override;
  bool OnMouseDown(PixelPoint p) noexcept override;
  bool OnMouseUp(PixelPoint p) noexcept override;
  bool OnMouseDouble(PixelPoint p) noexcept override;
  bool OnKeyDown(unsigned key_code) noexcept override;
  void OnCancelMode() noexcept override;

  /* virtual methods from class PaintWindow */
    void OnPaint(Canvas &canvas) noexcept override;
  };

//------------------------------------------------------------------------------
void
FlarmTrafficControl::OnCreate() noexcept
  {
  FlarmTrafficWindow::OnCreate();

  const TrafficSettings &settings = CommonInterface::GetUISettings().traffic;

  Profile::GetEnum(ProfileKeys::FlarmSideData, side_display_type);
  enable_auto_zoom = settings.auto_zoom;
  enable_north_up = settings.north_up;
  }

//------------------------------------------------------------------------------
unsigned
FlarmTrafficControl::GetZoomDistance(unsigned zoom)
  {
  switch (zoom)
    {
    case 0:
      return 500;
    case 1:
      return 1000;
    case 3:
      return 5000;
    case 4:
      return 10000;
    case 2:
    default:
      return 2000;
    }
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::SetNorthUp(bool enabled)
  {
  TrafficSettings &settings = CommonInterface::SetUISettings().traffic;
  settings.north_up = enable_north_up = enabled;
  Profile::Set(ProfileKeys::FlarmNorthUp, enabled);
  //north_up->SetState(enabled);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::SetAutoZoom(bool enabled)
  {
  TrafficSettings &settings = CommonInterface::SetUISettings().traffic;
  settings.auto_zoom = enable_auto_zoom = enabled;
  Profile::Set(ProfileKeys::FlarmAutoZoom, enabled);
  //auto_zoom->SetState(enabled);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::CalcAutoZoom()
  {
  /**
   * \ todo Include ADSB in calculation of auto zoom (Issue 5)
   */
  bool warning_mode = WarningMode();
  RoughDistance zoom_dist = 0;

  for (auto it = this->traffic.flarm_list.begin(),
       end = this->traffic.flarm_list.end();
       it != end; ++it)
    {
    if (warning_mode && !it->HasAlarm())
      continue;

    zoom_dist = std::max(it->distance, zoom_dist);
    }

  double zoom_dist2 = zoom_dist;
  for (unsigned i = 0; i <= 4; i++)
    {
    if (i == 4 || GetZoomDistance(i) >= zoom_dist2)
      {
      SetZoom(i);
      break;
      }
    }
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::Update(Angle new_direction,
                            const TargetList& new_traffic_data,
                            const TeamCodeSettings &new_settings,
                            TargetStatus traffic_status)
  {
  FlarmTrafficWindow::Update(new_direction,
                             new_traffic_data,
                             new_settings,
                             traffic_status);
  if (enable_auto_zoom || WarningMode())
    CalcAutoZoom();
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::UpdateTaskDirection(bool show_task_direction, Angle bearing)
  {
  if (!show_task_direction)
    task_direction = Angle::Degrees(-1);
  else
    task_direction = bearing.AsBearing();
  }

/**
 * Zoom out one step
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::ZoomOut()
  {
  if (WarningMode())
    return;

  if (zoom < 4)
    SetZoom(zoom + 1);

  SetAutoZoom(false);
  }

/**
 * Zoom in one step
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::ZoomIn()
  {
  if (WarningMode())
    return;

  if (zoom > 0)
    SetZoom(zoom - 1);

  SetAutoZoom(false);
  }

/**
 * Paints an arrow into the direction of the current task leg
 * @param canvas The canvas to paint on
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintTaskDirection(Canvas &canvas) const
  {
  if (task_direction.IsNegative())
    return;

  canvas.Select(look.radar_pen);
  canvas.SelectHollowBrush();

  BulkPixelPoint triangle[3];
  triangle[0].x = 0;
  triangle[0].y = -radius / Layout::FastScale(1) + 15;
  triangle[1].x = 7;
  triangle[1].y = triangle[0].y + 30;
  triangle[2].x = -triangle[1].x;
  triangle[2].y = triangle[1].y;

  PolygonRotateShift(triangle, radar_mid,
                     task_direction - (enable_north_up ?
                                       Angle::Zero() : heading),
                     Layout::FastScale(100u));

  // Draw the arrow
  canvas.DrawPolygon(triangle, 3);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintClimbRate(Canvas &canvas, PixelRect rc,
                                    double climb_rate) const
  {
  // Paint label
  canvas.Select(look.info_labels_font);
  const unsigned label_width = canvas.CalcTextSize(_("Vario")).width;
  canvas.DrawText(rc.GetTopRight().At(-(int)label_width, 0), _("Vario"));

  // Format climb rate
  TCHAR buffer[20];
  Unit unit = Units::GetUserVerticalSpeedUnit();
  FormatUserVerticalSpeed(climb_rate, buffer, false);

  // Calculate unit size
  canvas.Select(look.info_units_font);
  const unsigned unit_width = UnitSymbolRenderer::GetSize(canvas, unit).width;
  const unsigned unit_height =
    UnitSymbolRenderer::GetAscentHeight(look.info_units_font, unit);

  unsigned space_width = unit_width / 3;

  // Calculate value size
  canvas.Select(look.info_values_font);
  const unsigned value_height = look.info_values_font.GetAscentHeight();
  const unsigned value_width = canvas.CalcTextSize(buffer).width;

  // Calculate positions
  const int max_height = std::max(unit_height, value_height);
  const int y = rc.top + look.info_units_font.GetHeight() + max_height;

  const int unit_x = rc.right - unit_width;
  const int unit_y = y - unit_height;

  const int value_x = unit_x - space_width - value_width;
  const int value_y = y - value_height;

  // Paint value
  canvas.DrawText({value_x, value_y}, buffer);

  // Paint unit
  canvas.Select(look.info_units_font);
  UnitSymbolRenderer::Draw(canvas, {unit_x, unit_y},
                           unit, look.unit_fraction_pen);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintDistance(Canvas &canvas, PixelRect rc,
                                   double distance) const
  {
  // Format distance
  TCHAR buffer[20];
  Unit unit = FormatUserDistanceSmart(distance, buffer, false, 1000);

  // Calculate unit size
  canvas.Select(look.info_units_font);
  const unsigned unit_width = UnitSymbolRenderer::GetSize(canvas, unit).width;
  const unsigned unit_height =
    UnitSymbolRenderer::GetAscentHeight(look.info_units_font, unit);

  const unsigned space_width = unit_width / 3;

  // Calculate value size
  canvas.Select(look.info_values_font);
  const unsigned value_height = look.info_values_font.GetAscentHeight();
  const unsigned value_width = canvas.CalcTextSize(buffer).width;

  // Calculate positions
  const unsigned max_height = std::max(unit_height, value_height);

  const auto p0 = rc.GetBottomLeft();

  // Paint value
  canvas.DrawText(p0.At(0, -(int)value_height), buffer);

  // Paint unit
  canvas.Select(look.info_units_font);
  UnitSymbolRenderer::Draw(canvas,
                           p0.At(value_width + space_width,
                                 -(int)unit_height),
                           unit, look.unit_fraction_pen);


  // Paint label
  canvas.Select(look.info_labels_font);
  canvas.DrawText(p0.At(0, -int(max_height + look.info_labels_font.GetHeight())),
                  _("Distance"));
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintRelativeAltitude(Canvas &canvas, PixelRect rc,
    double relative_altitude) const
  {
  // Format relative altitude
  TCHAR buffer[20];
  Unit unit = Units::GetUserAltitudeUnit();
  FormatRelativeUserAltitude(relative_altitude, buffer, false);

  // Calculate unit size
  canvas.Select(look.info_units_font);
  const unsigned unit_width = UnitSymbolRenderer::GetSize(canvas, unit).width;
  const unsigned unit_height =
    UnitSymbolRenderer::GetAscentHeight(look.info_units_font, unit);

  const unsigned space_width = unit_width / 3;

  // Calculate value size
  canvas.Select(look.info_values_font);
  const unsigned value_height = look.info_values_font.GetAscentHeight();
  const unsigned value_width = canvas.CalcTextSize(buffer).width;

  // Calculate positions
  const unsigned max_height = std::max(unit_height, value_height);

  const auto p0 = rc.GetBottomRight();

  // Paint value
  canvas.DrawText(p0.At(-int(unit_width + space_width + value_width),
                        -(int)value_height),
                  buffer);

  // Paint unit
  canvas.Select(look.info_units_font);
  UnitSymbolRenderer::Draw(canvas,
                           p0.At(-(int)unit_width, -(int)unit_height),
                           unit, look.unit_fraction_pen);


  // Paint label
  canvas.Select(look.info_labels_font);
  const unsigned label_width = canvas.CalcTextSize(_("Rel. Alt.")).width;
  canvas.DrawText(p0.At(-(int)label_width,  -int(max_height + look.info_labels_font.GetHeight())),
                  _("Rel. Alt."));
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintID(Canvas& canvas,
                             PixelRect rc,
                             const RemoteTarget& traffic) const
  {
  TCHAR buffer[20];

  unsigned font_size;
  if (traffic.HasName())
    {
    canvas.Select(look.call_sign_font);
    font_size = look.call_sign_font.GetHeight();

    _tcscpy(buffer, traffic.name);
    }
  else
    {
    canvas.Select(look.info_labels_font);
    font_size = look.info_labels_font.GetHeight();

    traffic.id.Format(buffer);
    }

  if (!WarningMode())
    {
#ifdef ENABLE_TEAM_FLYING
    /**
     * \todo
     * pfb: Implement ENABLE_TEAM_FLYING in build. (Issue 5)
     */
    // Team color dot
    TargetColor team_color = FlarmFriends::GetFriendColor(traffic.id);
#else
    TargetColor team_color = TargetColor::NONE;
#endif  // ENABLE_TEAM_FLYING

    // If team color found -> draw a colored circle in front of the name
    if (team_color != TargetColor::NONE)
      {
      switch (team_color)
        {
        case TargetColor::GREEN:
          canvas.Select(look.team_brush_green);
          break;
        case TargetColor::BLUE:
          canvas.Select(look.team_brush_blue);
          break;
        case TargetColor::YELLOW:
          canvas.Select(look.team_brush_yellow);
          break;
        case TargetColor::MAGENTA:
          canvas.Select(look.team_brush_magenta);
          break;
        default:
          break;
        }

      canvas.SelectNullPen();
      canvas.DrawCircle(rc.GetTopLeft().At(Layout::FastScale(7u), (font_size / 2)),
                        Layout::FastScale(7u));

      rc.left += Layout::FastScale(16);
      }
    }

  canvas.DrawText(rc.GetTopLeft(), buffer);
  }

/**
 *
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintTrafficInfo(Canvas& canvas) const
  {
  if (this->flarm_selection != -1)
    this->PaintFlarmTrafficInfo(canvas);
  else if (this->adsb_selection != -1)
    this->PaintAdsbTrafficInfo(canvas);
  return;
  }

/**
 * Paints the basic info for the selected FLARM target on the given canvas
 * @param canvas The canvas to paint on
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintFlarmTrafficInfo(Canvas& canvas) const
  {
  // Don't paint numbers if no plane selected
  if (this->flarm_selection == -1 && !this->WarningMode())
    return;

  // Shortcut to the selected traffic
  FlarmTarget ft = this->traffic.flarm_list[this->WarningMode() ?
                                              this->warning :
                                              this->flarm_selection];
  assert(ft.IsDefined());

  const unsigned padding = Layout::GetTextPadding();
  PixelRect rc;
  rc.left = padding;
  rc.top = padding;
  rc.right  = canvas.GetWidth()  - padding;
  rc.bottom = canvas.GetHeight() - padding;

  // Set the text color and background
  switch (ft.alarm_level)
    {
    case RemoteTarget::AlarmType::LOW:
    case RemoteTarget::AlarmType::INFO_ALERT:
      canvas.SetTextColor(look.warning_color);
      break;
    case RemoteTarget::AlarmType::IMPORTANT:
    case RemoteTarget::AlarmType::URGENT:
      canvas.SetTextColor(look.alarm_color);
      break;
    case RemoteTarget::AlarmType::NONE:
      canvas.SetTextColor(look.default_color);
      break;
    }

  canvas.SetBackgroundTransparent();

  // Climb Rate
  if (!this->WarningMode() && ft.climb_rate_avg30s_available)
    PaintClimbRate(canvas, rc, ft.climb_rate_avg30s);

  // Distance
  PaintDistance(canvas, rc, ft.distance);

  // Relative Height
  PaintRelativeAltitude(canvas, rc, ft.relative_altitude);

  // ID / Name
  if (!ft.HasAlarm())
    canvas.SetTextColor(look.selection_color);

  this->PaintID(canvas, rc, ft);
  }

/**
 * Paints the basic info for the selected FLARM target on the given canvas
 * @param canvas The canvas to paint on
 */
//------------------------------------------------------------------------------
void
FlarmTrafficControl::PaintAdsbTrafficInfo(Canvas& canvas) const
  {
  // Don't paint numbers if no plane selected
  if (this->adsb_selection == -1 && !this->WarningMode())
    return;

  // Shortcut to the selected traffic
  AdsbTarget at = this->traffic.adsb_list[this->WarningMode() ?
                                            this->warning :
                                            this->adsb_selection];
  assert(at.IsDefined());

  const unsigned padding = Layout::GetTextPadding();
  PixelRect rc;
  rc.left = padding;
  rc.top = padding;
  rc.right  = canvas.GetWidth()  - padding;
  rc.bottom = canvas.GetHeight() - padding;

  // Set the text color and background
  switch (at.alarm_level)
    {
    case RemoteTarget::AlarmType::LOW:
    case RemoteTarget::AlarmType::INFO_ALERT:
      canvas.SetTextColor(look.warning_color);
      break;
    case RemoteTarget::AlarmType::IMPORTANT:
    case RemoteTarget::AlarmType::URGENT:
      canvas.SetTextColor(look.alarm_color);
      break;
    case RemoteTarget::AlarmType::NONE:
      canvas.SetTextColor(look.default_color);
      break;
    }

  canvas.SetBackgroundTransparent();

  // Climb Rate
  if (!this->WarningMode())
    {
    if (at.climb_rate_received)
      PaintClimbRate(canvas, rc, at.climb_rate);
    else if (at.climb_rate_avg30s_available)
      PaintClimbRate(canvas, rc, at.climb_rate_avg30s);
    }

  // Distance
  PaintDistance(canvas, rc, at.distance);

  // Relative Height
  PaintRelativeAltitude(canvas, rc, at.relative_altitude);

  // ID / Name
  if (!at.HasAlarm())
    canvas.SetTextColor(look.selection_color);

  this->PaintID(canvas, rc, at);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::OnPaint(Canvas &canvas) noexcept
  {
  canvas.ClearWhite();

  PaintTaskDirection(canvas);
  FlarmTrafficWindow::Paint(canvas);
  PaintTrafficInfo(canvas);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::OpenDetails()
  {
  // If warning is displayed -> prevent from opening details dialog
  if (WarningMode())
    return;

  // Don't open the details dialog if no plane selected
  const RemoteTarget* rt = this->GetTarget();
  const FlarmTarget* flarm = dynamic_cast<const FlarmTarget*>(rt);
  const AdsbTarget* adsb   = dynamic_cast<const AdsbTarget*>(rt);
  if (flarm == nullptr)
    {
    if (adsb == nullptr)
      {
      return;
      }
    else
      dlgAdsbTrafficDetailsShowModal(adsb->id);
    }
  else
    dlgFlarmTrafficDetailsShowModal(flarm->id);
  }

//------------------------------------------------------------------------------
static Button
MakeSymbolButton(ContainerWindow &parent, const ButtonLook &look,
                 const TCHAR *caption,
                 const PixelRect &rc,
                 Button::Callback callback) noexcept
  {
  return Button(parent, rc, WindowStyle(),
                std::make_unique<SymbolButtonRenderer>(look, caption),
                std::move(callback));
  }

struct TrafficWidget::Windows
  {
  Button zoom_in_button, zoom_out_button;
  Button previous_item_button, next_item_button;
  Button details_button;
  Button close_button;

  FlarmTrafficControl view;

  Windows(TrafficWidget &widget, ContainerWindow &parent, const PixelRect &r,
          const ButtonLook &button_look, const FlarmTrafficLook &flarm_look)
    : zoom_in_button(MakeSymbolButton(parent, button_look, _T("+"), r,
                                     [&widget]()
        {
        widget.ZoomIn();
        })),
      zoom_out_button(MakeSymbolButton(parent, button_look,
                                       _T("-"), r,
                                       [&widget]()
        {
        widget.ZoomOut();
        })),
      previous_item_button(MakeSymbolButton(parent, button_look,
                                            _T("<"), r,
                                            [&widget]()
        {
        widget.PreviousTarget();
        })),
      next_item_button(MakeSymbolButton(parent, button_look,
                                        _T(">"), r,
                                        [&widget]()
        {
        widget.NextTarget();
        })),
      details_button(parent, button_look,
                     _("Details"), r, WindowStyle(),
                     [&widget]()
        {
        widget.OpenDetails();
        }),
      close_button(parent, button_look,
                   _("Close"), r, WindowStyle(),
                   []()
        {
        PageActions::Restore();
        }),
      view(flarm_look)
        {
        view.Create(parent, r);
        UpdateLayout(r);
        }

  void UpdateLayout(const PixelRect &rc) noexcept;
  };

//------------------------------------------------------------------------------
void
TrafficWidget::Windows::UpdateLayout(const PixelRect &rc) noexcept
  {
  this->view.Move(rc);

  const unsigned margin = Layout::Scale(1);
  const unsigned button_height = Layout::GetMinimumControlHeight();
  const unsigned button_width = std::max(unsigned(rc.right / 6),
                                         button_height);

  const int x1 = rc.right / 2;
  const int x0 = x1 - button_width;
  const int x2 = x1 + button_width;

  const int y0 = margin;
  const int y1 = y0 + button_height;
  const int y3 = rc.bottom - margin;
  const int y2 = y3 - button_height;

  PixelRect button_rc;

  button_rc.left = x0;
  button_rc.top = y0;
  button_rc.right = x1 - margin;
  button_rc.bottom = y1;
  zoom_in_button.Move(button_rc);

  button_rc.left = x1;
  button_rc.right = x2 - margin;
  zoom_out_button.Move(button_rc);

  button_rc.left = x0;
  button_rc.top = y2;
  button_rc.right = x1 - margin;
  button_rc.bottom = y3;
  previous_item_button.Move(button_rc);

  button_rc.left = x1;
  button_rc.right = x2 - margin;
  next_item_button.Move(button_rc);

  button_rc.left = margin;
  button_rc.top = button_height * 3 / 2;
  button_rc.right = button_rc.left + Layout::Scale(50);
  button_rc.bottom = button_rc.top + button_height;
  details_button.Move(button_rc);

  button_rc.right = rc.right - margin;
  button_rc.left = button_rc.right - Layout::Scale(50);
  close_button.Move(button_rc);
  }

TrafficWidget::TrafficWidget() noexcept = default;
TrafficWidget::~TrafficWidget() noexcept = default;

//------------------------------------------------------------------------------
void
TrafficWidget::OpenDetails() noexcept
  {
  this->windows->view.OpenDetails();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::ZoomIn() noexcept
  {
  this->windows->view.ZoomIn();
  this->UpdateButtons();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::ZoomOut() noexcept
  {
  this->windows->view.ZoomOut();
  this->UpdateButtons();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::PreviousTarget() noexcept
  {
  this->windows->view.PrevTarget();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::NextTarget() noexcept
  {
  this->windows->view.NextTarget();
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::SwitchData()
  {
  if (side_display_type == FlarmTrafficWindow::SideInfoType::VARIO)
    side_display_type = FlarmTrafficWindow::SideInfoType::RELATIVE_ALTITUDE;
  else
    side_display_type = FlarmTrafficWindow::SideInfoType::VARIO;

  Profile::SetEnum(ProfileKeys::FlarmSideData, side_display_type);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::SwitchData() noexcept
  {
  this->windows->view.SwitchData();
  }

//------------------------------------------------------------------------------
bool
TrafficWidget::GetAutoZoom() const noexcept
  {
  return this->windows->view.GetAutoZoom();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::SetAutoZoom(bool value) noexcept
  {
  this->windows->view.SetAutoZoom(value);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::ToggleAutoZoom() noexcept
  {
  this->windows->view.ToggleAutoZoom();
  }

//------------------------------------------------------------------------------
bool
TrafficWidget::GetNorthUp() const noexcept
  {
  return this->windows->view.GetNorthUp();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::SetNorthUp(bool value) noexcept
  {
  this->windows->view.SetAutoZoom(value);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::ToggleNorthUp() noexcept
  {
  this->windows->view.ToggleNorthUp();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::Update() noexcept
  {
  const NMEAInfo &basic = CommonInterface::Basic();
  const DerivedInfo &calculated = CommonInterface::Calculated();

  if (CommonInterface::GetUISettings().traffic.auto_close_dialog &&
      basic.target_data.traffic.IsEmpty()                        &&
      /* auto-close only really closes the FLARM radar if the
         "restored" page has no FLARM radar */
      PageActions::GetConfiguredLayout().main != PageLayout::Main::FLARM_RADAR)
    {
    /* this must be deferred, because this method is called from
       within the BlackboardListener, and we must not unregister the
       listener in this context */
    PageActions::DeferredRestore();
    return;
    }

  this->windows->view.Update(basic.track,
                             basic.target_data.traffic,
                             CommonInterface::GetComputerSettings().team_code,
                             basic.target_data.status);

  this->windows->view.UpdateTaskDirection(calculated.task_stats.task_valid &&
                                          calculated.task_stats.current_leg.solution_remaining.IsOk(),
                                          calculated.task_stats.
                                          current_leg.solution_remaining.cruise_track_bearing);

  this->UpdateButtons();
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnMouseMove(PixelPoint p,
                                 [[maybe_unused]] unsigned keys) noexcept
  {
  if (dragging)
    gestures.Update(p);

  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnMouseDown(PixelPoint p) noexcept
  {
  if (!dragging)
    {
    dragging = true;
    SetCapture();
    gestures.Start(p, Layout::Scale(20));
    }

  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnMouseUp(PixelPoint p) noexcept
  {
  if (dragging)
    {
    StopDragging();

    const TCHAR *gesture = gestures.Finish();
    if (gesture && OnMouseGesture(gesture))
      return true;
    }

  if (!WarningMode())
    SelectNearTarget(p, Layout::Scale(15));

  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnMouseDouble([[maybe_unused]] PixelPoint p) noexcept
  {
  StopDragging();
  InputEvents::ShowMenu();
  return true;
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnMouseGesture(const TCHAR* gesture)
  {
  if (StringIsEqual(gesture, _T("U")))
    {
    ZoomIn();
    return true;
    }
  if (StringIsEqual(gesture, _T("D")))
    {
    ZoomOut();
    return true;
    }
  if (StringIsEqual(gesture, _T("UD")))
    {
    SetAutoZoom(true);
    return true;
    }
  if (StringIsEqual(gesture, _T("DR")))
    {
    OpenDetails();
    return true;
    }
  if (StringIsEqual(gesture, _T("RL")))
    {
    SwitchData();
    return true;
    }

  return InputEvents::processGesture(gesture);
  }

//------------------------------------------------------------------------------
void
FlarmTrafficControl::OnCancelMode() noexcept
  {
  FlarmTrafficWindow::OnCancelMode();
  StopDragging();
  }

//------------------------------------------------------------------------------
bool
FlarmTrafficControl::OnKeyDown(unsigned key_code) noexcept
  {
  switch (key_code)
    {
    case KEY_UP:
      if (!HasPointer())
        break;

      ZoomIn();
      return true;

    case KEY_DOWN:
      if (!HasPointer())
        break;

      ZoomOut();
      return true;
    }

  return FlarmTrafficWindow::OnKeyDown(key_code) ||
         InputEvents::processKey(key_code);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::UpdateLayout() noexcept
  {
  this->windows->UpdateLayout(GetContainer().GetClientRect());
  }

//------------------------------------------------------------------------------
void
TrafficWidget::UpdateButtons() noexcept
  {
  const bool unlocked = !this->windows->view.WarningMode();
  const TargetList& traffic = CommonInterface::Basic().target_data.traffic;
  const bool not_empty = !traffic.IsEmpty();
  const bool two_or_more = traffic.GetActiveTrafficCount() >= 2;

  this->windows->zoom_in_button.SetEnabled(unlocked && windows->view.CanZoomIn());
  this->windows->zoom_out_button.SetEnabled(unlocked && windows->view.CanZoomOut());
  this->windows->previous_item_button.SetEnabled(unlocked && two_or_more);
  this->windows->next_item_button.SetEnabled(unlocked && two_or_more);
  this->windows->details_button.SetEnabled(unlocked && not_empty);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::Prepare(ContainerWindow &parent, const PixelRect &_rc) noexcept
  {
  ContainerWidget::Prepare(parent, _rc);

  const Look &look = UIGlobals::GetLook();

  this->windows = std::make_unique<Windows>(*this,
                                            GetContainer(),
                                            GetContainer().GetClientRect(),
                                            look.dialog.button, look.flarm_dialog);
  this->UpdateLayout();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::Show(const PixelRect &rc) noexcept
  {
  // Update Radar and Selection for the first time
  this->Update();

  ContainerWidget::Show(rc);
  this->UpdateLayout();

  /* show the "Close" button only if this is a "special" page */
  this->windows->close_button.SetVisible(CommonInterface::GetUIState().pages.special_page.IsDefined());

  CommonInterface::GetLiveBlackboard().AddListener(*this);
  }

//------------------------------------------------------------------------------
void
TrafficWidget::Hide() noexcept
  {
  CommonInterface::GetLiveBlackboard().RemoveListener(*this);
  ContainerWidget::Hide();
  }

//------------------------------------------------------------------------------
void
TrafficWidget::Move(const PixelRect &rc) noexcept
  {
  ContainerWidget::Move(rc);

  this->UpdateLayout();
  }


//------------------------------------------------------------------------------
bool
TrafficWidget::SetFocus() noexcept
  {
  this->windows->view.SetFocus();
  return true;
  }

//------------------------------------------------------------------------------
void
TrafficWidget::OnGPSUpdate([[maybe_unused]] const MoreData &basic)
  {
  this->Update();
  }
