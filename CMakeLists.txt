cmake_minimum_required (VERSION 2.6)
PROJECT(VERLIHUB)
SET(VERLIHUB_VERSION_MAJOR 0)
SET(VERLIHUB_VERSION_MINOR 9)
SET(VERLIHUB_VERSION_PATCH 9)
SET(VERLIHUB_VERSION "${VERLIHUB_VERSION_MAJOR}.${VERLIHUB_VERSION_MINOR}.${VERLIHUB_VERSION_PATCH}")

SET(VERLIHUB_SOVERSION "${VERLIHUB_VERSION}")
SET(PROJECT_NAME "verlihub")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)
INCLUDE (CheckIncludeFiles)


OPTION(WITH_GEOIP "Enable Geolocalization support" ON)
OPTION(WITH_OPENSSL "Enable OpenSSL support" ON)
OPTION(WITH_PLUGMAN "Enable Pluging Manager support" ON)

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" ON)
else()
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" OFF)
endif()

# Find dependences
Find_Package(OpenSSL)
Find_Package(Crypt)
IF(NOT OPENSSL_FOUND AND NOT CRYPT_FOUND)
	MESSAGE(STATUS "WARNING: OpenSSL and Crypt not found. Password will be stored in plaintext.")
ENDIF(NOT OPENSSL_FOUND AND NOT CRYPT_FOUND)

IF(OPENSSL_FOUND)
	CHECK_INCLUDE_FILES(openssl/md5.h HAVE_OPENSSL_MD5_H)
ENDIF(OPENSSL_FOUND)

IF(CRYPT_FOUND)
	SET(HAVE_LIBCRYPT 1)
ENDIF(CRYPT_FOUND)

Find_Package(MySQL)

Find_Package(ZLIB REQUIRED)
Find_Package(GeoIP REQUIRED)
Find_Package(DL REQUIRED)
Find_Package(Pcre REQUIRED)
Find_Package(Threads REQUIRED)
Find_Package(Gettext REQUIRED)
Find_Package(Gettext-devel REQUIRED)
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

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

FIND_PROGRAM(BASH_PATH bash PATHS /usr/bin /usr/local/bin)
IF(NOT BASH_PATH)
	MESSAGE(FATAL_ERROR "Bash shell not found. Please install Bourne Again SHell")
ENDIF(NOT BASH_PATH)

# ----------------------------------------------------------------------------------------------------
# Generate configure files
MESSAGE(STATUS "Generating config.h file")
CONFIGURE_FILE(config.h.cm config.h)
MESSAGE(STATUS "Generating dirsettings.h file")
CONFIGURE_FILE(dirsettings.h.cm dirsettings.h)
MESSAGE(STATUS "Generating FindVerliHub.cmake file")
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake_modules/FindVerliHub.cmake.cm" "${CMAKE_BINARY_DIR}/cmake_modules/FindVerliHub.cmake")
# ----------------------------------------------------------------------------------------------------


if(INSTALL_CMAKE_FILES)
 	install(FILES "${CMAKE_BINARY_DIR}/cmake_modules/FindVerliHub.cmake" DESTINATION ${CMAKE_ROOT}/Modules)
else()
 	install(FILES "${CMAKE_BINARY_DIR}/cmake_modules/FindVerliHub.cmake" DESTINATION ${SHARE_INSTALL_DIR}/VerliHub/cmake)
endif()

# Create uninstall configuration file and uninstall target
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")
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
# Centos deps to compile: gcc-c++, mysql, mysql-devel, cmake, zlib, openssl, openssl-devel
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
SET(CPACK_GENERATOR "DEB;RPM;TBZ2")
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VERLIHUB_VERSION}-${CMAKE_SYSTEM_PROCESSOR}")
INCLUDE(CPack)
# ----------------------------------------------------------------------------------------------------


message(STATUS)
message(STATUS "========== VerliHub Build Information ==========")
message(STATUS "Build Version: ${VERLIHUB_VERSION}")
message(STATUS "Install Prefix (CMAKE_INSTALL_PREFIX): ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Install the *.cmake files into CMake root (INSTALL_CMAKE_FILES): ${INSTALL_CMAKE_FILES}")
message(STATUS "Enable Geolocalization support (WITH_GEOIP): ${WITH_GEOIP}")
message(STATUS "Enable OpenSSL support (WITH_OPENSSL): ${WITH_OPENSSL}")
message(STATUS "Enable Pluging Manager support (WITH_PLUGMAN): ${WITH_PLUGMAN}")
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the command line.")
message(STATUS "To build and install VerliHub, run \"make\" and \"make install\"")
message(STATUS)
