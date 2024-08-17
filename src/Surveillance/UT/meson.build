cxx = meson.get_compiler('cpp')
libpthread      = cxx.find_library('libpthread')

libsurveillance_test_harness = executable('libsurveillance_test_harness',
                                          [
                                           'test_harness.cpp',
                                           'LogFile.cpp' 
                                          ],
                                          include_directories : common_includes,
                                          link_with : [ surveillance_lib,
                                                        thread_lib,
                                                        resource_lib,
                                                      ],
                                          dependencies : libpthread
                                         )
test('surveillance_lib test 1', libsurveillance_test_harness,
     args : '1', ) 
test('surveillance_lib test 2', libsurveillance_test_harness,
     args : '2', )
test('surveillance_lib test 3', libsurveillance_test_harness,
     args : '3', )
test('surveillance_lib test 4', libsurveillance_test_harness,
     args : '4', )
test('surveillance_lib test 5', libsurveillance_test_harness,
     args : '5', )
test('surveillance_lib test 6', libsurveillance_test_harness,
     args : '6', )
test('surveillance_lib test 7', libsurveillance_test_harness,
     args : '7', )
test('surveillance_lib test 8', libsurveillance_test_harness,
     args : '8', )
test('surveillance_lib test 9', libsurveillance_test_harness,
     args : '9', )

targetlist_semantics = executable('targetlist-semantics',
                                  'TargetList-Semantics.cpp',
                                  include_directories : common_includes,
                                  link_with : [ surveillance_lib,
                                                thread_lib,
                                                resource_lib,
                                              ],
                                   dependencies : libpthread
                                  )
