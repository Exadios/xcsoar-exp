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
verbose = ARGUMENTS.get('VERBOSE', 0)
debug   = ARGUMENTS.get('DEBUG', 1)
targets = ARGUMENTS.get('TARGETS', ['UNIX32'])
enable_sdl = 1
if int(verbose) == 0:
    env['CXXCOMSTR']  = 'Compiling $TARGET'
    env['LINKCOMSTR'] = 'Linking $TARGET'
    env['ARCOMSTR']   = 'Archiving $TARGET'

env.Append(CXXFLAGS = '-I' + topsrcdir + ' ' +
                      '-I' + topsrcdir + 'unix/ ' +
                      '-I' + topsrcdir + 'Engine/ ' +
                      '-I./src/Terrain/jasper/.. ' +
                      '-I/home/pfb/Software/Libraries/SDL_ttf/SDL_ttf-2.0.11/local/include/SDL ' +
                      '-I/usr/include/SDL ' +
                      '-fno-exceptions ' +
                      '-fno-rtti ' +
                      '-std=gnu++0x ' +
                      '-fno-threadsafe-statics ' +
                      '-fmerge-all-constants ' +
                      '-funit-at-a-time ' +
                      '-m32 ' +
                      '-Os ' +
                      '-Wuninitialized ' +
                      '-ffast-math -ftree-vectorize ' +
                      '-funsafe-loop-optimizations  -fno-exceptions ' +
                      '-fno-rtti -std=gnu++0x -fno-threadsafe-statics ' +
                      '-fmerge-all-constants -fconserve-space ' +
                      '-fno-operator-names -fvisibility=hidden ' +
                      '-ffunction-sections -finput-charset=utf-8 -Wall ' +
                      '-Wextra -Wwrite-strings -Wcast-qual -Wpointer-arith '+
                      '-Wsign-compare -Wundef -Wmissing-declarations ' +
                      '-Wredundant-decls -Wmissing-noreturn ' +
                      '-Wno-unused-parameter ' +
                      '-Wno-missing-field-initializers  -Wcast-align ' +
                      '-DHAVE_POSIX ' +
                      '-DHAVE_STDINT_H -DHAVE_UNISTD_H -DHAVE_VASPRINTF ' +
                      '-DRADIANS -DEYE_CANDY  -DHAVE_BOOST -DXCSOAR ' +
                      '-D_GNU_SOURCE=1 -D_REENTRANT ' +
                      '-DENABLE_SDL  -DENABLE_OPENGL -DGL_GLEXT_PROTOTYPES ')

env.Append(LINKFLAGS = '-m32')
dbgenv = env
stdenv = env
dbgenv.Append(CXXFLAGS = ' -g -O0 -DNDEBUG')
stdenv.Append(CXXFLAGS = ' -O2')
if int(debug):
  e = dbgenv
else:
  e = stdenv


Export('e', 'verbose', 'debug', 'targets', 'topdir', 'topsrcdir', 'enable_sdl')

for target in targets:
    outbase = 'output/' + target + '/'
    Export('outbase', 'target')
#    SConscript(topsrcdir + 'OS/SConscript', variant_dir=outbase + 'src/OS/', duplicate=0)
    SConscript('./SConscript', variant_dir=outbase, duplicate=0)

