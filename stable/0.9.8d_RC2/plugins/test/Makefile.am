noinst_HEADERS =  cpitest.h
lib_LTLIBRARIES =  libtest_pi.la
libtest_pi_la_LDFLAGS = -module -L$(top_builddir)/src
libtest_pi_la_SOURCES =  cpitest.cpp
libtest_pi_la_LIBADD = -lverlihub
INCLUDES = $(all_includes) -I$(top_srcdir)/src $(MYSQL_CFLAGS)
