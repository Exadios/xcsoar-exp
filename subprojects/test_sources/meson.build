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

testflarmchain_sources = [
                          'TestFlarmChain.cpp',
                          source_root + 'Device/Parser.cpp',
                          source_root + 'Units/System.cpp',
                          source_root + 'Geo/Geoid.cpp',
                          source_root + 'Atmosphere/AirDensity.cpp',
                          source_root + 'Units/Descriptor.cpp',
                         ]

test_sources = [
                feednmea_sources,
                testflarmchain_sources,
               ]
