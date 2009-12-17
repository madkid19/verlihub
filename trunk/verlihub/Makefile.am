# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs

DISTCLEANFILES = dirsettings.h
BUILT_SOURCES = dirsettings.h
dirsettings.h: Makefile
	echo '#define DATADIR "$(datadir)"' >$@
	echo '#define SYSCONFDIR "$(sysconfdir)"' >>$@
	echo '#define PREFIXDIR "${prefix}"' >> $@

SUBDIRS = src plugins scripts share po
EXTRA_DIST = noapos_mysql noapos_pcre config_geoip verlihub.spec config.rpath ChangeLog  AUTHORS COPYING INSTALL LICENSE TODO config.h.in
DIST_SUBDIRS = $(SUBDIRS)
bin_SCRIPTS = verlihub_config


