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

#include "../Timer.hpp"
#include "../Globals.hpp"
#include "../Queue.hpp"

namespace UI {

Timer::Timer(Callback _callback) noexcept
  :timer_event(event_queue->GetEventLoop(), BIND_THIS_METHOD(OnTimer)),
   callback(std::move(_callback)) {}

void
Timer::Schedule(std::chrono::steady_clock::duration d) noexcept
{
  timer_event.Schedule(d);
}

void
Timer::SchedulePreserve(std::chrono::steady_clock::duration d) noexcept
{
  if (!IsPending())
    Schedule(d);
}

void
Timer::Cancel()
{
  timer_event.Cancel();
}

void
Timer::OnTimer() noexcept
{
  event_queue->Interrupt();

  callback();
}

} // namespace UI
