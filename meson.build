project('XCSoar', [ 'c', 'cpp' ], 
        meson_version  : '>=1.0.1',
        license        : 'GPL2+',
        version        : ('v7.27.0.3.2'),
        default_options: [
                          'buildtype=release',
                          'debug=false',
                          'b_lundef=false',
                          'c_std=gnu11',
                          'cpp_std=c++20',
                          'warning_level=2',
                          'default_library=static',
                          'werror=true',
#                          'wno-error=non-virtual-dtor',
                          'optimization=0',
                          'strip=true',
                         ])

vs = find_program('tools/Meson/scripts/version')
rv = run_command(vs, check : true)
message(rv.stdout().strip())
message(rv.stderr().strip())
version_define = ''.join(['-DXCSOAR_VERSION="', rv.stdout().strip(), '"'])
message(version_define)
# \todo Figure out a GIT_COMMIT that works. See build/version.mk

c_compiler = meson.get_compiler('c')
cpp_compiler = meson.get_compiler('cpp')

add_project_arguments([
                       '-funit-at-a-time',
                       '-ffast-math',
                       '-std=c++20',  # default_options above appear not to set
                                      # this. I don't know why.
                       '-fno-threadsafe-statics',
                       '-fmerge-all-constants',
                       '-fno-operator-names',
                       '-fvisibility=hidden',
                       '-finput-charset=utf-8',
                       '-Wwrite-strings',
                       '-Wcast-qual',
                       '-Wpointer-arith',
                       '-Wsign-compare',
                #       '-Wmissing-declarations',
                       '-Wredundant-decls',
                       '-Wvla',
                       '-Wno-missing-field-initializers',
                       '-Wcast-align',
                       '-Wno-error=non-virtual-dtor',
                       '-Wno-unused-parameter',
                       '-Wno-non-virtual-dtor', # Meson insists on having -Wnon-virtual-dtor
                       '-Wno-undef',  # This should not be. It is necessary to fix a problem in Boost.
#                       '-Wno-unused-parameter', # More Boost problems.
                       '-DBOOST_NO_STDLIB_CONFIG',  # Stop Boost headers doing
                                                    # crazy stuff with our
                                                    # defines given here.
                       '-DBOOST_FT_AUTODETECT_CALLING_CONVENTIONS',
                       '-DBOOST_NO_IOSTREAM',
                       '-DBOOST_MATH_NO_LEXICAL_CAST',
                       '-DBOOST_UBLAS_NO_STD_CERR',
                       '-DBOOST_ERROR_CODE_HEADER_ONLY',
                       '-DBOOST_SYSTEM_NO_DEPRECATED',
                       '-DBOOST_NO_STD_LOCALE',
                       '-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE',
                       '-DBOOST_NO_CXX98_FUNCTION_BASE',
                       '-DBOOST_JSON_STANDALONE',
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
                       '-isystem output/src/boost_1_80_0',
                       '-g3',
                       version_define,
                      ],
                      language : 'cpp',
                     )
#add_project_arguments(cpp_args,
#                      language : 'cpp',
#                     )
if cpp_compiler.get_id() == 'gcc'
add_project_arguments([
                       '-fconserve-space',
                       '-Wno-format-truncation',
                       '-fcoroutines',
                       '-Wmissing-noreturn',  # For boost library
                      ],
                      language : 'cpp',)
endif

