project('XCSoar', [ 'cpp', 'c' ], 
        meson_version  : '>=0.56.2',
        license        : 'GPL2+',
        version        : ('v7.27.0.3.1'),
        default_options: [
                          'buildtype=debugoptimized',
                          'b_lundef=false',
                          'c_std=gnu11',
#                          'cpp_std=c++17',
                          'warning_level=0',
                          'default_library=static',
                          'werror=false',
                          'wno-error=non-virtual-dtor',
                          'optimization=g',
                         ])

vs = find_program('tools/Meson/scripts/version')
rv = run_command(vs)
message(rv.stdout().strip())
message(rv.stderr().strip())
version_define = ''.join(['-DXCSOAR_VERSION="', rv.stdout().strip(), '"'])
message(version_define)
# \todo Figure out a GIT_COMMIT that works. See build/version.mk

add_project_arguments([
                       '-funit-at-a-time',
                       '-ffast-math',
                       '-std=c++20',
                       '-fno-threadsafe-statics',
                       '-fmerge-all-constants',
                       '-fcoroutines',
                       '-fconserve-space',
                       '-fno-operator-names',
                       '-fvisibility=hidden',
                       '-finput-charset=utf-8',
                       '-Wall',
                       '-Wextra',
                       '-Wwrite-strings',
                       '-Wcast-qual',
                       '-Wpointer-arith',
                       '-Wsign-compare',
                       '-Wundef',
                #       '-Wmissing-declarations',
                       '-Wredundant-decls',
                       '-Wmissing-noreturn',
                       '-Wvla',
                       '-Wno-format-truncation',
                       '-Wno-missing-field-initializers',
                       '-Wcast-align',
                       '-Werror',
                       '-Wno-error=non-virtual-dtor',
                       '-Wno-non-virtual-dtor', # Meson insists on having -Wnon-virtual-dtor
                       '-Wno-undef',  # This should not be. It is necessary to fix a problem in Boost.
                #       '-DBOOST_NO_IOSTREAM',
                #       '-DBOOST_MATH_NO_LEXICAL_CAST',
                #       '-DBOOST_UBLAS_NO_STD_CERR',
                #       '-DBOOST_ERROR_CODE_HEADER_ONLY',
                #       '-DBOOST_SYSTEM_NO_DEPRECATED',
                #       '-DBOOST_NO_STD_LOCALE',
                #       '-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE',
                #       '-DBOOST_NO_CXX98_FUNCTION_BASE',
                 #      '-DBOOST_JSON_STANDALONE',
                       '-DHAVE_POSIX',
                       '-DHAVE_VASPRINTF',
                       '-DEYE_CANDY',
                       '-D_GLIBCXX_ASSERTIONS',
                       '-DENABLE_ALSA',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES ',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-isystem /usr/include/x86_6-linux-gnu',
                       '-DUSE_POLL_EVENT',
                 #      '-DBOOST_JSON_STANDALONE',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11 ',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_ALSA',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES ',
                       '-DUSE_GLX',
                       '-DUSE_X11 ',
                       '-DUSE_POLL_EVEN',
                       '-isystem lib/glm',
                       '-isystem /usr/include/x86_6-linux-gnu',
                       '-isystem /usr/include/lua5.4',
                       '-isystem /usr/include/freetype2/',
                       version_define,
                      ],
                      language : 'cpp')
add_project_arguments([
                       '-Og',
                       '-funit-at-a-time',
                       '-ffast-math',
                       '-g',
                       '-std=gnu11',
                       '-fmerge-all-constants',
                       '-fconserve-space',
                       '-fvisibility=hidden',
                       '-finput-charset=utf-8',
                       '-Wall',
                       '-Wextra',
                       '-Wwrite-strings',
                       '-Wcast-qual',
                       '-Wpointer-arith',
                       '-Wsign-compare',
                       '-Wundef',
                       '-Wmissing-declarations',
                       '-Wredundant-decls',
                       '-Wmissing-noreturn',
                       '-Wvla',
                       '-Wno-format-truncation',
                       '-Wno-missing-field-initializers',
                       '-Wcast-align',
                       '-Werror',
                       '-Wno-error=non-virtual-dtor',
                #       '-DBOOST_NO_IOSTREAM',
                #       '-DBOOST_MATH_NO_LEXICAL_CAST',
                #       '-DBOOST_UBLAS_NO_STD_CERR',
                #       '-DBOOST_ERROR_CODE_HEADER_ONLY',
                #       '-DBOOST_SYSTEM_NO_DEPRECATED',
                #       '-DBOOST_NO_STD_LOCALE',
                #       '-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE',
                #       '-DBOOST_NO_CXX98_FUNCTION_BASE',
                #       '-DBOOST_JSON_STANDALONE',
                       '-DHAVE_POSIX',
                       '-DHAVE_VASPRINTF',
                       '-DEYE_CANDY',
                       '-D_GLIBCXX_ASSERTIONS',
                       '-DENABLE_ALSA',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES ',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-isystem /usr/include/x86_6-linux-gnu',
                       '-DUSE_POLL_EVENT',
                  #     '-DBOOST_JSON_STANDALONE',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-isystem lib/glm',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11 ',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_POLL_EVENT',
                       '-DUSE_FREETYPE',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES',
                       '-DUSE_GLX',
                       '-DUSE_X11',
                       '-DUSE_POLL_EVENT',
                       '-DENABLE_ALSA',
                       '-DENABLE_OPENGL',
                       '-DHAVE_GLES',
                       '-DHAVE_GLES2',
                       '-DGL_GLEXT_PROTOTYPES ',
                       '-DUSE_GLX',
                       '-DUSE_X11 ',
                       '-DUSE_POLL_EVEN',
                       '-DZZIP_HAVE_STDINT_H',
                       '-DHAVE_POSIX',
                       '-DHAVE_VASPRINTF',
                       '-DZLIB_CONST',
                       '-isystem lib/glm',
                       '-isystem /usr/include/x86_6-linux-gnu',
                       '-isystem /usr/include/lua5.4',
                       '-isystem /usr/include/freetype2/',
                       version_define,
                      ],
                      language : 'c')

