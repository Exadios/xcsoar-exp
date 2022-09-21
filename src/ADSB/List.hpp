/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#ifndef XCSOAR_ADSB_TRAFFIC_LIST_HPP
#define XCSOAR_ADSB_TRAFFIC_LIST_HPP

#include "Traffic.hpp"
#include "NMEA/Validity.hpp"
#include "util/TrivialArray.hxx"

#include <type_traits>

/**
 * @file
 * This class keeps track of the traffic objects received from an
 * ADSB.
 */
struct AdsbTrafficList
  {
  static constexpr size_t MAX_COUNT = 50;

  /**
   * Time stamp of the latest modification to this object.
   */
  Validity modified;

  /**
   * When was the last new traffic received?
   */
  Validity new_traffic;

  /** Adsb traffic information */
  TrivialArray<AdsbTraffic, MAX_COUNT> list;

  void Clear()
    {
    this->modified.Clear();
    this->new_traffic.Clear();
    this->list.clear();
    }

  /**
   * Test whether this list is empty.
   * @return If empty then true.
   */
  bool IsEmpty() const
    {
    return this->list.empty();
    }

  /**
   * Adds data from the specified object, unless already present in
   * this one.
   * @param add Source list.
   */
  void Complement(const AdsbTrafficList &add)
    {
    // Add unique traffic from 'add' list
    for (auto &traffic : add.list)
      {
      if (this->FindTraffic(traffic.id) == nullptr)
        {
        AdsbTraffic* new_traffic = AllocateTraffic();
        if (new_traffic == nullptr)
          return;
        *new_traffic = traffic;
        }
      }
    }

  /**
   * Delete stale targets.
   * @param clock Time now.
   */
  void Expire(TimeStamp clock) noexcept
    {
    this->modified.Expire(clock, std::chrono::minutes(5));
    this->new_traffic.Expire(clock, std::chrono::minutes(1));

    for (unsigned i = this->list.size(); i-- > 0;)
      if (!this->list[i].Refresh(clock))
        this->list.quick_remove(i);
    }

  /**
   * Give the number of items in this list.
   * @return Number of targets in the list.
   */
  unsigned GetActiveTrafficCount() const
    {
    return this->list.size();
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id ADSB id
   * @return the ADSB_TRAFFIC pointer, NULL if not found
   */
  AdsbTraffic *FindTraffic(unsigned int id)
    // \todo pfb: Using unsigned int for convenience but should we be
    //       using "AdsbId"?
    {
    for (auto &traffic : this->list)
      if (traffic.id == id)
        return &traffic;

    return NULL;
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param id ADSB id
   * @return the ADSB_TRAFFIC pointer, NULL if not found
   */
  const AdsbTraffic *FindTraffic(unsigned int id) const
    {
    for (const auto &traffic : this->list)
      if (traffic.id == id)
        return &traffic;

    return NULL;
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name the name or call sign
   * @return the AdsbTraffic pointer, NULL if not found
   */
  AdsbTraffic *FindTraffic(const TCHAR *name)
    {
    for (auto &traffic : this->list)
      if (traffic.name.equals(name))
        return &traffic;

    return NULL;
    }

  /**
   * Looks up an item in the traffic list.
   *
   * @param name the name or call sign
   * @return the ADSB_TRAFFIC pointer, NULL if not found
   */
  const AdsbTraffic *FindTraffic(const TCHAR *name) const
    {
    for (const auto &traffic : this->list)
      if (traffic.name.equals(name))
        return &traffic;

    return NULL;
    }

  /**
   * Allocates a new ADSB_TRAFFIC object from the array.
   *
   * @return the ADSB_TRAFFIC pointer, NULL if the array is full
   */
  AdsbTraffic *AllocateTraffic()
    {
    return this->list.full()
           ? NULL
           : &this->list.append();
    }

  /**
   * Search for the previous traffic in the ordered list.
   * @param t 
   */
  const AdsbTraffic *PreviousTraffic(const AdsbTraffic *t) const
    {
    return t > this->list.begin()
           ? t - 1
           : NULL;
    }

  /**
   * Search for the next traffic in the ordered list.
   */
  const AdsbTraffic *NextTraffic(const AdsbTraffic *t) const
    {
    return t + 1 < this->list.end()
           ? t + 1
           : NULL;
    }

  /**
   * Search for the first traffic in the ordered list.
   */
  const AdsbTraffic *FirstTraffic() const
    {
    return this->list.empty() ? NULL : this->list.begin();
    }

  /**
   * Search for the last traffic in the ordered list.
   */
  const AdsbTraffic *LastTraffic() const
    {
    return this->list.empty() ? NULL : this->list.end() - 1;
    }

  /**
   * Finds the most critical alert.  Returns NULL if there is no
   * alert.
   */
  const AdsbTraffic *FindMaximumAlert() const;

  unsigned TrafficIndex(const AdsbTraffic *t) const
    {
    return t - this->list.begin();
    }
  };

static_assert(std::is_trivial<AdsbTrafficList>::value, "type is not trivial");

#endif  // XCSOAR_ADSB_TRAFFIC_LIST_HPP
