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

#include "InfoBoxes/Content/Trace.hpp"
#include "InfoBoxes/Panel/Panel.hpp"
#include "InfoBoxes/Data.hpp"
#include "Renderer/BarographRenderer.hpp"
#include "Renderer/TraceHistoryRenderer.hpp"
#include "Renderer/ThermalBandRenderer.hpp"
#include "Renderer/TaskProgressRenderer.hpp"
#include "ui/dim/Rect.hpp"
#include "Formatter/UserUnits.hpp"
#include "Components.hpp"
#include "Interface.hpp"
#include "Screen/Layout.hpp"
#include "UIGlobals.hpp"
#include "Look/Look.hpp"
#include "Computer/GlideComputer.hpp"
#include "Dialogs/dlgAnalysis.hpp"
#include "util/Macros.hpp"
#include "Language/Language.hpp"
#include "Widget/CallbackWidget.hpp"

[[gnu::const]]
static PixelRect
GetSparkRect(PixelRect rc)
{
  rc.top += Layout::FastScale(2);
  rc.right -= Layout::FastScale(2);
  rc.left += Layout::FastScale(2);
  return rc;
}

void
InfoBoxContentSpark::Paint(Canvas &canvas, const PixelRect &rc,
                           const TraceVariableHistory &var,
                           const bool center) noexcept
{
  if (var.empty())
    return;

  const Look &look = UIGlobals::GetLook();
  TraceHistoryRenderer renderer(look.trace_history, look.vario, look.chart);
  renderer.RenderVario(canvas, GetSparkRect(rc), var, center,
                       CommonInterface::GetComputerSettings().polar.glide_polar_task.GetMC(),
                       CommonInterface::Calculated().common_stats.vario_scale_positive,
                       CommonInterface::Calculated().common_stats.vario_scale_negative * (center? 1:0));
}

void
InfoBoxContentVarioSpark::OnCustomPaint(Canvas &canvas,
                                        const PixelRect &rc) noexcept
{
  Paint(canvas, rc, CommonInterface::Calculated().trace_history.BruttoVario);
}

void
InfoBoxContentNettoVarioSpark::OnCustomPaint(Canvas &canvas,
                                             const PixelRect &rc) noexcept
{
  Paint(canvas, rc, CommonInterface::Calculated().trace_history.NettoVario);
}

void
InfoBoxContentCirclingAverageSpark::OnCustomPaint(Canvas &canvas,
                                                  const PixelRect &rc) noexcept
{
  Paint(canvas, rc,
        CommonInterface::Calculated().trace_history.CirclingAverage,
        false);
}

void
InfoBoxContentSpark::SetVSpeedComment(InfoBoxData &data,
                                      const TraceVariableHistory &var,
                                      Validity validity) noexcept
{
  if (var.empty())
    return;

  data.SetCommentFromVerticalSpeed(var.last());
  data.SetCustom(validity.ToInteger());
}

void
InfoBoxContentVarioSpark::Update(InfoBoxData &data) noexcept
{
  const auto &trace_history = CommonInterface::Calculated().trace_history;

  SetVSpeedComment(data, trace_history.BruttoVario,
                   trace_history.vario_available);
}

void
InfoBoxContentNettoVarioSpark::Update(InfoBoxData &data) noexcept
{
  const auto &trace_history = CommonInterface::Calculated().trace_history;

  SetVSpeedComment(data, trace_history.NettoVario,
                   trace_history.vario_available);
}

void
InfoBoxContentCirclingAverageSpark::Update(InfoBoxData &data) noexcept
{
  const auto &trace_history = CommonInterface::Calculated().trace_history;

  SetVSpeedComment(data, trace_history.CirclingAverage,
                   trace_history.circling_available);
}

void
InfoBoxContentBarogram::Update(InfoBoxData &data) noexcept
{
  const MoreData &basic = CommonInterface::Basic();
  TCHAR sTmp[32];

  if (basic.NavAltitudeAvailable()) {
    FormatUserAltitude(basic.nav_altitude, sTmp,
                       ARRAY_SIZE(sTmp));
    data.SetComment(sTmp);
  } else
    data.SetCommentInvalid();

  // TODO: use an appropriate digest
  data.SetCustom(basic.location_available.ToInteger() +
                 basic.gps_altitude_available.ToInteger() +
                 basic.baro_altitude_available.ToInteger() +
                 basic.pressure_altitude_available.ToInteger() +
                 basic.static_pressure_available.ToInteger());
}

void
InfoBoxContentBarogram::OnCustomPaint(Canvas &canvas, const PixelRect &rc) noexcept
{
  const Look &look = UIGlobals::GetLook();
  RenderBarographSpark(canvas, GetSparkRect(rc),
                       look.chart, look.cross_section,
                       look.info_box.inverse,
                       glide_computer->GetFlightStats(),
                       CommonInterface::Basic(),
                       CommonInterface::Calculated(), protected_task_manager);
}

static void
ShowAnalysisBarograph() noexcept
{
  dlgAnalysisShowModal(UIGlobals::GetMainWindow(),
                       UIGlobals::GetLook(),
                       CommonInterface::Full(), *glide_computer,
                       &airspace_database,
                       terrain, AnalysisPage::BAROGRAPH);
}

static std::unique_ptr<Widget>
LoadAnalysisBarographPanel([[maybe_unused]] unsigned id) noexcept
{
  return std::make_unique<CallbackWidget>(ShowAnalysisBarograph);
}

static constexpr
InfoBoxPanel analysis_barograph_infobox_panels[] = {
  { N_("Analysis"), LoadAnalysisBarographPanel },
  { nullptr, nullptr }
};

const InfoBoxPanel *
InfoBoxContentBarogram::GetDialogContent() noexcept
{
  return analysis_barograph_infobox_panels;
}

void
InfoBoxContentThermalBand::OnCustomPaint(Canvas &canvas,
                                         const PixelRect &rc) noexcept
{
  const Look &look = UIGlobals::GetLook();
  ThermalBandRenderer renderer(look.thermal_band, look.chart);
  renderer.DrawThermalBandSpark(CommonInterface::Basic(),
                                CommonInterface::Calculated(),
                                CommonInterface::GetComputerSettings(),
                                canvas, rc,
                                CommonInterface::GetComputerSettings().task);
}

void
InfoBoxContentThermalBand::Update(InfoBoxData &data) noexcept
{
  data.SetCustom(CommonInterface::Basic().location_available.ToInteger());
}

void
InfoBoxContentTaskProgress::OnCustomPaint(Canvas &canvas,
                                          const PixelRect &rc) noexcept
{
  const Look &look = UIGlobals::GetLook();
  TaskProgressRenderer renderer(look.map.task);
  renderer.Draw(CommonInterface::Calculated().
                common_stats.ordered_summary,
                canvas, rc,
                look.info_box.inverse);
}

void
InfoBoxContentTaskProgress::Update(InfoBoxData &data) noexcept
{
  // TODO: use an appropriate digest
  data.SetCustom(CommonInterface::Basic().location_available.ToInteger());
}
