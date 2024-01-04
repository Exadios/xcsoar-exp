/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2023 The XCSoar Project
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
 * It adds copy operators to the decorated class.
 */
template<class T, std::size_t max>
class TrivialArrayDecorator : public TrivialArray<T, max>
  {
  using Array = std::array<T, max>;

public:
  /**
   * Ctor. This ctor plainly does not initialize the object in any way.
   * However, the decorated class, \ref TrivialArray, requires initialization
   * before use. If not initialized the object will usually fail
   * catastrophically! Initialization is done by calling this object's Clear
   * method.
   */
  TrivialArrayDecorator() = default;

  using size_type = typename Array::size_type;
  using const_reference =  typename Array::const_reference;

  /**
   *
   */
  constexpr TrivialArrayDecorator(size_t size, const_reference value)
    {
    this->the_size = size;
    std::fill(TrivialArray<T, max>::begin(),
              TrivialArray<T, max>::end(),
              value);
    }

  /**
   * Copy operator for \ref TrivialArray class.
   * @param rhs The source \ref TrivialArray object.
   * @return This object.
   */
  TrivialArrayDecorator<T, max>& operator=(const TrivialArrayDecorator<T, max>& rhs)
    {
#ifndef NDEBUG
//    LogFormat("%s, %d: %s", __FILE__, __LINE__,
//              rhs.array.empty() ? "true" : "false");
//    LogFormat("%s, %d: %lu",  __FILE__, __LINE__, rhs.the_size);
#endif
    this->the_size = rhs.the_size;
    this->array    = rhs.array;
#ifndef NDEBUG
//    LogFormat("%s, %d: %s", __FILE__, __LINE__,
//              rhs.array.empty() ? "true" : "false");
#if 0
    LogFormat("%s, %d:  %lu, %s, %d",  __FILE__, __LINE__,
              this->the_size,
              (this->empty()) ? "true" : "false",
              std::this_thread::get_id()
              );
#else
//    std::cout << __FILE__ << ", " << __LINE__ << ": "
//              << this->the_size << ", "
//              << CommonInterface::thread_register.ThreadName(pthread_self())
//              << '\n';
#endif
#endif
    return *this;
    }
  };

/**
 * \file
 */
