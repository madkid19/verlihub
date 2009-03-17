
# set the include path found by configure
INCLUDES= $(all_includes)
AM_CXXFLAGS = @CXXFLAGS@ @LUA_CFLAGS@

# the library search path.
lib_LTLIBRARIES = liblua_pi.la
noinst_HEADERS = cluainterpreter.h callbacks.h cconsole.h cpilua.h
liblua_pi_la_SOURCES = cpilua.cpp cluainterpreter.cpp callbacks.cpp cconsole.cpp
liblua_pi_la_LDFLAGS = -module @LIBLUA@ ${VERLIHUB_LIBS} -lvhapi 
liblua_pi_la_LIBADD = 