common_includes = include_directories([
                                       './src/unix',
                                       './output/include',
                                       './src',
                                       './src/Engine',
                                       './output/UNIX/include',
                                       './src/Terrain/jasper/..',
                                       './src/Profile',
                                       './src/time',
                                       './src/util',
                                       './src/co',
                                       './lib/glm',
                                       './output/src/boost_1_80_0',
                                       '/usr/include/lua5.4',  # Sigh, why is this necessary?
                                       '/usr/include/freetype2',  # And this.
                                      ])

# Use old make system to generate some required targets (because its too 
# difficult to do otherwise)
make = find_program('make', required : true)
custom_target('egm96s',
              command : [ make,
                          'output/data/egm96s.dem.c',
                          'output/data/COPYING.gz',
                          'output/data/AUTHORS.gz',
                          'output/data/NEWS.txt.gz',
                        ],
              output  : [ 'egm96s.dem.c',
                          'COPYING.gz.c',
                          'AUTHORS.gz.c',
                          'NEWS.txt.gz.c',
                        ]
             )

egm96s_lib = library('egm96s', [
                                 'output/data/egm96s.dem.c',
                                 'output/data/COPYING.gz.c',
                                 'output/data/AUTHORS.gz.c',
                                 'output/data/NEWS.txt.gz.c',
                               ]
                    )
custom_target('resourses',
              command : [ make,
                          'output/UNIX/resources.c',
                        ],
              output : 'resources.c'
             )

resources_lib = library('resources', 'output/UNIX/resources.c')

#subdir('Data')

subdir('src')
subdir('src/Engine/Task')
subdir('src/Geo')
subdir('src/lua')
subdir('src/Device/Driver/Vega')
subdir('src/Device/Driver/BlueFly')
subdir('src/Device/Driver/Volkslogger')
subdir('src/Device/Driver/Flytec')
subdir('src/Device/Driver/IMI')
subdir('src/Device/Driver/CAI302')
subdir('src/Device/Driver/LX')
subdir('src/Device/Driver/FLARM')
subdir('src/Device/Driver/ThermalExpress')
subdir('src/Device/Driver/XCTracer')
subdir('src/Device/Driver/SkyEcho-2')
subdir('src/Device/Driver')
subdir('src/NMEA')
subdir('src/thread')
subdir('src/Look')
subdir('src/MapWindow')
subdir('src/Profile')
subdir('src/Engine/Airspace')
subdir('src/Engine/GlideSolvers')
subdir('src/Engine/Contest')
subdir('src/Engine/Route')
subdir('src/Form')
subdir('src/InfoBoxes')
subdir('src/net/client')
subdir('src/system')
subdir('src/Operation')
subdir('src/Form/DataField')
subdir('src/Terrain')
subdir('src/net/http')
subdir('src/Widget')
subdir('src/Topography')
subdir('src/ui/window/custom')
subdir('src/ui/canvas/custom')
subdir('src/time')
subdir('src/co')
subdir('src/Computer')
subdir('src/ui/event')
subdir('src/Screen')
subdir('src/event')
subdir('src/Math')
subdir('src/util')
subdir('src/Cloud')
subdir('src/Device/Port')
subdir('src/Topography/shapelib')
subdir('src/Audio')
subdir('src/io')
subdir('src/net')
subdir('src/Engine/Waypoint')
subdir('src/zzip')
subdir('src/json')
subdir('src/Terrain/jasper')

cxx = meson.get_compiler('cpp')

xs = subproject('xcsoar_sources')
xcsoar_sources = xs.get_variable('xcsoar_sources')

lua5_4          = dependency('lua5.4-c++')
glu1_mesa_dev   = dependency('glu')
curl_dev        = dependency('libcurl')
freetype_dev    = dependency('freetype2')
cares_dev       = dependency('libcares')
png16_dev       = dependency('libpng16')
libjpeg_dev     = dependency('libjpeg')
libasound2_dev  = dependency('alsa')
libjpeg_dev     = dependency('libjpeg')
libsodium_dev   = dependency('libsodium')
libtiff_dev     = dependency('libtiff-4')
libssl_dev      = dependency('openssl')
libpthread      = cxx.find_library('libpthread')
libm            = cxx.find_library('libm', required : true)
libz            = cxx.find_library('libz', required : true)
libgeotiff_dev  = cxx.find_library('geotiff', required : true)
x11             = cxx.find_library('X11', required : true)
glx             = cxx.find_library('GLX', required : true)

xcsoar = executable('xcsoar', xcsoar_sources,
                    link_with : [ task_lib,
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
                                  screencustom_lib,
                                  screencustomimg_lib,
                                  time_lib,
                                  flarm_lib,
                                  co_lib,
                                  thermalexpress_lib,
                                  computer_lib,
                                  event_lib,
                                  screen_lib,
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
                                ],
                    include_directories : common_includes,
                    dependencies: [ lua5_4,
                                    glu1_mesa_dev,
                                    curl_dev, 
                                    freetype_dev,
                                    cares_dev,
                                    png16_dev,
                                    libjpeg_dev,
                                    libasound2_dev,
                                    libsodium_dev,
                                    libtiff_dev,
                                    libgeotiff_dev,
                                    libssl_dev,
                                    libjpeg_dev,
                                    libm,
                                    libz,
                                    x11,
                                    glx,
                                    libpthread,
                                  ],
                    )

