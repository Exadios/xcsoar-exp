
src_includes = include_directories('.')

xcsoar = subproject('xcsoar_sources')
xcsoar_sources = xcsoar.get_variable('xcsoar_sources')
resource = subproject('resource_sources')
resource_sources = resource.get_variable('resource_sources')
resource_lib = library('resource', resource_sources,
                       include_directories : common_includes,
                       
                      )
