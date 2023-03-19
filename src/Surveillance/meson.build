surveillance = subproject('surveillance_sources')
surveillance_sources = surveillance.get_variable('surveillance_sources')

surveillance_lib = library('surveillance', surveillance_sources,
                           include_directories : common_includes
                          )

subdir('UT')
