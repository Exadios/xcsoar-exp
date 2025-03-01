json = subproject('json_sources')
json_sources = json.get_variable('json_sources')
json_cpp_args = json.get_variable('json_cpp_args')

json_lib = library('json', json_sources,
                   cpp_args           : [
                                         '-DBOOST_NO_IOSTREAM',
                                         '-DBOOST_MATH_NO_LEXICAL_CAST',
                                         '-DBOOST_UBLAS_NO_STD_CERR',
                                         '-DBOOST_ERROR_CODE_HEADER_ONLY',
                                         '-DBOOST_SYSTEM_NO_DEPRECATED',
                                         '-DBOOST_NO_STD_LOCALE',
                                         '-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE',
                                         '-DBOOST_JSON_STANDALONE',
                                         json_cpp_args,
                                        ],
                   include_directories : common_includes,
                  )
