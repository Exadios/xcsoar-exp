ts = subproject('tool_sources')
gst_sources = ts.get_variable('gst_sources')

generatesinetables = executable('generatesinetables', gst_sources,
                                include_directories : common_includes
                               )

text2_helper = find_program('Meson/scripts/text2-helper')

# Some custom targets

inputevents_text2event = custom_target('InputEvents_Text2Event',
                                       command : [ text2_helper,
                                                   'Text2Event.pl',
                                                   '@INPUT@',
                                                   '@OUTPUT@',
                                                 ],
                                        input  : '../src/Input/InputEvents.hpp',
                                        output : 'InputEvents_Text2Event.cpp',
                                       )

inputevents_text2gce = custom_target('InputEvents_Text2GCE',
                                     command : [ text2_helper,
                                                 'Text2GCE.pl',
                                                 '@INPUT@',
                                                 '@OUTPUT@',
                                               ],
                                      input  : '../src/Input/InputQueue.hpp',
                                      output : 'InputEvents_Text2GCE.cpp',
                                     )

inputevents_text2ne = custom_target('InputEvents_Text2NE',
                                    command : [ text2_helper,
                                                'Text2NE.pl',
                                                '@INPUT@',
                                                '@OUTPUT@',
                                              ],
                                     input  : '../src/Input/InputQueue.hpp',
                                     output : 'InputEvents_Text2NE.cpp',
                                    )

inputevents_char2gce = custom_target('InputEvents_Char2GCE',
                                     command : [ text2_helper,
                                                 'Char2GCE.pl',
                                                 '@INPUT@',
                                                 '@OUTPUT@',
                                               ],
                                      input  : '../src/Input/InputQueue.hpp',
                                      output : 'InputEvents_Char2GCE.cpp',
                                     )

inputevents_char2ne = custom_target('InputEvents_Char2NE',
                                    command : [ text2_helper,
                                                'Char2NE.pl',
                                                '@INPUT@',
                                                '@OUTPUT@',
                                              ],
                                     input  : '../src/Input/InputQueue.hpp',
                                     output : 'InputEvents_Char2NE.cpp',
                                     )

