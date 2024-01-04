nmea = subproject('nmea_sources')
nmea_sources = nmea.get_variable('nmea_sources')

nmea_lib = library('nmea', nmea_sources,
                   include_directories : common_includes
                  )
