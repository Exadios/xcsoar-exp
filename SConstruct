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
verbose = ARGUMENTS.get('VERBOSE', 1)
debug   = ARGUMENTS.get('DEBUG', 0)
targets = ARGUMENTS.get('TARGETS', ['UNIX32'])
if int(verbose):
    env['CXXCOMSTR']  = 'Compiling $TARGET'
    env['LINKCOMSTR'] = 'Linking $TARGET'

env.Append(CXXFLAGS = '-I' + topsrcdir + ' ' +
                      '-fno-exceptions ' +
                      '-fno-rtti ' +
                      '-std=gnu++0x ' +
                      '-fno-threadsafe-statics ' +
                      '-fmerge-all-constants ' +
                      '-funit-at-a-time ' +
                      '-Wp ' +
                      '-m32 ' +
                      '-g -O0 ' +
                      '-DHAVE_POSIX ')
env.Append(LINKFLAGS = '-m32')

Export('env', 'verbose', 'debug', 'targets', 'topdir', 'topsrcdir')

for target in targets:
    outbase = 'output/' + target + '/'
    Export('outbase')
    SConscript(topsrcdir + 'OS/SConscript', variant_dir=outbase + 'src/OS/', duplicate=0)
    SConscript('./SConscript', variant_dir=outbase, duplicate=0)

