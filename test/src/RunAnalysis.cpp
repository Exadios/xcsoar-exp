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

#define ENABLE_LOOK
#define ENABLE_DIALOG
#define ENABLE_CMDLINE
#define ENABLE_PROFILE
#define USAGE "DRIVER FILE"

#include "Main.hpp"
#include "ui/window/SingleWindow.hpp"
#include "ui/canvas/BufferCanvas.hpp"
#include "InfoBoxes/InfoBoxLayout.hpp"
#include "Logger/Logger.hpp"
#include "Terrain/RasterTerrain.hpp"
#include "Waypoint/WaypointGlue.hpp"
#include "Dialogs/dlgAnalysis.hpp"
#include "Dialogs/Task/TaskDialogs.hpp"
#include "Dialogs/Dialogs.h"
#include "Dialogs/Airspace/AirspaceWarningDialog.hpp"
#include "Dialogs/DialogSettings.hpp"
#include "UIGlobals.hpp"
#include "Airspace/AirspaceParser.hpp"
#include "Airspace/AirspaceGlue.hpp"
#include "Engine/Waypoint/Waypoints.hpp"
#include "Engine/Airspace/Airspaces.hpp"
#include "Engine/Task/TaskManager.hpp"
#include "Engine/Task/TaskEvents.hpp"
#include "Engine/Task/Ordered/OrderedTask.hpp"
#include "Computer/BasicComputer.hpp"
#include "Computer/GlideComputer.hpp"
#include "Computer/GlideComputerInterface.hpp"
#include "Task/DefaultTask.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "Task/ProtectedRoutePlanner.hpp"
#include "Task/TaskFile.hpp"
#include "LocalPath.hpp"
#include "Blackboard/InterfaceBlackboard.hpp"
#include "DebugReplay.hpp"
#include "io/FileLineReader.hpp"
#include "Operation/ConsoleOperationEnvironment.hpp"
#include "Look/Look.hpp"
#include "system/Args.hpp"

#ifdef _WIN32
#include <shellapi.h>
#endif

/* fake symbols: */

#include "Computer/ConditionMonitor/ConditionMonitors.hpp"
#include "Input/InputQueue.hpp"
#include "Logger/Logger.hpp"

void dlgBasicSettingsShowModal() {}
void ShowWindSettingsDialog() {}

void
dlgAirspaceWarningsShowModal([[maybe_unused]] ProtectedAirspaceWarningManager &warnings,
                             [[maybe_unused]] bool auto_close)
{
}

void
dlgStatusShowModal([[maybe_unused]] int page)
{
}

void
ConditionMonitors::Update([[maybe_unused]] const NMEAInfo &basic,
                          [[maybe_unused]] const DerivedInfo &calculated,
                          [[maybe_unused]] const ComputerSettings &settings) noexcept
{
}

bool InputEvents::processGlideComputer(unsigned) { return false; }

void Logger::LogStartEvent([[maybe_unused]] const NMEAInfo &gps_info) {}
void Logger::LogFinishEvent([[maybe_unused]] const NMEAInfo &gps_info) {}
void Logger::LogPoint([[maybe_unused]] const NMEAInfo &gps_info) {}

/* done with fake symbols. */

static RasterTerrain *terrain;

static void
LoadFiles(Airspaces &airspace_database)
{
  ConsoleOperationEnvironment operation;

  terrain = RasterTerrain::OpenTerrain(nullptr, operation).release();

  const AtmosphericPressure pressure = AtmosphericPressure::Standard();
  ReadAirspace(airspace_database, terrain, pressure, operation);
}

static void
LoadReplay(DebugReplay *replay, GlideComputer &glide_computer,
           InterfaceBlackboard &blackboard)
{
  unsigned i = 0;
  while (replay->Next()) {
    const MoreData &basic = replay->Basic();

    glide_computer.ReadBlackboard(basic);
    glide_computer.ProcessGPS();

    if (++i == 8) {
      i = 0;
      glide_computer.ProcessIdle();
    }
  }

  glide_computer.ProcessExhaustive();

  blackboard.ReadBlackboardBasic(glide_computer.Basic());
  blackboard.ReadBlackboardCalculated(glide_computer.Calculated());
}

static DebugReplay *replay;

static void
ParseCommandLine(Args &args)
{
  replay = CreateDebugReplay(args);
  if (replay == nullptr)
    exit(EXIT_FAILURE);
}

static void
Main(UI::Display &display)
{
  const Waypoints way_points;

  InterfaceBlackboard blackboard;
  blackboard.SetComputerSettings().SetDefaults();
  blackboard.SetComputerSettings().polar.glide_polar_task = GlidePolar(1);
  blackboard.SetUISettings().SetDefaults();

  TaskBehaviour task_behaviour;
  task_behaviour.SetDefaults();

  TaskManager task_manager(task_behaviour, way_points);
  task_manager.SetGlidePolar(blackboard.GetComputerSettings().polar.glide_polar_task);

  GlideComputerTaskEvents task_events;
  task_manager.SetTaskEvents(task_events);

  Airspaces airspace_database;

  ProtectedTaskManager protected_task_manager(task_manager,
                                              blackboard.GetComputerSettings().task);

  LoadFiles(airspace_database);

  auto task = LoadDefaultTask(blackboard.GetComputerSettings().task,
                              &way_points);
  if (task)
    protected_task_manager.TaskCommit(*task);

  GlideComputer glide_computer(blackboard.GetComputerSettings(),
                               way_points, airspace_database,
                               protected_task_manager,
                               task_events);
  glide_computer.SetTerrain(terrain);
  glide_computer.SetContestIncremental(false);
  glide_computer.Initialise();

  LoadReplay(replay, glide_computer, blackboard);
  delete replay;

  UI::SingleWindow main_window{display};
  main_window.Create(_T("RunAnalysis"),
                     {640, 480});

  dlgAnalysisShowModal(main_window, *look, blackboard, glide_computer,
                       &airspace_database,
                       terrain);

  delete terrain;
}
