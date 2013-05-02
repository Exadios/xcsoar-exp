# Copyright_License {
#
#  XCSoar Glide Computer - http://www.xcsoar.org/
#  Copyright (C) 2000-2012 The XCSoar Project
#  A detailed list of copyright holders can be found in the file "AUTHORS".
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#}

import os

topdir = './'
topsrcdir = topdir + 'src/'

env     = Environment(ENV = os.environ)
verbose = ARGUMENTS.get('verbose', 'n')
debug   = ARGUMENTS.get('debug', 'y')
targets = ARGUMENTS.get('targets', ['UNIX32'])

targetmap = { 'UNIX32'      : 'Unix',
              'ANDROID'     : 'Android',
              'ANDROID7'    : 'Android',
              'ANDROID86'   : 'Android',
              'ANDROIDMIPS' : 'Android' }

conf = Configure(env)
current_ndk = 'android-ndk-r8d'
ndk_bin = 'toolchains/arm-linux-androideabi-4.7/prebuilt/linux-x86/bin/'
env['ENABLE_SDL'] = 'n'
env['OPENGL'] = 'y'
env['HAVE_PCM_PLAYER'] = 'y'
env['HAVE_HTTP'] = 'y'
env['IOIOLIB_DIR'] = '~/Software/Libraries/IOIO/V3.23/IOIOLib'
env = conf.Finish()

if verbose == 'n':
    env['CXXCOMSTR']  = 'Compiling $TARGET'
    env['CCCOMSTR']   = 'Compiling $TARGET'
    env['LINKCOMSTR'] = 'Linking $TARGET'
    env['ARCOMSTR']   = 'Archiving $TARGET'
    env['CPPCOMSTR']  = 'Processing $TARGET'
    env['PERLCOMSTR'] = 'Generating $TARGET'

githash = os.popen('git rev-parse --short --verify HEAD 2>/dev/null').read()
version = os.popen('cat ' + topdir + 'VERSION.txt').read()
env['CPPFLAGS'] = '-DHAVE_POSIX ' + '-DHAVE_STDINT_H ' + '-DHAVE_UNISTD_H ' + '-DHAVE_VASPRINTF'
env.Append(CXXFLAGS = '-I' + topsrcdir + 'unix/ ' +
                      '-I' + topdir + 'output/include ' +
                      '-I' + topsrcdir + ' ' +
                      '-I' + topsrcdir + 'Engine/ ' +
                      '-I./src/Terrain/jasper/.. ' +
                      '-I/usr/local/SDL_ttf/include/SDL ' +
                      '-I/usr/include/SDL ' +
                      '-m32 -Wuninitialized -g -ffast-math -ftree-vectorize -fno-exceptions -fno-rtti -std=gnu++0x -fno-threadsafe-statics -fmerge-all-constants -fconserve-space -fno-operator-names -fvisibility=hidden -ffunction-sections -finput-charset=utf-8 -Wall -Wextra -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare -Wundef -Wmissing-declarations -Wredundant-decls -Wmissing-noreturn -Wno-unused-parameter -Wno-missing-field-initializers  -Wcast-align ' +
                      '-DHAVE_POSIX ' +
                      '-DHAVE_STDINT_H -DHAVE_UNISTD_H -DHAVE_VASPRINTF ' +
                      '-DRADIANS -DEYE_CANDY  -DHAVE_BOOST -DXCSOAR ' +
                      '-D_GNU_SOURCE=1 -D_REENTRANT ' +
                      '-DENABLE_SDL -DZLIB_CONST -DENABLE_OPENGL -DGL_GLEXT_PROTOTYPES ' +
                      '-DXCSOAR_VERSION="\\"' + version + '\\"" ' +
                      '-DGIT_COMMIT_ID="\\"' + githash + '\\""')
env.Append(CFLAGS = env['CXXFLAGS'])

env.Append(LINKFLAGS = '-m32')

# Make common includes.
env.Command('output/include/MathTables.h',
            'output/host/tools/GenerateSineTables',
            'output/host/tools/GenerateSineTables > $TARGET')
env.Command('output/include/InputEvents_Text2Event.cpp',
            topsrcdir + 'Input/InputEvents.hpp',
            'perl ' + topdir + '/tools/Text2Event.pl $SOURCE >$TARGET')
env.Command('output/include/InputEvents_Text2GCE.cpp',
            topsrcdir + 'Input/InputQueue.hpp',
            'perl ' + topdir + 'tools/Text2GCE.pl $SOURCE > $TARGET')
env.Command('output/include/InputEvents_Text2NE.cpp',
            topsrcdir + 'Input/InputQueue.hpp',
            'perl ' + topdir + 'tools/Text2NE.pl $SOURCE > $TARGET')
env.Command('output/include/InputEvents_default.cpp',
            topdir + 'Data/Input/default.xci',
            'perl ' + topdir + 'tools/xci2cpp.pl $SOURCE > $TARGET')
env.Command('output/include/Status_defaults.cpp',
            topdir + 'Data/Status/default.xcs',
            'perl ' + topdir + 'tools/xcs2cpp.pl $SOURCE > $TARGET')

dbg = env.Clone()
std = env.Clone()
dbg['DEBUG'] = 'y'
std['DEBUG'] = 'n'
dbg.Append(CXXFLAGS = ' -O0 -funit-at-a-time -Werror')
std.Append(CXXFLAGS = ' -Os -DNDEBUG -ftree-vectorize -funsafe-loop-optimizations -ffunction-sections')
dbg.unix = dbg.Clone(TARGET_OS = 'Unix', PLATFORM = 'Posix', SHELL = 'bash')
dbg.unix['ENABLE_SDL'] = 'y'
dbg.unix.Append(CXXFLAGS = '-DENABLE_SDL')
dbg.android = dbg.Clone(TARGET_OS = 'Android',
                        PLATFORM = 'Posix',
                        SHELL = 'bash',
                        CXX = '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++',
                        CC =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-gcc',
                        LD =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++')
dbg.android7 = dbg.Clone(TARGET_OS = 'Android',
                         PLATFORM = 'Posix',
                         SHELL = 'bash',
                         CXX = '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++',
                         CC =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-gcc',
                         LD =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++')
dbg.android86 = dbg.Clone(TARGET_OS = 'Android',
                          PLATFORM = 'Posix',
                          SHELL = 'bash',
                          CXX = '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++',
                          CC =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-gcc',
                          LD =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++')
dbg.androidmips = dbg.Clone(TARGET_OS = 'Android',
                            PLATFORM = 'Posix',
                            SHELL = 'bash',
                            CXX = '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++',
                            CC =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-gcc',
                            LD =  '~/opt/' + current_ndk + '/' + ndk_bin + 'arm-linux-androideabi-g++')

map = { 'y' : { 'Unix' : dbg.unix, 'Android' : dbg.android } }

Export('verbose', 'debug', 'targets', 'topdir', 'topsrcdir', 'env')

if debug == 'y':
  for target in targets:
    e = map[debug][targetmap[target]]
    print "Debug target " + target
    outbase = 'output/' + target + '/dbg/'
    e.Append(CXXFLAGS = '-I' + topdir + 'output/' + target + '/include')
    Export('e', 'outbase', 'target')
    SConscript('./SConscript', variant_dir=outbase, duplicate=0)
else:
  for target in targets:
    e = std
    print "Release target " + target
    outbase = 'output/' + target + '/std/'
    e.Append(CXXFLAGS = '-I' + topdir + 'output/' + target + '/include')
    Export('e', 'outbase', 'target')
    SConscript('./SConscript', variant_dir=outbase, duplicate=0)

