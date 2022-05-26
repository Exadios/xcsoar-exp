/* Copyright_License {

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

//use std::io;
use std::net::UdpSocket;
use std::time::Duration;

/// A command line tool to dump SkyEcho 2 data output to terminal or file.
///

//------------------------------------------------------------------------------
fn main() -> std::io::Result<()>
  {
    {
    let socket = UdpSocket::bind("127.0.0.1:4000")?;
    socket.set_read_timeout(Some(Duration::new(20, 0))).unwrap();

    loop
      {
      // Receives a single datagram message on the socket. If `buf` is too
      // small to hold the message, it will be cut off.
      let mut buf = [0; 435];
      let n = socket.recv_from(&mut buf)?;
      let t = buf[0];
      match (t, n)
        {
        (0, (7, _)) => println!("Heartbeat with correct length"),
        (0, ..) => println!("Heartbeat with incorrect length"),
        (7, (436, _)) => println!("Uplink message with correct length"),
        (7, ..) => println!("Uplink message with incorrect length"),
        (10, (28, _)) => println!("Ownship with correct length"),
        (10, ..) => println!("Ownship with incorrect length"),
        (20, (28, _)) => println!("Traffic with correct length"),
        (20, ..) => println!("Traffic with incorrect length"),
        (30, (22, _)) => println!("Basic with correct length"),
        (30, ..) => println!("Basic with incorrect length"),
        (11, (5, _)) => println!("Altitude with correct length"),
        (11, ..) => println!("Altitude with incorrect length"),
        _ => println!("Unknown type"),
        }

      break;
      }
    } // the socket is closed here

  Ok(())
  }
