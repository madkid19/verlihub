INCLUDE_DIRECTORIES(${VERLIHUB_BINARY_DIR} ${VERLIHUB_SOURCE_DIR}/src)

SET(VERLIHUB_HDRS
	cantiflood.h
	casyncconn.h
	casyncsocketserver.h
	cban.h
	cbanlist.h
	ccallbacklist.h
	ccallbacklist.h
	cchatconsole.h
	ccmdr.h
	ccmdr.h
	ccommand.h
	ccommand.h
	cconfigbase.h
	cconfigfile.h
	cconfigitembase.h
	cconfmysql.h
	cconnbase.h
	cconnchoose.h
	cconnchoose.h
	cconndc.h
	cconnpoll.h
	cconnpoll.h
	cconnselect.h
	cconntypes.h
	cdbconf.h
	cdbconf.h
	cdcclient.h
	cdcclients.h
	cdccommand.h
	cdcconf.h
	cdcconsole.h
	cdcproto.h
	cdctag.h
	cfreqlimiter.h
	cgeoip.h
	cgeoip.h
	cinfoserver.h
	cinterpolexp.h
	ckick.h
	ckicklist.h
	ckicklist.h
	cmeanfrequency.h
	cmessagedc.h
	cmutex.h
	cmysql.h
	cobj.h
	coperator.h
	cpcre.h
	cpenaltylist.h
	cpenaltylist.h
	cpluginbase.h
	cpluginloader.h
	cpluginloader.h
	cpluginmanager.h
	cpluginmanager.h
	cprotocol.h
	cprotocommand.h
	cquery.h
	creglist.h
	creguserinfo.h
	cserverdc.h
	csetuplist.h
	csetuplist.h
	ctempfunctionbase.h
	cthread.h
	cthreadwork.h
	ctime.h
	ctimeout.h
	ctrigger.h
	ctriggers.h
	curr_date_time.h
	cusercollection.h
	cusercollection.h
	cuser.h
	cvhplugin.h
	cvhplugin.h
	cvhpluginmgr.h
	cvhpluginmgr.h
	cworkerthread.h
	script_api.h
	stringutils.h
	tcache.h
	tchashlistmap.h
	tchashlistmap.h
	thasharray.h
	thasharray.h
	tlistconsole.h
	tlistplugin.h
	tmysqlmemoryhash.h
	tmysqlmemorylist.h
	tmysqlmemoryordlist.h
	tpluginbase.h
	ccustomredirects.h
	ccustomredirect.h
	czlib.h
)
SET(VERLIHUB_SRCS
	cantiflood.cpp
	casyncconn.cpp
	casyncsocketserver.cpp
	cban.cpp
	cbanlist.cpp
	ccallbacklist.cpp
	cchatconsole.cpp
	ccmdr.cpp
	ccommand.cpp
	cconfigbase.cpp
	cconfigfile.cpp
	cconfigitembase.cpp
	cconfmysql.cpp
	cconnchoose.cpp
	cconndc.cpp
	cconnpoll.cpp
	cconnselect.cpp
	cconntypes.cpp
	cdbconf.cpp
	cdcclient.cpp
	cdcclients.cpp
	cdccommand.cpp
	cdcconf.cpp
	cdcconsole.cpp
	cdcproto.cpp
	cdctag.cpp
	cfreqlimiter.cpp
	cgeoip.cpp
	cinfoserver.cpp
	cinterpolexp.cpp
	ckick.cpp
	ckicklist.cpp
	cmeanfrequency.cpp
	cmessagedc.cpp
	cmutex.cpp
	cmysql.cpp
	cobj.cpp
	coperator.cpp
	cpcre.cpp
	cpenaltylist.cpp
	cpluginbase.cpp
	cpluginloader.cpp
	cpluginmanager.cpp
	cprotocol.cpp
	cprotocommand.cpp
	cquery.cpp
	creglist.cpp
	creguserinfo.cpp
	cserverdc.cpp
	csetuplist.cpp
	ctempfunctionbase.cpp
	cthread.cpp
	cthreadwork.cpp
	ctime.cpp
	ctimeout.cpp
	ctrigger.cpp
	ctriggers.cpp
	cusercollection.cpp
	cuser.cpp
	cvhplugin.cpp
	cvhpluginmgr.cpp
	cworkerthread.cpp
	script_api.cpp
	stringutils.cpp
	thasharray.cpp
	tpluginbase.cpp
	ccustomredirects.cpp
	ccustomredirect.cpp
	czlib.cpp
)

