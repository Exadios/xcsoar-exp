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

#include "StatusPanel.hpp"
#include "Blackboard/RateLimitedBlackboardListener.hpp"

class SystemStatusPanel final
  : public StatusPanel,
    private NullBlackboardListener {
  RateLimitedBlackboardListener rate_limiter;

public:
  explicit SystemStatusPanel(const DialogLook &look) noexcept
    :StatusPanel(look), rate_limiter(*this, std::chrono::seconds(2),
                                     std::chrono::milliseconds(500)) {}

  /* virtual methods from class StatusPanel */
  void Refresh() noexcept override;

  /* virtual methods from class Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  void Show(const PixelRect &rc) noexcept override;
  void Hide() noexcept override;

private:
  /* virtual methods from class BlackboardListener */
  void OnGPSUpdate(const MoreData &basic) override;
};
