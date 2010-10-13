# - Find GeoIP
# Find the GeoIP includes and library
# This module defines
#  GEOIP_INCLUDE_DIR, where to find geoip.h
#  GEOIP_LIBRARIES, the libraries needed to use GeoIP.
#  GEOIP_FOUND, If false, do not try to use GeoIP.
#
	
IF(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)
	SET(GEOIP_FOUND TRUE)
ELSE(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)
	find_path(GEOIP_INCLUDE_DIR GeoIP.h /usr/include /usr/local/include)
	find_library(GEOIP_LIBRARIES NAMES GeoIP PATHS
		/usr/lib
		/usr/local/lib
	)
	IF(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)
		SET(GEOIP_FOUND TRUE)
		MESSAGE(STATUS "Found GeoIP: ${GEOIP_INCLUDE_DIR}, ${GEOIP_LIBRARIES}")
	ELSE(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)
		SET(GEOIP_FOUND FALSE)
		MESSAGE(STATUS "GeoIP not found. Please download and install it http://www.maxmind.com/download/geoip/api/c/")
	ENDIF(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)
	
	mark_as_advanced(GEOIP_INCLUDE_DIR GEOIP_LIBRARIES)
ENDIF(GEOIP_INCLUDE_DIR AND GEOIP_LIBRARIES)