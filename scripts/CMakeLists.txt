SET(SCRIPTS_FILES
	vh_runhub
	vh_getcfg
	vh_getdb
	vh_install_cli
	vh_regnick
	vh_restart
	vh_setup
	vh_trigger
	vh_install_gui
	vh_install
)

# Autotool variables that will be replaced
SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
SET(libdir ${CMAKE_INSTALL_PREFIX}/lib)
SET(bindir ${CMAKE_INSTALL_PREFIX}/bin)
SET(datadir ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})

FOREACH(VH_SCRIPT ${SCRIPTS_FILES})
	MESSAGE(STATUS "Generating ${VH_SCRIPT}")
	CONFIGURE_FILE(${VH_SCRIPT}.in ${VH_SCRIPT} IMMEDIATE @ONLY)
ENDFOREACH(VH_SCRIPT ${SCRIPTS_FILES})
INSTALL(FILES ${SCRIPTS_FILES} DESTINATION bin)