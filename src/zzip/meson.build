zzip = subproject('zzip_sources')
zzip_sources = zzip.get_variable('zzip_sources')

zzip_lib = library('zzip', zzip_sources,
                   include_directories : common_includes,
                   # This switch is required to stop Meson inserting a
                   # '-I../../src/zzlib' option on the command line of gcc.
                   # Meson does this by default. This is a disaster for zzlib
                   # because zzlib/stdint.h has "#include <stdint.h>" which
                   # is meant to include /usr/include/stdint.h. However the
                   # Meson inserted option will cause it to include
                   # instead zzlib/stdint.h
                   implicit_include_directories : false,
                  )
