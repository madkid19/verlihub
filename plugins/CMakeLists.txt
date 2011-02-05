# Use cmake -DDDISABLE_PLUGMAN=ON to disable plugman
OPTION(DISABLE_PLUGMAN "Do not build pluging manager (aka plugman) plugin" OFF)
IF(NOT DISABLE_PLUGMAN)
	ADD_SUBDIRECTORY(plugman)
ENDIF(NOT DISABLE_PLUGMAN)
