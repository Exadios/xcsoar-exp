json = subproject('json_sources')
json_sources = json.get_variable('json_sources')
json_cpp_args = json.get_variable('json_cpp_args')

json_lib = library('json', json_sources,
                   cpp_args            :
                                         [
                                     #      '-funit-at-a-time',
                                     #      '-ffast-math',
                                     #      '-std=c++20',
                                     #      '-fno-threadsafe-statics',
                                     #      '-fmerge-all-constants',
                                     #      '-fcoroutines',
                                     #      '-fconserve-space',
                                     #      '-fno-operator-names',
                                     #      '-fvisibility=hidden',
                                     #      '-finput-charset=utf-8',
                                     #      '-Wall',
                                     #      '-Wextra',
                                     #      '-Wwrite-strings',
                                     #      '-Wcast-qual',
                                     #      '-Wpointer-arith',
                                     #      '-Wsign-compare',
                                     #      '-Wundef',
                                     #      '-Wmissing-declarations',
                                     #      '-Wredundant-decls',
                                     #      '-Wmissing-noreturn',
                                     #      '-Wvla',
                                     #      '-Wno-format-truncation',
                                     #      '-Wno-missing-field-initializers',
                                     #      '-Wcast-align',
                                     #      '-Werror',
                                     #      '-I../../src/unix',
                                     #      '-I../../output/include',
                                     #      '-isystem ../../output/src/boost_1_80_0',
                                           '-DBOOST_NO_IOSTREAM',
                                           '-DBOOST_MATH_NO_LEXICAL_CAST',
                                           '-DBOOST_UBLAS_NO_STD_CERR',
                                           '-DBOOST_ERROR_CODE_HEADER_ONLY',
                                           '-DBOOST_SYSTEM_NO_DEPRECATED',
                                           '-DBOOST_NO_STD_LOCALE',
                                           '-DBOOST_LEXICAL_CAST_ASSUME_C_LOCALE',
                                     #     '-DBOOST_NO_CXX98_FUNCTION_BASE',
                                     #      '-I../../src',
                                     #      '-I../../src/Engine',
                                     #      '-I../../output/UNIX/include',
                                     #      '-DHAVE_POSIX',
                                     #      '-DHAVE_VASPRINTF',
                                     #      '-DEYE_CANDY',
                                     #      '-D_GLIBCXX_ASSERTIONS',
                                           '-DBOOST_JSON_STANDALONE',
                                           json_cpp_args,
                                         ],
                   include_directories : common_includes,
                  )