add_project_arguments([
                       '-funit-at-a-time',
                       '-ffast-math',
                       '-std=gnu11',  # default_options above appear not to set
                                      # this. I don't know why.
                       '-fmerge-all-constants',
                       '-fvisibility=hidden',
                       '-finput-charset=utf-8',
                       '-Wwrite-strings',
                       '-Wcast-qual',
                       '-Wpointer-arith',
                       '-Wsign-compare',
                       '-Wundef',
                       '-Wmissing-declarations',
                       '-Wredundant-decls',
                       '-Wvla',
                       '-Wno-missing-field-initializers',
                       '-Wcast-align',
                       '-Wno-error=non-virtual-dtor',
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
if c_compiler.get_id() == 'gcc'
add_project_arguments([
                       '-fconserve-space',
                       '-Wno-format-truncation',
                      ],
                      language : 'c',)
endif
#generated_headers = include_directories('include')
common_includes = include_directories([
                                       './',  # To include generated headers
                                       './src/unix',
                                       './src',
                                       './src/Engine',
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
make    = find_program('make', required : true)
perl    = find_program('perl', required : true)
host_cc = find_program('cc',   required : true)
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

# Some build tools
LinkResources = find_program('tools/LinkResources-meson.pl')
xcsoar_rc_helper = find_program('tools/Meson/build-helpers/xcsoar_rc-helper')
resource_h_helper = find_program('tools/Meson/build-helpers/resource_h-helper')
text2_helper = find_program('tools/Meson/build-helpers/text2-helper')
text1_helper = find_program('tools/Meson/build-helpers/text1-helper')

debug('*** Host Machine ***')
debug('cpu : ' + host_machine.cpu())
debug('cpu_family : ' + host_machine.cpu_family())
debug('endian : ' + host_machine.endian())
#debug('kernel : ' + host_machine.kernel())
#debug('subsystem : ' + host_machine.subsystem())
debug('system : ' + host_machine.system())

#------------------------------------------------------------------------------#
# Some custom targets

xs = subproject('xcsoar_sources')
gst_sources = xs.get_variable('gst_sources')
xcsoar_sources = xs.get_variable('xcsoar_sources')
xcsoar_cpp_args = xs.get_variable('xcsoar_cpp_args')

generatesinetables = executable('generatesinetables', gst_sources,
                                include_directories : common_includes,
                                build_by_default    : true,
                                native              : true,
                               )

resource_h = custom_target('resource_h', 
                           command : [ resource_h_helper,
                                       '@INPUT@',
                                       '@OUTPUT@',
                                     ],
                           input   : meson.project_source_root() +
                                     '/src/Resources.hpp',
                           output  : 'resource.h',
                          )

xcsoar_rc = custom_target('xcsoar_rc',
                          command : [ xcsoar_rc_helper,
                                      '@INPUT0@',
                                      '@OUTPUT@',
                                      '@INPUT1@',
                                      '-DHAVE_POSIX',
                                      '-DHAVE_VASPRINTF',
                                      '-DEYE_CANDY',
                                      '-D_GLIBCXX_ASSERTIONS',
                                      '-DENABLE_OPENGL',
                                      '-DHAVE_GLES',
                                      '-DHAVE_GLES2',
                                      '-isystem lib/glm',
                                      '-DGL_GLEXT_PROTOTYPES',
                                    ],
                          input   : [ meson.project_source_root() +
                                      '/Data/XCSoar.rc',
                                      meson.current_build_dir(),
                                    ],
                          depends  : resource_h,
                          output   : 'XCSoar.rc'
                         )

inputevents_text2event = custom_target('InputEvents_Text2Event',
                                       command : [ text2_helper,
                                                   'Text2Event.pl',
                                                   '@INPUT@',
                                                   '@OUTPUT@',
                                                 ],
                                        input  : './src/Input/InputEvents.hpp',
                                        output : 'InputEvents_Text2Event.cpp',
                                        build_by_default : true,
                                       )

inputevents_text2gce = custom_target('InputEvents_Text2GCE',
                                     command : [ text2_helper,
                                                 'Text2GCE.pl',
                                                 '@INPUT@',
                                                 '@OUTPUT@',
                                               ],
                                      input  : './src/Input/InputQueue.hpp',
                                      output : 'InputEvents_Text2GCE.cpp',
                                      build_by_default : true,
                                     )

inputevents_text2ne = custom_target('InputEvents_Text2NE',
                                    command : [ text2_helper,
                                                'Text2NE.pl',
                                                '@INPUT@',
                                                '@OUTPUT@',
                                              ],
                                     input  : './src/Input/InputQueue.hpp',
                                     output : 'InputEvents_Text2NE.cpp',
                                     build_by_default : true,
                                    )

inputevents_char2gce = custom_target('InputEvents_Char2GCE',
                                     command : [ text2_helper,
                                                 'Char2GCE.pl',
                                                 '@INPUT@',
                                                 '@OUTPUT@',
                                               ],
                                      input  : './src/Input/InputQueue.hpp',
                                      output : 'InputEvents_Char2GCE.cpp',
                                      build_by_default : true,
                                     )

inputevents_char2ne = custom_target('InputEvents_Char2NE',
                                    command : [ text2_helper,
                                                'Char2NE.pl',
                                                '@INPUT@',
                                                '@OUTPUT@',
                                              ],
                                     input  : './src/Input/InputQueue.hpp',
                                     output : 'InputEvents_Char2NE.cpp',
                                     build_by_default : true,
                                    )

inputevents_default = custom_target('InputEvents_default',
                                    command : [ text2_helper,
                                                'xci2cpp.pl',
                                                '@INPUT@',
                                                '@OUTPUT@',
                                              ],
                                     input  : './Data/Input/default.xci',
                                     output : 'InputEvents_default.cpp',
                                     build_by_default : true,
                                    )

status_defaults = custom_target('Status_defaults',
                                command : [ text2_helper,
                                            'xcs2cpp.pl',
                                            '@INPUT@',
                                            '@OUTPUT@',
                                          ],
                                input   : './Data/Status/default.xcs',
                                output  : 'Status_defaults.cpp',
                                build_by_default : true,
                               )

resource_data_h = custom_target('resource_data',
                                command : [ text2_helper,
                                            'GenerateResources-meson.pl',
                                            '@INPUT@',
                                            '@OUTPUT@',
                                          ],
                                input   : xcsoar_rc,
                                output  : 'resource_data.h'
                               )

mathtables_h = custom_target('MathTables',
                             command : [ text1_helper,
                                         generatesinetables.full_path(),
                                         '@OUTPUT@',
                                       ],
                             output  : 'MathTables.h',
                             depends : generatesinetables,
                             build_by_default : true,
                            )

resources_c = custom_target('linkresources',
                            command : [ LinkResources, '@INPUT@', '@OUTPUT@' ],
                            input   : xcsoar_rc,
                            env     : [
                                        'PERL5LIB=../../tools',
                                        'PERLLIB=../../tools'
                                      ],
                            output  : 'resources.c',
                            depends : resource_h,
                           )

resources_lib = library('resources', resources_c)
#------------------------------------------------------------------------------#

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
subdir('src/Surveillance')

cxx = meson.get_compiler('cpp')

lua5_4          = dependency('lua5.4-c++',
                             disabler : true,
                             native   : false,
                             method   : 'pkg-config',
                            )
#lua5_4          = cxx.find_library('lua5.4-c++',
#                                   required : true,
#                                   disabler : true,
#                                  )
#glu1_mesa_dev   = dependency('glu')
glu1_mesa_dev   = cxx.find_library('GLU',
                                   required : true,
                                   disabler : true,
                                  )
curl_dev        = dependency('libcurl')
#curl_dev        = cxx.find_library('libcurl',
#                                   required : true,
#                                   disabler : true,
#                                  )
freetype_dev    = dependency('freetype2',
                             disabler : true,
                             method   : 'pkg-config',
                             native   : false,
                            )
#freetype_dev    = cxx.find_library('freetype2',
#                                   required : true,
#                                   disabler : true,
#                                   dirs     : '/usr/lib/arm-linux-gnueabihf/',
#                                  )
cares_dev       = dependency('libcares')
#cares_dev       = cxx.find_library('libcares', required : true)
png16_dev       = dependency('libpng16')
#png16_dev       = cxx.find_library('libpng16', required : true)
libjpeg_dev     = dependency('libjpeg')
#libjpeg_dev     = cxx.find_library('libjpeg', required : true)
libasound2_dev  = dependency('alsa')
#libasound2_dev  = cxx.find_library('libasound2', required : true)
libjpeg_dev     = dependency('libjpeg')
#libjpeg_dev     = cxx.find_library('libjpeg', required : true)
libsodium_dev   = dependency('libsodium')
#libsodium_dev   = cxx.find_library('libsodium', required : true)
libtiff_dev     = dependency('libtiff-4')
#libtiff_dev     = cxx.find_library('libtiff-4', required : true)
libssl_dev      = dependency('openssl',
                             disabler : true
                            )
#libssl_dev      = cxx.find_library('openssl', required : true)
libpthread      = cxx.find_library('libpthread')
libm            = cxx.find_library('libm', required : true)
libz            = cxx.find_library('libz', required : true)
libgeotiff_dev  = cxx.find_library('geotiff', required : true)
x11             = cxx.find_library('X11', required : true)
glx             = cxx.find_library('GLX', required : true)
gl              = cxx.find_library('GL', required : true)
glu             = cxx.find_library('GLU', required : true)

xcsoar_lib = library('xcsoar', xcsoar_sources,
                     include_directories : common_includes,
                    )
xcsoar = executable('xcsoar', 'src/XCSoar.cpp',
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
                                  surveillance_lib,
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
                                    gl,
                                    glu,
                                    libpthread,
                                  ],
                      cpp_args : xcsoar_cpp_args,
                    )

# Tools
if meson.is_cross_build() != true
  test_includes = include_directories(['src', 'src/unix'])
  subdir('test/src')
  subdir('tools/ADSB/NMEA')
  subdir('tools/ADSB/GDL90')

# Unit tests
  if get_option('ut_build').enabled()
    subdir('src/Gauge/UT')
  endif
endif

