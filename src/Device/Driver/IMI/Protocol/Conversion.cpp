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

#include "Conversion.hpp"
#include "time/BrokenDateTime.hpp"
#include "Math/Angle.hpp"
#include "Engine/Waypoint/Waypoint.hpp"
#include "time/Calendar.hxx"

#ifdef _UNICODE
#include <stringapiset.h>
#endif

static constexpr unsigned IMI_SECONDS_IN_MINUTE = 60;
static constexpr unsigned IMI_SECONDS_IN_HOUR = 60*60;
static constexpr unsigned IMI_SECONDS_IN_DAY = 24*60*60;

void
IMI::ConvertToChar(const TCHAR* unicode, char* ascii, int outSize)
{
#ifdef _UNICODE
  WideCharToMultiByte(CP_ACP, 0, unicode, -1, ascii, outSize, "?", nullptr);
#else
  strncpy(ascii, unicode, outSize - 1);
  ascii[outSize - 1] = 0;
#endif
}

IMI::AngleConverter::AngleConverter(Angle angle)
{
  sign = angle.IsNegative();
  double mag(angle.AbsoluteDegrees());
  degrees = static_cast<IMIDWORD> (mag);
  milliminutes = static_cast<IMIDWORD> ((mag - degrees) * 60 * 1000);
}

BrokenDateTime
IMI::ConvertToDateTime(IMI::IMIDATETIMESEC in)
{
  BrokenDateTime out;

  // find year
  for (out.year = 2000; out.year <= 2099; ++out.year) {
    unsigned secondsinyear = DaysInYear(out.year) * IMI_SECONDS_IN_DAY;
    if (in < secondsinyear)
      break;

    in -= secondsinyear;
  }

  // find month
  for (out.month = 1; out.month <= 12; ++out.month) {
    unsigned secondsinmonth = DaysInMonth(out.month, out.year)
      * IMI_SECONDS_IN_DAY;

    if (in < secondsinmonth)
      break;
    in -= secondsinmonth;
  }

  // calculate day
  out.day = (uint8_t)(1 + in / IMI_SECONDS_IN_DAY);
  in %= IMI_SECONDS_IN_DAY;

  // hour, minutes and seconds
  out.hour = (uint8_t)(in / IMI_SECONDS_IN_HOUR);
  in %= IMI_SECONDS_IN_HOUR;

  out.minute = (uint8_t)(in / IMI_SECONDS_IN_MINUTE);
  in %= IMI_SECONDS_IN_MINUTE;

  out.second = (uint8_t)in;

  return out;
}

void
IMI::ConvertWaypoint(const Waypoint &wp, TWaypoint &imiWp)
{
  // set name
  ConvertToChar(wp.name.c_str(), imiWp.name, sizeof(imiWp.name));

  // set latitude
  imiWp.lat = AngleConverter(wp.location.latitude).value;

  // set longitude
  imiWp.lon = AngleConverter(wp.location.longitude).value;
}

void
IMI::ConvertOZ(const Declaration::TurnPoint &tp, bool is_start, bool is_finish,
               TWaypoint &imiWp)
{
  // set observation zones
  if (is_start) {
    // START
    imiWp.oz.style = 3;
    switch (tp.shape) {
    case Declaration::TurnPoint::CYLINDER: // cylinder
      imiWp.oz.A1 = 1800;
      break;
    case Declaration::TurnPoint::LINE: // line
      imiWp.oz.line_only = 1;
      break;
    case Declaration::TurnPoint::DAEC_KEYHOLE: //interpreted as fai sector
    case Declaration::TurnPoint::SECTOR: // fai sector
      imiWp.oz.A1 = 450;
      break;
    }
    imiWp.oz.R1 = (IMIDWORD)std::min(250000u, tp.radius);
  } else if (is_finish) {
    // FINISH
    imiWp.oz.style = 4;
    switch (tp.shape) {
    case Declaration::TurnPoint::CYLINDER: // cylinder
      imiWp.oz.A1 = 1800;
      break;
    case Declaration::TurnPoint::LINE: // line
      imiWp.oz.line_only = 1;
      break;
    case Declaration::TurnPoint::DAEC_KEYHOLE: //interpreted as fai sector
    case Declaration::TurnPoint::SECTOR: // fai sector
      imiWp.oz.A1 = 450;
      break;
    }
    imiWp.oz.R1 = (IMIDWORD)std::min(250000u, tp.radius);
  } else {
    // TPs
    imiWp.oz.style = 2;
    switch (tp.shape) {
    case Declaration::TurnPoint::CYLINDER: // cylinder
      imiWp.oz.A1 = 1800;
      imiWp.oz.R1 = (IMIDWORD)std::min(250000u, tp.radius);
      break;
    case Declaration::TurnPoint::DAEC_KEYHOLE: //interpreted as fai sector
    case Declaration::TurnPoint::SECTOR: // sector
      imiWp.oz.A1 = 450;
      imiWp.oz.R1 = (IMIDWORD)std::min(250000u, tp.radius);
      break;
    case Declaration::TurnPoint::LINE: // line
      assert(0);
      break;
    }
  }

  // other unused data
  imiWp.oz.maxAlt = 0;
  imiWp.oz.reduce = 0;
  imiWp.oz.move = 0;
}
