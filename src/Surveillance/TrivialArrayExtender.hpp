/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2024 The XCSoar Project
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

/**
 * \file
 * \addtogroup Surveillance
 * \{
 */

#include "util/TrivialArray.hxx"
//#include "Interface.hpp"

#ifndef NDEBUG
#include "thread/Thread.hpp"
//namespace CommonInterface
//  {
//  extern ThreadRegister thread_register;
//  };

#include "LogFile.hpp"
#include <thread>
#include <iostream>
#endif

/**
 * This is a decorator for the \ref TrivialArray.
 *
 * It adds copy operators to the extended class.
 */
template<class T, std::size_t max>
class TrivialArrayExtender : public TrivialArray<T, max>
  {
  using Array = std::array<T, max>;

public:
  /**
   * Ctor. This ctor plainly does not initialize the object in any way.
   * However, the extended class, \ref TrivialArray, requires initialization
   * before use. If not initialized the object will usually fail
   * catastrophically! Initialization is done by calling this object's Clear
   * method.
   */
  TrivialArrayExtender() = default;

  using size_type = typename Array::size_type;
  using const_reference =  typename Array::const_reference;

  /**
   *
   */
  constexpr TrivialArrayExtender(size_t size, const_reference value)
    {
    this->the_size = size;
    std::fill(TrivialArray<T, max>::begin(),
              TrivialArray<T, max>::end(),
              value);
    }

//  /**
//   * Copy operator for \ref TrivialArray class.
//   * @param rhs The source \ref TrivialArray object.
//   * @return This object.
//   */
//  TrivialArrayExtender<T, max>& operator=(const TrivialArrayExtender<T, max>& rhs)
//    {
//    this->the_size = rhs.the_size;
//#ifndef NDEBUG
//    std::cout << __FILE__ << ", " << __LINE__ << ": "
//              << this->the_size << ", " << rhs.the_size << "\n";
//#endif
//    this->array    = rhs.array;
//#ifndef NDEBUG
//    std::cout << __FILE__ << ", " << __LINE__ << ": "
//              << this->the_size << ", " << rhs.the_size << "\n";
//#endif
//    return *this;
//    }

  /**
   * Quickly remove the indexed item by copying the last item over it. If
   * the index is the last item then just drop the indexed item.
   * \note This member function does not override 
   * \ref TrivialArray::quick_remove.
   * @param i The index to the item to remove.
   */
   constexpr void quick_remove(size_type i) noexcept
     {
     assert(i < this->size());
     TrivialArray<T, max>::quick_remove(i);
     }

  };
/**
 * \}
 */
