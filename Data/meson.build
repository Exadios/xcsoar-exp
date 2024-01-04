#message(meson.source_root())
#tool = join_paths(meson.source_root(), 'tools/BinToC')
#bintoc = find_program(tool, required : true)
#egm96sc = custom_target('egm96sc',
#                        command : [ bintoc,
#                                    '@INPUT@',
#                                    '@OUTPUT@',
#                                    '@SOURCE_ROOT@',
#                                    '@BUILD_ROOT@',
#                                  ],
#                        input   : 'other/egm96s.dem',
#                        output  : 'egm96s.dem.c',
#                       )
make = find_program('make', required : true)
custom_target('egm96s',
              command : [ make,
                          'output/data/egm96s.dem.c',
                        ],
              output  : 'egm96s.dem.c',
             )

egm96s = library('egm96s', 'egm96s.dem.c')

