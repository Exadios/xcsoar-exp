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

#include "RowFormWidget.hpp"
#include "Form/Edit.hpp"
#include "Form/DataField/File.hpp"
#include "Form/DataField/Date.hpp"
#include "Profile/Profile.hpp"
#include "LocalPath.hpp"
#include "util/ConvertString.hpp"
#include "Formatter/TimeFormatter.hpp"

WndProperty *
RowFormWidget::AddFile(const TCHAR *label, const TCHAR *help,
                       const char *registry_key, const TCHAR *filters,
                       FileType file_type,
                       bool nullable) noexcept
{
  WndProperty *edit = Add(label, help);
  auto *df = new FileDataField();
  df->SetFileType(file_type);
  edit->SetDataField(df);

  if (nullable)
    df->AddNull();

  df->ScanMultiplePatterns(filters);

  if (registry_key != nullptr) {
    const auto path = Profile::GetPath(registry_key);
    if (path != nullptr)
      df->SetValue(path);
  }

  edit->RefreshDisplay();

  return edit;
}

void
RowFormWidget::SetProfile(const char *registry_key, unsigned value) noexcept
{
  Profile::Set(registry_key, value);
}

bool
RowFormWidget::SaveValue(unsigned i, const char *registry_key,
                         TCHAR *string, size_t max_size) const noexcept
{
  if (!SaveValue(i, string, max_size))
    return false;

  Profile::Set(registry_key, string);
  return true;
}

bool
RowFormWidget::SaveValue(unsigned i, const char *registry_key,
                         bool &value, bool negated) const noexcept
{
  if (!SaveValue(i, value, negated))
    return false;

  Profile::Set(registry_key, value);
  return true;
}

bool
RowFormWidget::SaveValue(unsigned i, const char *registry_key,
                         double &value) const noexcept
{
  if (!SaveValue(i, value))
    return false;

  Profile::Set(registry_key, value);
  return true;
}

bool
RowFormWidget::SaveValueFileReader(unsigned i,
                                   const char *registry_key) noexcept
{
  const auto *dfe = (const FileDataField *)GetControl(i).GetDataField();
  Path new_value = dfe->GetValue();
  const auto contracted = ContractLocalPath(new_value);
  if (contracted != nullptr)
    new_value = contracted;

  const WideToUTF8Converter new_value2(new_value.c_str());
  if (!new_value2.IsValid())
    return false;

  const char *old_value = Profile::Get(registry_key, "");
  if (StringIsEqual(old_value, new_value2))
    return false;

  Profile::Set(registry_key, new_value2);
  return true;
}

bool
RowFormWidget::SaveValue(unsigned i,
                         const char *registry_key,
                         BrokenDate &value) const noexcept
{
  const auto &df = (const DataFieldDate &)GetDataField(i);
  assert(df.GetType() == DataField::Type::DATE);

  const auto new_value = df.GetValue();

  if (!new_value.IsPlausible())
    return false;

  if (new_value == value)
    return false;

  TCHAR buffer[0x10];
  FormatISO8601(buffer, new_value);
  Profile::Set(registry_key, buffer);
  value = new_value;
  return true;
}

bool
RowFormWidget::SaveValue(unsigned i,
                         const char *registry_key,
                         std::chrono::seconds &value) const noexcept
{
  if (!SaveValue(i, value))
    return false;

  Profile::Set(registry_key, value);
  return true;
}
