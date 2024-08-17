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

/**
 * \cond NEVER
 */
#include "Surveillance/TargetList.hpp"
#include "Surveillance/TargetData.hpp"
#include "Surveillance/TrivialArrayExtender.hpp"
#include "Surveillance/Flarm/FlarmTarget.hpp"
#include "Surveillance/Adsb/AdsbTarget.hpp"

#include <stdlib.h>
#include <iostream>
#include <array>

//------------------------------------------------------------------------------
int main(const int argc, const char* const argv[])
  {
  if (argc != 2)
    return -1;

  int test = atoi(argv[1]);
  std::cout << "Test " << test << std::endl;
  if (test == 0)
    return -1;

  switch (test)
    {

    // Basic TargetList test.
    case 1:
      {
      TargetList traffic;
      traffic.Clear();
      if ((traffic.IsEmpty() == true) && (traffic.GetActiveTrafficCount() == 0))
        return 0;
      else
        return 1;
      break;
      }

    case 2:
      {
      TargetList traffic;
      int i;

      for (i = 0; (size_t)i < TargetList::MAX_COUNT + 1; i++)
        {
        if (i % 2 == 0)
          {
          FlarmTarget tg;
          if (!(tg.CollisionAvoidance() == true) ||
              !(tg.TrafficAwareness() == false))
            {
            std::cout << "CollisionAvoidance() or TrafficAwareness() failure with FlarmTarget"
                      << std::endl;
            return 1;
            }
          if (traffic.AddFlarmTarget(tg) == nullptr)
            {
            if ((size_t)i < TargetList::MAX_COUNT)
              {
              std::cout << "TargetList capacity problem" << std::endl;
              return 1;
              }
            }
          }
        else
          {
          AdsbTarget tg;
          if (!(tg.CollisionAvoidance() == false) ||
              !(tg.TrafficAwareness() == true))
            {
            std::cout << "CollisionAvoidance() or TrafficAwareness() failure with AdsbTarget"
                      << std::endl;
            return 1;
            }
          if (traffic.AddAdsbTarget(tg) == nullptr)
            {
            if ((size_t)i < TargetList::MAX_COUNT)
              {
              std::cout << "TargetList capacity problem" << std::endl;
              return 1;
              }
            }
          }
        }
      return 0;
      break;
      }

    // TargetList polymorphism test.
    case 3:
      {

      break;
      }

    // FlarmListDecorator and AdsbListDecorator AllocateTraffic()
    case 4:
      {

      break;
      }

    // FlarmListDecorator and AdsbListDecorator AddTarget()
    case 5:
      {

      break;
      }

    case 6: // Test TargetId
      {
        {
        TargetId id;  // Default
        id.Clear();

        std::cout << "  IsDefined (should be false) = " 
                  << std::boolalpha 
                  << id.IsDefined()
                  << std::noboolalpha
                  << "\n";
        }
        {
        TargetId id((long int)1024);
 
        std::cout << "  IsDefined (should be true) = " 
                  << std::boolalpha 
                  << id.IsDefined()
                  << std::noboolalpha
                  << "\n";
        std::cout << "  Id = 0x" 
                  << std::hex
                  << id.Arg()
                  << std::dec
                  << "\n";

        }
        {
        TargetId id1;
        id1.Arg(1024);
        TargetId id2;
        id2.Arg(id1.Arg());

        std::cout << "  Id = 0x"
                  << std::hex
                  << id2.Arg()
                  << std::dec
                  << "\n";
        }
      std::cout << std::endl;
      break;
      }

    case 7:
      {
    
      break;
      }

    case 8:
      {
      /* 
      * Try finding a target as a RemoteTarget and then cast it to a
      * FlarmTarget.
      */

      break;
      }

    case 9: // TrivialArrayDecorator
      {
        {
        TrivialArrayExtender<size_t, 10> arr1, arr2;
        arr1.clear(); // This is necessary!
        arr2.clear(); // This is necessary!
        if ((arr1.empty() == false) | (arr2.empty() == false))
          {
          std::cout << "Status empty() fail" << '\n';
          return 1;
          }

        for (size_t i = 0; i < arr1.max_size(); i++)
          {
          arr1.append() = i;
          }

        arr2 = arr1;

        if (arr1.size() == arr2.size())
          for (size_t i = 0; i < arr2.size() ; i++)
            {
            size_t& e = arr2[i];
            if (e != i)
              {
              std::cout << "Elements not equal" << '\n';
              return 1;
              }
            }
        else
          {
          std::cout << "Size not equal" << '\n';
          return 1;
          }
        }

        {
        TrivialArrayExtender<size_t, 10> arr1(10, 0);
        TrivialArrayExtender<size_t, 10> arr2;
        arr2.clear(); // This is necessary!
        if ((arr1.empty() == true)  |
            (arr2.empty() == false) |
            (arr1.size()  != 10)
           )
          {
          std::cout << "Status empty() fail" << '\n';
          return 1;
          }

        for (size_t i = 0; i < arr1.max_size(); i++)
          {
          arr1[i] = i;
          }

        arr2 = arr1;

        if (arr1.size() == arr2.size())
          for (size_t i = 0; i < arr2.size(); i++)
            {
            size_t& e = arr2[i];
            if (e != i)
              {
              std::cout << "Elements not equal" << '\n';
              return 1;
              }
            }
            
        else
          {
          std::cout << "Size not equal" << '\n';
          return 1;
          }
        }
      
      break;
      }

    // FlarmListDecorator Complement and cast.
    case 10:
      {

      break;
      }

    // FlarmTargets and AdsbTargets
    case 11:
      {

      break;
      }

    default:
      std::cerr << "Invalid test option" << std::endl;
      return -1;
    }
  return 0;
  }

/**
 * \endcond
 */
