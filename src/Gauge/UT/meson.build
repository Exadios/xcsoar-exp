cxx = meson.get_compiler('cpp')
libpthread      = cxx.find_library('libpthread')

gauge_test_harness = executable('gauge_test_harness',
                                   [
                                   'FlarmTrafficWindow.cpp',
                                   'Window.cpp',
                                   'Font.cpp',
                                   'Canvas.cpp',
                                   'Display.cpp',
                                   'Brush.cpp',
                                   'Debug.cpp',
                                   ],
                                include_directories : common_includes,
                                link_with : [ xcsoar_lib,
                                  task_lib,
                                  geo_lib,
                                  lua_lib,
                                  vega_lib,
                                  bluefly_lib,
                                  volkslogger_lib,
                                  thread_lib,
                                  look_lib,
                                  mapwindow_lib,
                                  profile_lib,
                                  airspace_lib,
                                  form_lib,
                                  flytec_lib,
                                  imi_lib,
                                  infobox_lib,
                                  client_lib,
                                  cai302_lib,
                                  os_lib,
                                  glide_lib,
                                  operation_lib,
                                  driver_lib,
                                  datafield_lib,
                                  terrain_lib,
                                  http_lib,
                                  lx_lib,
                                  contest_lib,
                                  widget_lib,
                                  topo_lib,
                                  route_lib,
#                                  screencustom_lib,
                                  screencustomimg_lib,
                                  time_lib,
                                  flarm_lib,
                                  co_lib,
                                  thermalexpress_lib,
                                  computer_lib,
                                  event_lib,
#                                  screen_lib,
                                  async_lib,
                                  math_lib,
                                  adsb_lib,
                                  xctracer_lib,
                                  util_lib,
                                  cloudtokml_lib,
                                  port_lib,
                                  shapelib_lib,
                                  audio_lib,
                                  io_lib,
                                  net_lib,
                                  waypoint_lib,
                                  nmea_lib,
                                  zzip_lib,
                                  json_lib,
                                  egm96s_lib,
                                  jasper_lib,
                                  resources_lib,
                                  resource_lib,
                                  surveillance_lib, 
                                            ],
                                dependencies : [ libpthread,
                                                 freetype_dev,
                                                 x11,
                                                 glx,
                                                 gl,
                                                 glu,
                                               ]
                                
                               )
test('gauge_test 1', gauge_test_harness, args : '1', ) 

