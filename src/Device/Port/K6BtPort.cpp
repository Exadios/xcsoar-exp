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

#include "K6BtPort.hpp"

#include <stdio.h>
#include <string.h>

K6BtPort::K6BtPort(std::unique_ptr<Port> _port, unsigned _baud_rate,
                   PortListener *_listener, DataHandler &_handler) noexcept
  :Port(_listener, _handler), port(std::move(_port)), baud_rate(_baud_rate)
{
}

bool
K6BtPort::SendCommand(uint8_t cmd)
{
  const uint8_t data[2] = { ESCAPE, cmd };
  return port->Write(data, sizeof(data)) == sizeof(data);
}

PortState
K6BtPort::GetState() const noexcept
{
  return port->GetState();
}

bool
K6BtPort::WaitConnected(OperationEnvironment &env)
{
  if (!port->WaitConnected(env))
    return false;

  // TODO: wrap the PortHandler, move initialisation to PortStateChanged()

  /* ensure that the K6Bt is not in command mode */
  SendCommand(NOP);

  /* now that the Bluetooth connection has been established, tell our
     preferred baud rate to the K6Bt */
  SendSetBaudrate(baud_rate);

  return true;
}

std::size_t
K6BtPort::Write(const void *_data, std::size_t length)
{
  /* in order to forward the buffer verbatim to the real device, we
     have to escape all ESCAPE bytes (i.e. send each of them twice) */

  const uint8_t *data = (const uint8_t *)_data;

  std::size_t total = 0;

  const uint8_t *p;
  while ((p = (const uint8_t *)memchr(data, ESCAPE, length)) != nullptr) {
    std::size_t chunk = p - data + 1;
    std::size_t nbytes = port->Write(data, chunk);
    total += nbytes;
    if (nbytes != chunk)
      return total;

    /* write the ESCAPE byte again (but don't consider it in the
       return value) */
    port->Write(p, 1);

    ++p;

    length = data + length - p;
    data = p;
  }

  if (length > 0)
    total += port->Write(data, length);

  return total;
}

bool
K6BtPort::Drain()
{
  return port->Drain();
}

void
K6BtPort::Flush()
{
  port->Flush();

  SendCommand(FLUSH_BUFFERS | 0x3); /* flush RX and TX buffer */
}

static constexpr int
BaudRateToK6Bt(unsigned baud_rate) noexcept
{
  switch (baud_rate) {
  case 2400:
    return 0x0;

  case 4800:
    return 0x1;

  case 9600:
    return 0x2;

  case 19200:
    return 0x3;

  case 38400:
    return 0x4;

  case 57600:
    return 0x5;

  case 115200:
    return 0x6;

  default:
    return -1;
  }
}

void
K6BtPort::SendSetBaudrate(unsigned _baud_rate)
{
  int code = BaudRateToK6Bt(_baud_rate);
  if (code < 0)
    throw std::runtime_error("Baud rate not supported by K6Bt");

  if (!SendCommand(CHANGE_BAUD_RATE | code))
    throw std::runtime_error("Failed to send CHANGE_BAUD_RATE to K6Bt");
}

void
K6BtPort::SetBaudrate(unsigned _baud_rate)
{
  if (_baud_rate == baud_rate)
    return;

  SendSetBaudrate(_baud_rate);
  baud_rate = _baud_rate;
}

unsigned
K6BtPort::GetBaudrate() const noexcept
{
  return baud_rate;
}

bool
K6BtPort::StopRxThread()
{
  return port->StopRxThread();
}

bool
K6BtPort::StartRxThread(void)
{
  return port->StartRxThread();
}

std::size_t
K6BtPort::Read(void *Buffer, std::size_t Size)
{
  return port->Read(Buffer, Size);
}

void
K6BtPort::WaitRead(std::chrono::steady_clock::duration timeout)
{
  port->WaitRead(timeout);
}
