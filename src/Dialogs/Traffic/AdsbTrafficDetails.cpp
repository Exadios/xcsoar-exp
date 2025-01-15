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

/**
 * @file
 * The ADSB Traffic Details dialog displaying extended information about
 * the ADSB targets.
 */

#include "TrafficDialogs.hpp"
#include "Look/TrafficLook.hpp"
#include "Dialogs/TextEntry.hpp"
#include "Dialogs/Message.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Surveillance/Flarm/FlarmNetRecord.hpp"
#include "Surveillance/RemoteTarget.hpp"
#include "Surveillance/AircraftType.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"
#include "Surveillance/TargetFriends.hpp"
#include "Renderer/ColorButtonRenderer.hpp"
#include "UIGlobals.hpp"
#include "Components.hpp"
#include "Formatter/UserUnits.hpp"
#include "Formatter/AngleFormatter.hpp"
#include "util/StringBuilder.hxx"
#include "util/StringCompare.hxx"
#include "util/Macros.hpp"
#include "Language/Language.hpp"
#include "Interface.hpp"
#include "Blackboard/LiveBlackboard.hpp"
#include "Blackboard/BlackboardListener.hpp"

class AdsbTrafficDetailsWidget final : public RowFormWidget,
                                              NullBlackboardListener
  {
private:
  enum Controls 
    {
    CALLSIGN,
    SPACER1,
    DISTANCE,
    ALTITUDE,
    SPACER2,
    RADIO,
    PLANE,
    };

  WndForm& dialog;

  const TargetId target_id;

public:
  AdsbTrafficDetailsWidget(WndForm& dialog, TargetId target_id)
    : RowFormWidget(dialog.GetLook()),
      dialog(dialog),
      target_id(target_id)
    {
    }

  void CreateButtons(WidgetDialog& buttons);

  /* virtual methods from Widget */

  /**
   * Initialize and instantiate the dialog.
   * @param parent The parent of this dialog.
   * @param rc The size of the dialog.
   */
  void Prepare(ContainerWindow& parent, const PixelRect& rc) noexcept override;

  /**
   *
   */
  void Show(const PixelRect& rc) noexcept override;

  /**
   * Un show the dialog
   */
  void Hide() noexcept override;

private:
  void UpdateChanging(const MoreData &basic);
  void Update();

  void OnCallsignClicked();
  void OnTeamClicked();
  void OnFriendColorClicked(TargetColor color);

  /* virtual methods from BlackboardListener */

  /**
   * New GPS data has been received and calculations may be necessary.
   * @param basic The new GPS data
   */
  void OnGPSUpdate(const MoreData &basic) override
    {
    UpdateChanging(basic);
    }
  };

//------------------------------------------------------------------------------
inline void
AdsbTrafficDetailsWidget::CreateButtons(WidgetDialog& )
  {
  }

//------------------------------------------------------------------------------
void
AdsbTrafficDetailsWidget::Prepare([[maybe_unused]] ContainerWindow& parent,
                                  [[maybe_unused]] const PixelRect& rc) noexcept
  {
  AddReadOnly(_("Callsign"));
  AddSpacer();
  AddReadOnly(_("Distance"));
  AddReadOnly(_("Altitude"));
  AddSpacer();
  AddReadOnly(_("Radio frequency"));
  AddReadOnly(_("Aircraft"));

  Update();
  }

//------------------------------------------------------------------------------
void
AdsbTrafficDetailsWidget::Show(const PixelRect& rc) noexcept
  {
  RowFormWidget::Show(rc);
  Update();
  CommonInterface::GetLiveBlackboard().AddListener(*this);
  }

//------------------------------------------------------------------------------
void
AdsbTrafficDetailsWidget::Hide() noexcept
  {
  CommonInterface::GetLiveBlackboard().RemoveListener(*this);
  RowFormWidget::Hide();
  }

/**
 * Updates all the dialogs fields, that are changing frequently.
 * e.g. climb speed, distance, height
 */
