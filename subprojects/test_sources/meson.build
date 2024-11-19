project('Test_Sources')

source_root = meson.source_root() + '/src/'

feednmea_sources = [
                    'FeedNMEA.cpp',
                    'DebugPort.cpp',
                    'FakeLogFile.cpp',
                    'FakeLanguage.cpp',
                    source_root + 'Operation/ConsoleOperationEnvironment.cpp',
                    source_root + 'Device/Port/ConfiguredPort.cpp',
                    source_root + 'Device/Config.cpp',
                   ]

feedgdl90_sources = [
                     'FeedGDL90.cpp',
                     'DebugPort.cpp',
                     'FakeLogFile.cpp',
                     'FakeLanguage.cpp',
                     source_root + 'Operation/ConsoleOperationEnvironment.cpp',
                     source_root + 'Device/Port/ConfiguredPort.cpp',
                     source_root + 'Device/Config.cpp',
                    ]

testgdl90_sources = [
                     'TestGDL90.cpp',
                     'DebugPort.cpp',
                     'FakeLogFile.cpp',
                     'FakeLanguage.cpp',
                     source_root + 'Operation/ConsoleOperationEnvironment.cpp',
                     source_root + 'Device/Port/ConfiguredPort.cpp',
                     source_root + 'Device/Config.cpp',
                    ]

nmea2gdl90_sources = [
                      'NMEA2GDL90.cpp',
                      'DebugPort.cpp',
                      'FakeLogFile.cpp',
                      'FakeLanguage.cpp',
                      source_root + 'Operation/ConsoleOperationEnvironment.cpp',
                      source_root + 'Device/Port/ConfiguredPort.cpp',
                      source_root + 'Device/Config.cpp',
                      source_root + 'Device/Parser.cpp',
                      source_root + 'Device/Driver/FLARM/StaticParser.cpp',
                      source_root + 'Atmosphere/AirDensity.cpp',
                      source_root + 'Units/System.cpp',
                      source_root + 'Units/Descriptor.cpp',
                      source_root + 'Geo/Geoid.cpp',
                     ]

testflarmchain_sources = [
                          'TestFlarmChain.cpp',
                          source_root + 'Device/Parser.cpp',
                          source_root + 'Units/System.cpp',
                          source_root + 'Geo/Geoid.cpp',
                          source_root + 'Atmosphere/AirDensity.cpp',
                          source_root + 'Units/Descriptor.cpp',
                         ]

appendflarmcksm_sources = [
                           'AppendFlarmCksm.cpp',
                           source_root + 'NMEA/Checksum.hpp',
                          ]

test_sources = [
                feednmea_sources,
                testflarmchain_sources,
                nmea2gdl90_sources,
               ]
