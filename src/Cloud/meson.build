cloudtokml = subproject('cloudtokml_sources')
# The silliness below is because some sources need to be compiled with
# "-Wno-unused-parameter". This is because some boost library code needs
# it.
cloudtokml_unsued_parameter_sources = cloudtokml.get_variable('cloudtokml_unused_parameter_sources')
cloudtokml_sources = cloudtokml.get_variable('cloudtokml_sources')

cloudtokml_extra_lib = static_library('cloudtokml_unsued_parameter_sources',
                                      cloudtokml_unsued_parameter_sources,
                                      include_directories : common_includes,
                                      cpp_args : '-Wno-unused-parameter',
                                      build_by_default : false,
                                     )

cloudtokml_lib = library('cloudtokml', cloudtokml_sources,
                         include_directories : common_includes,
                         link_whole          : cloudtokml_extra_lib,
                        )