void
AdsbTrafficDetailsWidget::UpdateChanging(const MoreData& basic)
  {
  TCHAR tmp[40];
  const TCHAR *value;

  const AdsbTarget* target =
    basic.target_data.traffic.FindAdsbTraffic(target_id);

  bool target_ok = (target != nullptr) && target->IsDefined();

  // Fill distance/direction field
  if (target_ok)
    {
    FormatUserDistanceSmart(target->distance, tmp, 20, 1000);
    TCHAR *p = tmp + _tcslen(tmp);
    *p++ = _T(' ');
    FormatAngleDelta(p, 20, target->Bearing() - basic.track);
    value = tmp;
    }
  else
    value = _T("--");

  SetText(AdsbTrafficDetailsWidget::DISTANCE, value);

  // Fill altitude field
  if (target_ok)
    {
    TCHAR *p = tmp;
    if (target->altitude_available)
      {
      FormatUserAltitude(target->altitude, p, 20);
      p += _tcslen(p);
      *p++ = _T(' ');
      }

    Angle dir = Angle::FromXY(target->distance, target->relative_altitude);
    FormatVerticalAngleDelta(p, 20, dir);

    value = tmp;
    }
  else
    value = _T("--");

  SetText(AdsbTrafficDetailsWidget::ALTITUDE, value);

  /*
   * Set the aircraft call sign here rather than in #Update because this
   * function is called with #MoreData as an argument and #Update isn't.
   */
  SetText(CALLSIGN, target->name);
  }

/**
 * Updates all the dialogs fields.
 * Should be called on dialog opening as it closes the dialog when the
 * target does not exist.
 */
void
AdsbTrafficDetailsWidget::Update()
  {
  TCHAR tmp[200], tmp_id[7];
  const TCHAR *value;

  // Set the dialog caption
  StringFormatUnsafe(tmp, _T("%s (%s)"),
                     _("ADSB Traffic Details"), target_id.Format(tmp_id));
  dialog.SetCaption(tmp);

  // Fill the callsign field (+ registration)
  // note: don't use target->Name here since it is not updated
  //       yet if it was changed
#if 0
  const TCHAR* cs = FlarmDetails::LookupCallsign(target_id);
  if (cs != nullptr && cs[0] != 0)
    {
    try
      {
      BasicStringBuilder<TCHAR> builder(tmp, ARRAY_SIZE(tmp));
      builder.Append(cs);
      if (record)
        builder.Append(_T(" ("), record->registration.c_str(), _T(")"));
      value = tmp;
      }
    catch (BasicStringBuilder<TCHAR>::Overflow) 
      {
      value = cs;
      }
    }
  else
#endif
    value = _T("--");
  SetText(AdsbTrafficDetailsWidget::CALLSIGN, value);

  // Update the frequently changing fields too
  UpdateChanging(CommonInterface::Basic());
  }

/**
 * This event handler is called when the "Team" button is pressed
 */
inline void
AdsbTrafficDetailsWidget::OnTeamClicked()
  {
  }

/**
 * This event handler is called when the "Change Callsign" button is pressed
 */
inline void
AdsbTrafficDetailsWidget::OnCallsignClicked()
  {
  }

void
AdsbTrafficDetailsWidget::OnFriendColorClicked(TargetColor)
  {
  }

/**
 * The function opens the FLARM Traffic Details dialog
 */
void
dlgAdsbTrafficDetailsShowModal(TargetId id)
  {
  const DialogLook &look = UIGlobals::GetDialogLook();

  WidgetDialog dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
                      look, _("ADSB Traffic Details"));

  AdsbTrafficDetailsWidget* widget = new AdsbTrafficDetailsWidget(dialog, id);
  widget->CreateButtons(dialog);
  dialog.AddButton(_("Close"), mrCancel);
  dialog.FinishPreliminary(widget);
  dialog.ShowModal();
  }
