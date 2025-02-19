surveillance = subproject('surveillance_sources')
surveillance_sources = surveillance.get_variable('surveillance_sources')

surveillance_lib = library('surveillance', surveillance_sources,
                           include_directories : common_includes
                          )

if get_option('ut_build').enabled()
  subdir('UT')
endif
