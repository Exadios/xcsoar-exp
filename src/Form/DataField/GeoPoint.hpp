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

#include "Base.hpp"
#include "Geo/GeoPoint.hpp"
#include "Geo/CoordinateFormat.hpp"

/**
 * This #DataField implementation stores a #GeoPoint.
 */
class GeoPointDataField final : public DataField {
  GeoPoint value;

  const CoordinateFormat format;

  /**
   * For GetAsString().  Must be mutable because the method is const.
   */
  mutable TCHAR string_buffer[64];

public:
  GeoPointDataField(GeoPoint _value, CoordinateFormat _format,
                    DataFieldListener *listener=nullptr) noexcept
    :DataField(Type::GEOPOINT, false, listener),
     value(_value), format(_format) {}

  CoordinateFormat GetFormat() const noexcept {
    return format;
  }

  GeoPoint GetValue() const noexcept {
    return value;
  }

  void SetValue(GeoPoint _value) noexcept {
    value = _value;
  }

  void ModifyValue(GeoPoint _value) noexcept;

  /* virtual methods from class DataField */
  const TCHAR *GetAsString() const noexcept override;
};
