client = subproject('client_sources')
client_sources = client.get_variable('client_sources')
client_cpp_args = client.get_variable('client_cpp_args')

client_lib = library('client', client_sources,
                     include_directories : common_includes,
                     cpp_args : client_cpp_args, 
                    )
