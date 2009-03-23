# Copyright 1999-2004 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="LUA 5.0 plugins for Verlihub"
HOMEPAGE="http://www.verlihub-project.org"
SRC_URI="mirror://sourceforge/verlihub/${PN}-${PV}.tar.gz"
SLOT=1
LICENSE="GPL-2"
KEYWORDS="~amd64 ~x86-fbsd ~x86"
DEPEND="net-p2p/verlihub
	=dev-lang/lua-5.0.3"
S=${WORKDIR}/${PN}

src_unpack() {
	unpack "${PN}-${PV}.tar.gz"
	cd "${S}"
}

src_compile() {
        econf || die
        emake || die "Make failed for problems or bugs please visit http://forums.verlihub-project.org/"
}

 src_install() {
         make DESTDIR=${D} install || die
 
 }
 
 pkg_postinst() {
     	einfo "LUA 5.0 is now installed, please run it using !onplug lua or!plugin /usr/local/lib/liblua_pi.so"
        ewarn "Do NOT report bugs to Gentoo's bugzilla"
        einfo "Please report all bugs to http://forums.verlihubproject.org"
        einfo "Verlihub-project team"
}

