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

struct Look;
namespace UI { class SingleWindow; }
class FullBlackboard;
class GlideComputer;
class Airspaces;
class RasterTerrain;

enum class AnalysisPage {
  BAROGRAPH,
  CLIMB,
  THERMAL_BAND,
  VARIO_HISTOGRAM,
  TASK_SPEED,
  WIND,
  POLAR,
  MACCREADY,
  TEMPTRACE,
  TASK,
  CONTEST,
  AIRSPACE,
  COUNT
};

void
dlgAnalysisShowModal(UI::SingleWindow &parent, const Look &look,
                     const FullBlackboard &blackboard,
                     GlideComputer &glide_computer,
                     const Airspaces *airspaces,
                     const RasterTerrain *terrain,
                     AnalysisPage page=AnalysisPage::COUNT);
