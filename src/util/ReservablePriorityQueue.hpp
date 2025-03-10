/* Copyright_License {

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

/*
  This code courtesy of unknown author at
  http://stackoverflow.com/questions/3666387/c-priority-queue-underlying-vector-container-capacity-resize
*/

#pragma once

#include <queue>

template<class T, class Container, class Compare>
class reservable_priority_queue:
  public std::priority_queue<T, Container, Compare>
{
public:
  typedef typename std::priority_queue<T, Container, Compare>::size_type size_type;
  reservable_priority_queue(size_type capacity = 0) {
    reserve(capacity);
  }

  void reserve(size_type capacity) {
    this->c.reserve(capacity);
  }

  size_type capacity() const {
    return this->c.capacity();
  }

  void clear() {
    this->c.clear();
  }
};
