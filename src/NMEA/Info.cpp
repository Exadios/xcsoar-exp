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

#include "NMEA/Info.hpp"
#include "Atmosphere/AirDensity.hpp"
#include "time/Cast.hxx"

//------------------------------------------------------------------------------
void
NMEAInfo::UpdateClock()
  {
  clock = TimeStamp{std::chrono::steady_clock::now().time_since_epoch()};
  }

//------------------------------------------------------------------------------
BrokenDateTime
NMEAInfo::GetDateTimeAt(TimeStamp other_time) const noexcept
  {
  if (!other_time.IsDefined())
    return BrokenDateTime::Invalid();

  if (!this->time_available || !this->date_time_utc.IsDatePlausible())
    return BrokenDateTime(BrokenDate::Invalid(),
                          BrokenTime::FromSinceMidnightChecked(other_time.ToDuration()));

  return this->date_time_utc + std::chrono::duration_cast<std::chrono::system_clock::duration>(FloatDuration{other_time - this->time});
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideTime(TimeStamp _time) noexcept
  {
  assert(_time.IsDefined());

  this->time = _time;
  this->time_available.Update(clock);

  (BrokenTime &)this->date_time_utc = BrokenTime::FromSinceMidnightChecked(this->time.ToDuration());
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideDate(const BrokenDate &date)
  {
  assert(date.IsPlausible());

  (BrokenDate &)this->date_time_utc = date;
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideTrueAirspeedWithAltitude(double tas, double altitude)
  {
  this->true_airspeed = tas;
  this->indicated_airspeed = true_airspeed / AirDensityRatio(altitude);
  this->airspeed_available.Update(clock);
  this->airspeed_real = true;
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideIndicatedAirspeedWithAltitude(double ias, double altitude)
  {
  this->indicated_airspeed = ias;
  this->true_airspeed = indicated_airspeed * AirDensityRatio(altitude);
  this->airspeed_available.Update(clock);
  this->airspeed_real = true;
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideTrueAirspeed(double tas)
  {
  auto any_altitude = GetAnyAltitude();

  if (any_altitude)
    this->ProvideTrueAirspeedWithAltitude(tas, *any_altitude);
  else
    /* no altitude; dirty fallback */
    this->ProvideBothAirspeeds(tas, tas);
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ProvideIndicatedAirspeed(double ias)
  {
  auto any_altitude = GetAnyAltitude();

  if (any_altitude)
    this->ProvideIndicatedAirspeedWithAltitude(ias, *any_altitude);
  else
    /* no altitude; dirty fallback */
    this->ProvideBothAirspeeds(ias, ias);
  }

//------------------------------------------------------------------------------
void
NMEAInfo::Reset()
  {
  this->UpdateClock();

  this->alive.Clear();

  this->gps.Reset();
  this->acceleration.Reset();
  this->attitude.Reset();

  this->location_available.Clear();

  this->track = Angle::Zero();
  this->track_available.Clear();

  this->variation_available.Clear();

  this->ground_speed_available.Clear();
  this->airspeed_available.Clear();
  this->ground_speed = true_airspeed = indicated_airspeed = 0;
  this->airspeed_real = false;

  this->gps_altitude_available.Clear();

  this->static_pressure_available.Clear();
  this->dyn_pressure_available.Clear();
  this->pitot_pressure_available.Clear();
  this->sensor_calibration_available.Clear();

  this->baro_altitude_available.Clear();
  this->baro_altitude = 0;

  this->pressure_altitude_available.Clear();
  this->pressure_altitude = 0;

  this->time_available.Clear();
  this->time = {};

  this->date_time_utc = BrokenDateTime::Invalid();

  this->noncomp_vario_available.Clear();
  this->total_energy_vario_available.Clear();
  this->netto_vario_available.Clear();

  this->settings.Clear();

  this->external_wind_available.Clear();

  this->temperature_available = false;
  this->humidity_available = false;

  this->heart_rate_available.Clear();

  this->engine_noise_level_available.Clear();

  this->voltage_available.Clear();
  this->battery_level_available.Clear();

  this->switch_state.Reset();

  this->stall_ratio_available.Clear();

  // XXX StallRatio

  this->device.Clear();
  this->secondary_device.Clear();
  this->target_data.Clear();

#ifdef ANDROID
  this->glink_data.Clear();
#endif
  }

//------------------------------------------------------------------------------
void
NMEAInfo::ExpireWallClock()
  {
  if (!this->alive)
    return;

  this->UpdateClock();

#if defined(ANDROID) || defined(__APPLE__)
  if (this->gps.nonexpiring_internal_gps)
    /* the internal GPS does not expire */
    return;
#endif

  this->alive.Expire(clock, std::chrono::seconds(10));
  if (!this->alive)
    {
    this->time_available.Clear();
    this->gps.Reset();
    this->target_data.Clear();

#ifdef ANDROID
    this->glink_data.Clear();
#endif
    }
  else 
    {
    this->time_available.Expire(clock, std::chrono::seconds(10));
    }
  }

//------------------------------------------------------------------------------
void
NMEAInfo::Expire()
  {
  if (this->location_available.Expire(clock, std::chrono::seconds(10)))
    /* if the location expires, then GPSState should expire as well,
       because all GPSState does is provide metadata for the GPS
       fix */
    this->gps.Reset();
  else
    this->gps.Expire(clock);

  this->track_available.Expire(clock, std::chrono::seconds(10));
  this->ground_speed_available.Expire(clock, std::chrono::seconds(10));

  if (this->airspeed_available.Expire(clock, std::chrono::seconds(30)))
    this->airspeed_real = false;

  this->gps_altitude_available.Expire(clock, std::chrono::seconds(30));
  this->static_pressure_available.Expire(clock, std::chrono::seconds(30));
  this->dyn_pressure_available.Expire(clock, std::chrono::seconds(30));
  this->pitot_pressure_available.Expire(clock, std::chrono::seconds(30));
  this->sensor_calibration_available.Expire(clock, std::chrono::hours(1));
  this->baro_altitude_available.Expire(clock, std::chrono::seconds(30));
  this->pressure_altitude_available.Expire(clock, std::chrono::seconds(30));
  this->noncomp_vario_available.Expire(clock, std::chrono::seconds(5));
  this->total_energy_vario_available.Expire(clock, std::chrono::seconds(5));
  this->netto_vario_available.Expire(clock, std::chrono::seconds(5));
  this->settings.Expire(clock);
  this->external_wind_available.Expire(clock, std::chrono::minutes(10));
  this->heart_rate_available.Expire(clock, std::chrono::seconds(10));
  this->engine_noise_level_available.Expire(clock, std::chrono::seconds(30));
  this->voltage_available.Expire(clock, std::chrono::minutes(5));
  this->battery_level_available.Expire(clock, std::chrono::minutes(5));
  this->target_data.Expire(clock);
#ifdef ANDROID
  this->glink_data.Expire(clock);
#endif
  this->attitude.Expire(clock);
  }

//------------------------------------------------------------------------------
void
NMEAInfo::Complement(const NMEAInfo &add)
  {
  if (!add.alive)
    /* if there is no heartbeat on the other object, there cannot be
       useful information */
    return;

  if (!this->alive)
    {
    this->gps = add.gps;
    }

  this->alive.Complement(add.alive);

  if (this->time_available.Complement(add.time_available))
    {
    this->time = add.time;
    this->date_time_utc = add.date_time_utc;
    }

  this->acceleration.Complement(add.acceleration);
  this->attitude.Complement(add.attitude);

  if (this->location_available.Complement(add.location_available))
    {
    this->location = add.location;

    /* the GPSState belongs to the device that provides the GPS fix */
    this->gps = add.gps;
    }

  if (this->track_available.Complement(add.track_available))
    this->track = add.track;

  if (this->ground_speed_available.Complement(add.ground_speed_available))
    this->ground_speed = add.ground_speed;

  if ((add.airspeed_real || !this->airspeed_real) &&
      this->airspeed_available.Complement(add.airspeed_available))
    {
    this->true_airspeed = add.true_airspeed;
    this->indicated_airspeed = add.indicated_airspeed;
    this->airspeed_real = add.airspeed_real;
    }

  if (this->gps_altitude_available.Complement(add.gps_altitude_available))
    this->gps_altitude = add.gps_altitude;

  if (this->static_pressure_available.Complement(add.static_pressure_available))
    this->static_pressure = add.static_pressure;

  if (this->dyn_pressure_available.Complement(add.dyn_pressure_available))
    this->dyn_pressure = add.dyn_pressure;

  if (this->pitot_pressure_available.Complement(add.pitot_pressure_available))
    this->pitot_pressure = add.pitot_pressure;

  if (this->sensor_calibration_available.Complement(add.sensor_calibration_available))
    {
    this->sensor_calibration_factor = add.sensor_calibration_factor;
    this->sensor_calibration_offset = add.sensor_calibration_offset;
    }

  if (this->baro_altitude_available.Complement(add.baro_altitude_available))
    this->baro_altitude = add.baro_altitude;

  if (this->pressure_altitude_available.Complement(add.pressure_altitude_available))
    this->pressure_altitude = add.pressure_altitude;

  if (this->noncomp_vario_available.Complement(add.noncomp_vario_available))
    this->noncomp_vario = add.noncomp_vario;

  if (this->total_energy_vario_available.Complement(add.total_energy_vario_available))
    this->total_energy_vario = add.total_energy_vario;

  if (this->netto_vario_available.Complement(add.netto_vario_available))
    this->netto_vario = add.netto_vario;

  this->settings.Complement(add.settings);

  if (this->external_wind_available.Complement(add.external_wind_available))
    this->external_wind = add.external_wind;

  if (!this->temperature_available && add.temperature_available)
    {
    this->temperature = add.temperature;
    this->temperature_available = add.temperature_available;
    }

   if (!this->variation_available && add.variation_available)
     {
     this->variation = add.variation;
     this->variation_available = add.variation_available;
     }

  if (!this->humidity_available && add.humidity_available)
    {
    this->humidity = add.humidity;
    this->humidity_available = add.humidity_available;
    }

  if (this->heart_rate_available.Complement(add.heart_rate_available))
    this->heart_rate = add.heart_rate;

  if (this->engine_noise_level_available.Complement(add.engine_noise_level_available))
    this->engine_noise_level = add.engine_noise_level;

  if (this->voltage_available.Complement(add.voltage_available))
    this->voltage = add.voltage;

  if (this->battery_level_available.Complement(add.battery_level_available))
    this->battery_level = add.battery_level;

  this->switch_state.Complement(add.switch_state);

  if (!this->stall_ratio_available && add.stall_ratio_available)
    this->stall_ratio = add.stall_ratio;

  this->target_data.Complement(add.target_data);

#ifdef ANDROID
  this->glink_data.Complement(add.glink_data);
#endif
  }

//------------------------------------------------------------------------------
void
NMEAInfo::Copy(const NMEAInfo& rhs)
  {
  this->clock                        = rhs.clock;
  this->alive                        = rhs.alive;
  this->gps                          = rhs.gps;
  this->acceleration                 = rhs.acceleration;
  this->attitude                     = rhs.attitude;
  this->location_available           = rhs.location_available;
  this->location                     = rhs.location;
  this->track_available              = rhs.track_available;
  this->track                        = rhs.track;
  this->airspeed_real                = rhs.airspeed_real;
  this->ground_speed_available       = rhs.ground_speed_available;
  this->airspeed_available           = rhs.airspeed_available;
  this->ground_speed                 = rhs.ground_speed;
  this->true_airspeed                = rhs.true_airspeed;
  this->indicated_airspeed           = rhs.indicated_airspeed;
  this->gps_altitude_available       = rhs.gps_altitude_available;
  this->gps_altitude                 = rhs.gps_altitude;
  this->static_pressure              = rhs.static_pressure;
  this->static_pressure_available    = rhs.static_pressure_available;
  this->pitot_pressure               = rhs.pitot_pressure;
  this->pitot_pressure_available     = rhs.pitot_pressure_available;
  this->dyn_pressure                 = rhs.dyn_pressure;
  this->dyn_pressure_available       = rhs.dyn_pressure_available;
  this->sensor_calibration_offset    = rhs.sensor_calibration_offset;
  this->sensor_calibration_factor    = rhs.sensor_calibration_factor;
  this->sensor_calibration_available = rhs.sensor_calibration_available;
  this->baro_altitude_available      = rhs.baro_altitude_available;
  this->baro_altitude                = rhs.baro_altitude;
  this->pressure_altitude            = rhs.pressure_altitude;
  this->pressure_altitude_available  = rhs.pressure_altitude_available;
  this->baro_altitude_weak           = rhs.baro_altitude_weak;
  this->pressure_altitude_weak       = rhs.pressure_altitude_weak;
  this->time_available               = rhs.time_available;
  this->time                         = rhs.time;
  this->date_time_utc                = rhs.date_time_utc;
  this->noncomp_vario_available      = rhs.noncomp_vario_available;
  this->total_energy_vario_available = rhs.total_energy_vario_available;
  this->netto_vario_available        = rhs.netto_vario_available;
  this->noncomp_vario                = rhs.noncomp_vario;
  this->total_energy_vario           = rhs.total_energy_vario;
  this->netto_vario                  = rhs.netto_vario;
  this->settings                     = rhs.settings;
  this->external_wind_available      = rhs.external_wind_available;
  this->external_wind                = rhs.external_wind;
  this->temperature_available        = rhs.temperature_available;
  this->temperature                  = rhs.temperature;
  this->variation_available          = rhs.variation_available;
  this->variation                    = rhs.variation;
  this->humidity_available           = rhs.humidity_available;
  this->humidity                     = rhs.humidity;
  this->heart_rate_available         = rhs.heart_rate_available;
  this->heart_rate                   = rhs.heart_rate;
  this->engine_noise_level_available = rhs.engine_noise_level_available;
  this->engine_noise_level           = rhs.engine_noise_level;
  this->voltage_available            = rhs.voltage_available;
  this->voltage                      = rhs.voltage;
  this->battery_level_available      = rhs.battery_level_available;
  this->battery_level                = rhs.battery_level;
  this->switch_state                 = rhs.switch_state;
  this->stall_ratio                  = rhs.stall_ratio;
  this->stall_ratio_available        = rhs.stall_ratio_available;
  this->device                       = rhs.device;
  this->secondary_device             = rhs.secondary_device;
  this->target_data                  = rhs.target_data;
  this->count                        = rhs.count;
  }