# ----------------------------------------------------------------------------------------------------
# Generate make_time.h
ADD_CUSTOM_COMMAND(
	OUTPUT make_time.h
	COMMAND ${BASH_PATH} "echo \"#define VERLIHUB_MAKE_TIME \"`date \"+%s\"`" > make_time.h
	DEPENDS
	${VERLIHUB_SRCS}

)
ADD_CUSTOM_TARGET(make_time.h ALL)
LIST(APPEND ${VERLIHUB_HDRS} make_time.h)

SET(VERLIHUB_LIBS ${OPENSSL_LIBRARIES} ${CRYPT_LIBRARIES} ${MYSQL_LIBRARIES} ${GEOIP_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${GEOIP_INCLUDE_DIR} ${PCRE_INCLUDE_DIR})

# Use cmake -DDEFINE_DEBUG=ON to enable debug
OPTION(DEFINE_DEBUG "Build the project using debugging code" OFF)
IF(DEFINE_DEBUG)
	MESSAGE("Adding debug flag...")
	SET(CMAKE_CXX_FLAGS "-Wall -O3")
	SET(CMAKE_BUILD_TYPE Debug)
	MESSAGE("Build type is " ${CMAKE_BUILD_TYPE})
ENDIF(DEFINE_DEBUG)

# ----------------------------------------------------------------------------------------------------

# TODO: Do not compile *.cpp files twice for verlihub anb libverlihub.
# See: http://www.paraview.org/Wiki/CMake_FAQ#Does_that_mean_I_have_to_build_all_my_library_objects_twice.2C_once_for_shared_and_once_for_static.3F.21_I_don.27t_like_that.21
ADD_LIBRARY(libverlihub SHARED ${VERLIHUB_SRCS})
SET_TARGET_PROPERTIES(libverlihub PROPERTIES OUTPUT_NAME "verlihub")
SET_TARGET_PROPERTIES(libverlihub PROPERTIES VERSION 1.0.0 SOVERSION ${VERLIHUB_SOVERSION})
#target_link_libraries(libverlihub "-no-undefined" ${VERLIHUB_LIBS})
target_link_libraries(libverlihub ${VERLIHUB_LIBS})

ADD_LIBRARY(vhapi SHARED "script_api.cpp")
SET_TARGET_PROPERTIES(libverlihub PROPERTIES VERSION 1.0.0 SOVERSION ${VERLIHUB_SOVERSION})
#target_link_libraries(vhapi "-module -shared -dy")

ADD_EXECUTABLE(verlihub verlihub.cpp)
target_link_libraries(verlihub libverlihub)

# ----------------------------------------------------------------------------------------------------
# Generate verlihub_config
MESSAGE(STATUS "Generating verlihub_config")
STRING(REPLACE ";" " " VERLIHUB_LIBS "${VERLIHUB_LIBS}")
CONFIGURE_FILE(../verlihub_config.cm ../verlihub_config)
# ----------------------------------------------------------------------------------------------------

#TODO: Add -rdynamic for backtracking
INSTALL(TARGETS vhapi libverlihub verlihub
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)
INSTALL(FILES ${VERLIHUB_BINARY_DIR}/verlihub_config DESTINATION bin)
INSTALL(FILES ${VERLIHUB_HDRS} ${VERLIHUB_BINARY_DIR}/dirsettings.h DESTINATION include/${PROJECT_NAME})