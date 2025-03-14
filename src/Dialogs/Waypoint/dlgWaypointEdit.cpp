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

#include "WaypointDialogs.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Message.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Form/DataField/Enum.hpp"
#include "Form/DataField/GeoPoint.hpp"
#include "Form/DataField/Listener.hpp"
#include "UIGlobals.hpp"
#include "Waypoint/Waypoint.hpp"
#include "FormatSettings.hpp"
#include "Language/Language.hpp"

class WaypointEditWidget final : public RowFormWidget, DataFieldListener {
  enum Rows {
    NAME,
    SHORTNAME,
    COMMENT,
    LOCATION,
    ELEVATION,
    TYPE,
  };

  Waypoint value;

  bool modified;

public:
  WaypointEditWidget(const DialogLook &look, const Waypoint &_value) noexcept
    :RowFormWidget(look), value(_value), modified(false) {}

  const Waypoint &GetValue() const {
    return value;
  }

private:
  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  bool Save(bool &changed) noexcept override;

  /* virtual methods from DataFieldListener */
  void OnModified(DataField &) noexcept override {
    modified = true;
  }
};

static constexpr StaticEnumChoice waypoint_types[] = {
  { 0, N_("Turnpoint"), nullptr },
  { 1, N_("Airport"), nullptr },
  { 2, N_("Landable"), nullptr },
  { 3, N_("Mountain Pass"), nullptr },
  { 4, N_("Mountain Top"), nullptr },
  { 5, N_("Transmitter Mast"), nullptr },
  { 6, N_("Tower"), nullptr },
  { 7, N_("Tunnel"), nullptr },
  { 8, N_("Bridge"), nullptr },
  { 9, N_("Power Plant"), nullptr },
  { 0 }
};

void
WaypointEditWidget::Prepare(ContainerWindow &, const PixelRect &) noexcept
{
  AddText(_("Name"), nullptr, value.name.c_str(), this);
  AddText(_("Short Name"), nullptr, value.shortname.c_str(), this);
  AddText(_("Comment"), nullptr, value.comment.c_str(), this);
  Add(_("Location"), nullptr,
      new GeoPointDataField(value.location,
                            UIGlobals::GetFormatSettings().coordinate_format,
                            this));
  AddFloat(_("Altitude"), nullptr,
           _T("%.0f %s"), _T("%.0f"),
           0, 30000, 5, false,
           UnitGroup::ALTITUDE, value.elevation);
  AddEnum(_("Type"), nullptr, waypoint_types,
          value.IsAirport() ? 1u : (value.IsLandable() ? 2u : 0u),
          this);
}

bool
WaypointEditWidget::Save(bool &_changed) noexcept
{
  bool changed = modified;
  value.name = GetValueString(NAME);
  value.shortname = GetValueString(SHORTNAME);
  value.comment = GetValueString(COMMENT);
  value.location = ((GeoPointDataField &)GetDataField(LOCATION)).GetValue();
  changed |= SaveValue(ELEVATION, UnitGroup::ALTITUDE, value.elevation);
  _changed |= changed;

  switch (GetValueEnum(TYPE)) {
  case 1:
    value.flags.turn_point = true;
    value.type = Waypoint::Type::AIRFIELD;
    break;

  case 2:
    value.type = Waypoint::Type::OUTLANDING;
    break;

  case 3:
    value.type = Waypoint::Type::MOUNTAIN_PASS;
    break;

  case 4:
    value.type = Waypoint::Type::MOUNTAIN_TOP;
    break;

  case 5:
    value.type = Waypoint::Type::OBSTACLE;
    break;

  case 6:
    value.type = Waypoint::Type::TOWER;
    break;

  case 7:
    value.type = Waypoint::Type::TUNNEL;
    break;

  case 8:
    value.type = Waypoint::Type::BRIDGE;
    break;

  case 9:
    value.type = Waypoint::Type::POWERPLANT;
    break;

  default:
    value.type = Waypoint::Type::NORMAL;
    value.flags.turn_point = true;
    break;
  };

  return true;
}

WaypointEditResult
dlgWaypointEditShowModal(Waypoint &way_point)
{
  if (UIGlobals::GetFormatSettings().coordinate_format ==
      CoordinateFormat::UTM) {
    ShowMessageBox(
        _("Sorry, the waypoint editor is not yet available for the UTM coordinate format."),
        _("Waypoint Editor"), MB_OK);
    return WaypointEditResult::CANCEL;
  }

  const DialogLook &look = UIGlobals::GetDialogLook();
  TWidgetDialog<WaypointEditWidget>
    dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(),
           look, _("Waypoint Editor"));
  dialog.AddButton(_("OK"), mrOK);
  dialog.AddButton(_("Cancel"), mrCancel);
  dialog.SetWidget(look, way_point);
  const int result = dialog.ShowModal();

  if (result != mrOK)
    return WaypointEditResult::CANCEL;

  if (!dialog.GetChanged())
    return WaypointEditResult::UNMODIFIED;

  way_point = dialog.GetWidget().GetValue();
  return WaypointEditResult::MODIFIED;
}
