package com.exadios.test;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.BitmapFactory.Options;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Surface;
import android.view.View;
import android.view.WindowManager;
import android.location.GpsSatellite;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.view.View;

public class GPSNoiseProfilerActivity extends Activity
  {
	private AquireView mAquireView;
	private SensorManager mSensorManager;
	private WindowManager mWindowManager;
	private Display mDisplay;
  private WakeLock mWakeLock;

  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState)
    {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);

		// Get an instance of the SensorManager
    mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

    // Get an instance of the WindowManager
    mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
    mDisplay = mWindowManager.getDefaultDisplay();
    }

	@Override
  protected void onResume()
	  {
    super.onResume();
    /*
     * when the activity is resumed, we acquire a wake-lock so that the
     * screen stays on, since the user will likely not be fiddling with the
     * screen or buttons.
     */
    mWakeLock.acquire();

    // Start the simulation
    mAquireView.startAquisition();
    }

  class AquireView extends View implements SensorEventListener
    {
		/** the index of this device in the global list */
    private int index;
		/** the name of the currently selected location provider */
    String locationProvider = LocationManager.GPS_PROVIDER;

		private LocationManager locationManager;
    private SensorManager sensorManager;
    private WindowManager windowManager;
    private Sensor accelerometer;
    private double acceleration;
		private boolean queriedLocationSettings = false;


		AquireView(Context context, int index)
		  {
      this.index = index;
			locationManager = (LocationManager)context.getSystemService(Context.LOCATION_SERVICE);
      if (!locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER) &&
          !queriedLocationSettings)
			  {
        // Let user turn on GPS, we are not allowed to.
        Intent myIntent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
        context.startActivity(myIntent);
        queriedLocationSettings = true;
        }
			windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
			sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
      accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
      acceleration = 1.0;
			update();
			}

    void startAquisition()
		  {
			}

		private void update()
		  {
			}
		}
  }
