# - Find where dlopen and friends are located.
# DL_FOUND - system has dynamic linking interface available
# DL_INCLUDE_DIR - where dlfcn.h is located.
# DL_LIBRARIES - libraries needed to use dlopen

INCLUDE(CheckFunctionExists)

IF(DL_INCLUDE_DIR AND DL_LIBRARIES)
	SET(DL_FOUND TRUE)
ELSE(DL_INCLUDE_DIR AND DL_LIBRARIES)
	FIND_PATH(DL_INCLUDE_DIR NAMES dlfcn.h)
	FIND_LIBRARY(DL_LIBRARIES NAMES dl)
	IF(DL_LIBRARIES)
		MESSAGE(STATUS "Found dlopen")
		SET(DL_FOUND TRUE)
	ELSE(DL_LIBRARIES)
		check_function_exists(dlopen DL_FOUND)
		# If dlopen can be found without linking in dl then dlopen is part
		# of libc, so don't need to link extra libs.
		SET(DL_LIBRARIES "")
	ENDIF(DL_LIBRARIES)
ENDIF(DL_INCLUDE_DIR AND DL_LIBRARIES)