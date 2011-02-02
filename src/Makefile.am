
# set the include path found by configure
INCLUDES = $(all_includes)
# the library search path.
bin_PROGRAMS = verlihub
#Fix LOCALEDIR
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

verlihubdir=$(pkgincludedir)
verlihub_HEADERS = cantiflood.h casyncconn.h casyncsocketserver.h cban.h cbanlist.h ccallbacklist.h ccallbacklist.h cchatconsole.h ccmdr.h ccmdr.h ccommand.h ccommand.h cconfigbase.h cconfigfile.h cconfigitembase.h cconfmysql.h cconnbase.h cconnchoose.h cconnchoose.h cconndc.h cconnpoll.h cconnpoll.h cconnselect.h cconntypes.h cdbconf.h cdbconf.h cdcclient.h cdcclients.h cdccommand.h cdcconf.h cdcconsole.h cdcproto.h cdctag.h cfreqlimiter.h cgeoip.h cgeoip.h cinfoserver.h cinterpolexp.h ckick.h ckicklist.h ckicklist.h cmeanfrequency.h cmessagedc.h cmutex.h cmysql.h cobj.h coperator.h cpcre.h cpenaltylist.h cpenaltylist.h cpluginbase.h cpluginloader.h cpluginloader.h cpluginmanager.h cpluginmanager.h cprotocol.h cprotocommand.h cquery.h creglist.h creguserinfo.h cserverdc.h csetuplist.h csetuplist.h ctempfunctionbase.h cthread.h cthreadwork.h ctime.h ctimeout.h ctrigger.h ctriggers.h curr_date_time.h cusercollection.h cusercollection.h cuser.h cvhplugin.h cvhplugin.h cvhpluginmgr.h cvhpluginmgr.h cworkerthread.h dll.h make_time.h script_api.h stringutils.h tcache.h tchashlistmap.h tchashlistmap.h thasharray.h thasharray.h tlistconsole.h tlistplugin.h tmysqlmemoryhash.h tmysqlmemorylist.h tmysqlmemoryordlist.h tpluginbase.h ccustomredirects.h ccustomredirect.h czlib.h i18n.h

nodist_verlihub_HEADERS = ../dirsettings.h

all_sources = cantiflood.cpp casyncconn.cpp casyncsocketserver.cpp cban.cpp cbanlist.cpp ccallbacklist.cpp cchatconsole.cpp ccmdr.cpp ccommand.cpp cconfigbase.cpp cconfigfile.cpp cconfigitembase.cpp cconfmysql.cpp cconnchoose.cpp cconndc.cpp cconnpoll.cpp cconnselect.cpp cconntypes.cpp cdbconf.cpp cdcclient.cpp cdcclients.cpp cdccommand.cpp cdcconf.cpp cdcconsole.cpp cdcproto.cpp cdctag.cpp cfreqlimiter.cpp cgeoip.cpp cinfoserver.cpp cinterpolexp.cpp ckick.cpp ckicklist.cpp cmeanfrequency.cpp cmessagedc.cpp cmutex.cpp cmysql.cpp cobj.cpp coperator.cpp cpcre.cpp cpenaltylist.cpp cpluginbase.cpp cpluginloader.cpp cpluginmanager.cpp cprotocol.cpp cprotocommand.cpp cquery.cpp creglist.cpp creguserinfo.cpp cserverdc.cpp csetuplist.cpp ctempfunctionbase.cpp cthread.cpp cthreadwork.cpp ctime.cpp ctimeout.cpp ctrigger.cpp ctriggers.cpp cusercollection.cpp cuser.cpp cvhplugin.cpp cvhpluginmgr.cpp cworkerthread.cpp script_api.cpp stringutils.cpp thasharray.cpp tpluginbase.cpp ccustomredirects.cpp ccustomredirect.cpp czlib.cpp


if DEBUG
lib_LIBRARIES = libverlihub.a
libverlihub_a_SOURCES = $(all_sources)
verlihub_LDADD = libverlihub.a @LIBINTL@ $(PCRE_LIBS) $(MYSQL_LIBS) $(DLOPEN_LIBS) $(CRYPT_LIBS) $(GEOIP_LIBS)
libverlihub_a_CXXFLAGS = $(AM_CXXFLAGS)
else
lib_LTLIBRARIES = libverlihub.la libvhapi.la
libverlihub_la_LDFLAGS = -no-undefined -rdynamic $(VERLIHUB_LIBS)
#$(PCRE_LIBS) $(MYSQL_LIBS) $(DLOPEN_LIBS) -lcrypt $(GEOIP_LIBS) $(PTHREAD_LIBS)
libverlihub_la_SOURCES = $(all_sources)
verlihub_LDADD = libverlihub.la
libvhapi_la_SOURCES = script_api.cpp script_api.h
libvhapi_la_LDFLAGS = -module -shared -dy 
endif

verlihub_SOURCES = verlihub.cpp
verlihub_LDFLAGS = $(VERLIHUB_LIBS)
if DEBUG
else
verlihub_LDFLAGS += -rdynamic
endif

libverlihub_la_LIBADD = $(GEOIP_LIBS)

make_time.h: Makefile
	echo "#define VERLIHUB_MAKE_TIME "`date "+%s"` > make_time.h

