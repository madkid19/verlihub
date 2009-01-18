INCLUDES = $(all_includes) -I$(top_srcdir)/src
noinst_HEADERS = cpiplug.h cplugs.h cconsole.h
lib_LTLIBRARIES =  libplug_pi.la
libplug_pi_la_LDFLAGS = -module  -L$(top_builddir)/src
libplug_pi_la_LIBADD = -lverlihub
libplug_pi_la_SOURCES = cpiplug.cpp cplugs.cpp cconsole.cpp
