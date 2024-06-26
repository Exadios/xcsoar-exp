/*
  Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
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

package au.org.narroginglidingclub.xcsoar;

import ioio.lib.api.IOIO;
import ioio.lib.api.exception.ConnectionLostException;

/*
 * A driver for voltage measurement on the IOIO board.
 */
final class GlueVoltage implements AndroidSensor, IOIOConnectionListener {
  private IOIOConnectionHolder holder;
  private final SensorListener listener;
  private final int sample_rate;
  private Voltage instance;
  private int state = STATE_LIMBO;

  GlueVoltage(IOIOConnectionHolder _holder, int _sample_rate,
              SensorListener _listener) {
    listener = _listener;
    sample_rate = _sample_rate;
    holder = _holder;
    _holder.addListener(this);
  }

  @Override
  public void close() {
    IOIOConnectionHolder holder;
    synchronized(this) {
      holder = this.holder;
      this.holder = null;
    }

    if (holder != null)
      holder.removeListener(this);
  }

  @Override
  public int getState() {
    return state;
  }

  @Override public void onIOIOConnect(IOIO ioio)
    throws ConnectionLostException, InterruptedException {
    try {
      instance = new Voltage(ioio, sample_rate, listener);
      state = STATE_READY;
      listener.onSensorStateChanged();
    } catch (Exception e) {
      state = STATE_FAILED;
      listener.onSensorError(e.getMessage());
    }
  }

  @Override public void onIOIODisconnect(IOIO ioio) {
    if (instance == null)
      return;

    instance.close();
    instance = null;
    state = STATE_LIMBO;
    listener.onSensorStateChanged();
  }
}
