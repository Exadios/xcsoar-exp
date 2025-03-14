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
#include "util/StaticString.hxx"

class DataFieldBoolean final : public DataField {
private:
  bool mValue;

  StaticString<32> true_text;
  StaticString<32> false_text;

public:
  DataFieldBoolean(bool _value,
                   const TCHAR *_true_text, const TCHAR *_false_text,
                   DataFieldListener *listener=nullptr) noexcept
    :DataField(Type::BOOLEAN, true, listener),
     mValue(_value),
     true_text(_true_text), false_text(_false_text) {}

  bool GetValue() const noexcept {
    return mValue;
  }

  void SetValue(bool new_value) noexcept {
    mValue = new_value;
  }

  void ModifyValue(bool new_value) noexcept {
    if (new_value != GetValue()) {
      SetValue(new_value);
      Modified();
    }
  }

  /* virtual methods from class DataField */
  void Inc() noexcept override;
  void Dec() noexcept override;
  const TCHAR *GetAsString() const noexcept override;
  ComboList CreateComboList(const TCHAR *reference) const noexcept override;
  void SetFromCombo(int i, const TCHAR *s) noexcept override;
};
