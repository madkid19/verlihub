cmake_minimum_required (VERSION 2.6)
PROJECT(VERLIHUB)
SET(VERLIHUB_VERSION_MAJOR 0)
SET(VERLIHUB_VERSION_MINOR 9)
SET(VERLIHUB_VERSION_PATCH 9)
SET(VERLIHUB_VERSION "${VERLIHUB_VERSION_MAJOR}.${VERLIHUB_VERSION_MINOR}.${VERLIHUB_VERSION_PATCH}")

SET(VERLIHUB_SOVERSION "${VERLIHUB_VERSION}")
SET(PROJECT_NAME "verlihub")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
INCLUDE (CheckIncludeFiles)
INCLUDE(MacroLogFeature)
INCLUDE(VerliHubMacro)


OPTION(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" ON)

# Find dependences
Find_Package(OpenSSL)
MACRO_LOG_FEATURE(OPENSSL_FOUND "openssl" "OpenSSL cryptography library" "http://www.openssl.org/" "" "OpenSSL NOT found")
Find_Package(Crypt)
MACRO_LOG_FEATURE(CRYPT_FOUND "crypto" "Crypto cryptographic primitives")
IF(CRYPT_FOUND)
	SET(HAVE_LIBCRYPT 1)
ENDIF(CRYPT_FOUND)
IF(NOT OPENSSL_FOUND AND NOT CRYPT_FOUND)
	MESSAGE(STATUS "WARNING: OpenSSL and Crypt not found. Password will be stored in plaintext.")
ENDIF(NOT OPENSSL_FOUND AND NOT CRYPT_FOUND)

IF(OPENSSL_FOUND)
	CHECK_INCLUDE_FILES(openssl/md5.h HAVE_OPENSSL_MD5_H)
ENDIF(OPENSSL_FOUND)

Find_Package(MySQL REQUIRED)
Find_Package(ZLIB REQUIRED)
Find_Package(GeoIP)
MACRO_LOG_FEATURE(GEOIP_FOUND "geoip" "Geolocalization support library" "http://www.maxmind.com/geoip/api/c.shtml")
Find_Package(DL REQUIRED)
Find_Package(Pcre REQUIRED)
Find_Package(Threads REQUIRED)
Find_Package(Gettext REQUIRED)
Find_Package(Gettext-devel REQUIRED)

SET(VERLIHUB_LIBS ${OPENSSL_LIBRARIES} ${CRYPT_LIBRARIES} ${MYSQL_LIBRARIES} ${GEOIP_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GETTEXT_ASPRINTF_LIBRARY})
SET(VERLIHUB_SCRIPTS_LIBS "-lverlihub -lvhapi")
SET(VERLIHUB_INCLUDE_DIRS  "-I${OPENSSL_INCLUDE_DIR} -I${MYSQL_INCLUDE_DIR} -I${GEOIP_INCLUDE_DIR} -I${ZLIB_INCLUDE_DIRS} -I${PCRE_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${GEOIP_INCLUDE_DIR} ${PCRE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR})
# ----------------------------------------------------------------------------------------------------

CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILES(sys/poll.h HAVE_SYS_POLL_H)
#TODO: Fix creguserinfo.cpp for HAVE_LIBSSL and crypto stuff

# Detect Windows, FreeBSD or Linux
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
     ADD_DEFINITIONS(-D_WIN32 1)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
     ADD_DEFINITIONS(-DHAVE_FREEBSD)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     ADD_DEFINITIONS(-DHAVE_APPLE)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Use cmake -DDEFINE_DEBUG=ON to enable debug
OPTION(DEFINE_DEBUG "Build the project using debugging code" OFF)
IF(DEFINE_DEBUG)
	# enable gcc specific stuff
	if (CMAKE_COMPILER_IS_GNUCC)
		SET(CMAKE_CXX_FLAGS "-Wall -O3")
		SET(CMAKE_BUILD_TYPE Debug)
		MESSAGE(STATUS "Build type is " ${CMAKE_BUILD_TYPE})
	ENDIF()
ENDIF(DEFINE_DEBUG)

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF(UNIX)
	FIND_PROGRAM(BASH_PATH bash PATHS /usr/bin /usr/local/bin)
	IF(NOT BASH_PATH)
		MESSAGE(FATAL_ERROR "Bash shell not found. Please install Bourne Again SHell")
	ENDIF(NOT BASH_PATH)
ENDIF(UNIX)

# ----------------------------------------------------------------------------------------------------
# Generate configure files
MESSAGE(STATUS "Generating config.h file")
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/config.h.cm" "${CMAKE_BINARY_DIR}/config.h")
MESSAGE(STATUS "Generating dirsettings.h file")
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/dirsettings.h.cm" "${CMAKE_BINARY_DIR}/dirsettings.h")
# ----------------------------------------------------------------------------------------------------


if(INSTALL_CMAKE_FILES)
 	install(FILES "${CMAKE_SOURCE_DIR}/cmake/Modules/FindVerliHub.cmake" DESTINATION ${CMAKE_ROOT}/Modules)
endif()

# Create uninstall configuration file and uninstall target
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_BINARY_DIR}/cmake/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
# ----------------------------------------------------------------------------------------------------

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(share)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(po)

# ----------------------------------------------------------------------------------------------------
# Build a CPack driven installer package
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_NAME "verlihub")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_DESCRIPTION "VerliHub Direct Connect Hub Server")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VerliHub Direct Connect Hub Server")

SET(CPACK_PACKAGE_CONTACT "bugs@verlihub-project.org")
SET(CPACK_PACKAGE_VENDOR "VerliHub Team")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERLIHUB_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERLIHUB_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERLIHUB_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION "${VERLIHUB_VERSION}")
SET(CPACK_DEBIAN_PACKAGE_SECTION "net")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "zlib, mysql-server-5.1, mysql-client-5.1, geoip-bin, gettext, openssl, libpcre3")

SET(CPACK_RPM_PACKAGE_LICENSE ${CPACK_RESOURCE_FILE_LICENSE})
SET(CPACK_RPM_PACKAGE_URL "http://www.verlihub-project.org")
SET(CPACK_RPM_PACKAGE_GROUP "Application/Internet")
set(CPACK_RPM_PACKAGE_REQUIRES "zlib, openssl, mysql-server >= 5.0, mysql >= 5.0, GeoIP, gettext, pcre")
IF(WITH_PYTHON)
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, python (>= 2.5)")
	SET(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, python >= 2.5")
ENDIF(WITH_PYTHON)
IF(WITH_LUA)
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, lua (>= 5.1)")
	SET(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, lua >= 5.1")
ENDIF(WITH_LUA)

SET(CPACK_GENERATOR "DEB;RPM;TBZ2")
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VERLIHUB_VERSION}-${CMAKE_SYSTEM_PROCESSOR}")
INCLUDE(CPack)
# ----------------------------------------------------------------------------------------------------


message(STATUS)
message(STATUS "========== VerliHub Build Information ==========")
message(STATUS "Build Version: ${VERLIHUB_VERSION}")
message(STATUS "Install Prefix (CMAKE_INSTALL_PREFIX): ${CMAKE_INSTALL_PREFIX}")
MACRO_DISPLAY_FEATURE_LOG()
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the command line.")
message(STATUS "To build and install VerliHub, run \"make\" and \"make install\"")
message(STATUS)
