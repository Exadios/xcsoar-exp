port = subproject('port_sources')
port_sources = port.get_variable('port_sources')
port_cpp_args = port.get_variable('port_cpp_args')

port_lib = library('port', port_sources,
                   include_directories : common_includes,
                   cpp_args : port_cpp_args,
                  )
