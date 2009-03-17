# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs

DISTCLEANFILES = dirsettings.h
BUILT_SOURCES = dirsettings.h
dirsettings.h: Makefile
	echo '#define DATADIR "$(datadir)"' >$@
	echo '#define SYSCONFDIR "$(sysconfdir)"' >>$@
	echo '#define PREFIXDIR "${prefix}"' >> $@

SUBDIRS = src plugins scripts share
EXTRA_DIST = noapos_mysql noapos_pcre config_geoip verlihub.spec
DIST_SUBDIRS = docs $(SUBDIRS)
bin_SCRIPTS = verlihub_config


