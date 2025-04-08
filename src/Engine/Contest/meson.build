contest = subproject('contest_sources')
contest_sources       = contest.get_variable('contest_sources')
contest_sources_extra = contest.get_variable('contest_sources_extra')

contest_lib_extra = static_library('contest_extra', contest_sources_extra,
                                   include_directories : common_includes,
                                   cpp_args            : '-Wno-range-loop-construct',
                                  build_by_default     : false,
                                  )
contest_lib = library('contest', contest_sources,
                      include_directories : common_includes,
                      link_whole          : contest_lib_extra,
                     )
