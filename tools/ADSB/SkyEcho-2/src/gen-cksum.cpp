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

#include <iostream>

static uint16_t crc16Table[256]; // Table for FCS.

//------------------------------------------------------------------------------
void
Initfcs()
  {
  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  uint16_t i, bitchr, crc;

  for (i = 0; i < 256; i++)
    {
    crc = (i << 8);
    for (bitchr = 0; bitchr < 8; bitchr++)
      {
      crc = (crc << 1) ^ ((crc & 0x8000) ? 0x1021 : 0);
      }
    crc16Table[i] = crc;
    }
  }

//------------------------------------------------------------------------------
uint16_t
Generatefcs(const unsigned char* packet,
            unsigned long length)
  {
  /* This code comes from GDL 90 specification document 
   * Garmin 560-1058-00 Rev A. */
  uint32_t i;
  uint16_t crc = 0;

  /** The data to be check summed start just after the initial flag
   * and terminates just before the packet check sum.
   */
  for (i = 1; i < length; i++)
    {
    crc = crc16Table[crc >> 8] ^ (crc << 8) ^ packet[i];
    }
  return crc;
  }

//------------------------------------------------------------------------------
int
main(int argc, const char* argv[])
  {
  Initfcs();
  char buf[256];

  std::cin.read(buf, sizeof(buf) / sizeof (unsigned char));

  uint16_t fcs = Generatefcs((unsigned char *)&buf[0],
                             (unsigned long)std::cin.gcount());

  std::cout << std::hex << fcs << std::dec << std::endl;

  return 0;
  }
