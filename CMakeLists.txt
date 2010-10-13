cmake_minimum_required (VERSION 2.6)
PROJECT(VERLIHUB)
SET(VERLIHUB_VERSION "0.9.9a")
SET(VERLIHUB_VERSION_MAJOR 0)
SET(VERLIHUB_VERSION_MINOR 9)

SET(VERLIHUB_SOVERSION "1")
SET(PROJECT_NAME "verlihub")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)
INCLUDE (CheckIncludeFiles)

# ----------------------------------------------------------------------------------------------------
# CPPFLAGS, CXXFLAGS and LDFLAGS from the environment
SET(VERLIHUB_COMPILE_FLAGS_ENV "$ENV{CPPFLAGS} ${VERLIHUB_CXXFLAGS} $ENV{CXXFLAGS}")
MESSAGE(STATUS "Environment compile flags: ${VERLIHUB_COMPILE_FLAGS_ENV}")

SET(VERLIHUB_COMPILE_FLAGS_ENV "$ENV{LDFLAGS}")
MESSAGE(STATUS "Environment link flags: ${VERLIHUB_COMPILE_FLAGS_ENV}")
# ----------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------
# Find dependences
Find_Package(OpenSSL)
IF(NOT OPENSSL_FOUND)
	MESSAGE(STATUS "OpenSSL not found. Passwords will not be encrypt and stored in plain-text.")
ENDIF(NOT OPENSSL_FOUND)

Find_Package(MySQL)
IF(NOT MYSQL_FOUND)
	MESSAGE(FATAL_ERROR "MySQL not found. Please install MySQL devel package.")
ENDIF(NOT MYSQL_FOUND)

Find_Package(ZLIB REQUIRED)
Find_Package(GeoIP REQUIRED)
Find_Package(DL REQUIRED)
Find_Package(Pcre REQUIRED)
Find_Package(Threads REQUIRED)
# ----------------------------------------------------------------------------------------------------


CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)

# SET _WIN32 definition
IF(WIN32)
	ADD_DEFINITIONS(-D_WIN32 1)
ENDIF(WIN32)
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
# ----------------------------------------------------------------------------------------------------



# ----------------------------------------------------------------------------------------------------
# Create uninstall configuration file and uninstall target
CONFIGURE_FILE("cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")
# ----------------------------------------------------------------------------------------------------


ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(share)
ADD_SUBDIRECTORY(scripts)

# ----------------------------------------------------------------------------------------------------
# Build a CPack driven installer package
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_RESOURCE_FILE_LICENSE
     "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERLIHUB_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERLIHUB_VERSION_MINOR}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VerliHub Direct Connect Hub Server")
SET(CPACK_PACKAGE_VENDOR "VerliHub Team")
INCLUDE(CPack)
# ----------------------------------------------------------------------------------------------------

MESSAGE(STATUS "Installation path is: ${CMAKE_INSTALL_PREFIX} (overwrite with -DCMAKE_INSTALL_PREFIX=/your/path)")
