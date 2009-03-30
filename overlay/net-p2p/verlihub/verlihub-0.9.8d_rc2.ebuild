# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="Verlihub, a linux hub for the p2p program 'direct connect'"
HOMEPAGE="http://www.verlihub-project.org"
SRC_URI="http://umn.dl.sourceforge.net/sourceforge/verlihub/${PN}-0.9.8d-RC2.tar.gz"
LICENSE="GPL-2"
SLOT="0"
KEYWORDS="amd64 x86-fbsd x86"
DEPEND="dev-libs/libpcre
        dev-libs/geoip
        >=dev-db/mysql-4.0.20
	sys-libs/zlib"
IUSE="lua50 lua51 floodprot iplog forbid funny messanger chatroom isp replacer stats p_perl hublink"
PDEPEND=" lua50? ( net-libs/lua50 )
	  lua51? ( net-libs/lua51 )
	  floodport? ( net-libs/floodprot )
  	  iplog? ( net-libs/iplog )
	  forbid? ( net-libs/forbid )
	  funny? ( net-libs/funny )
	  messanger? ( net-libs/messanger )
	  chatroom? ( net-libs/chatroom )
	  isp? ( net-libs/isp ) 
	  replacer? ( net-libs/replacer ) 
	  stats? ( net-libs/stats ) 
	  p_perl? ( net-libs/perl ) 
	  hublink? ( net-libs/hublink )"

S=${WORKDIR}/${PN}

src_unpack() {
	unpack "${PN}-0.9.8d-RC2.tar.gz"
	cd "${S}"
}
src_compile() {
        econf || die
        emake || die "Make failed for problems or bugs please visit http://forums.verlihub-project.org/"
}

src_install() {
        make DESTDIR=${D} install || die

        dohtml docs/*.html

        docinto "scripts"

        dodoc \
                scripts/ccgraph \
                scripts/import_reglist_0.9.x_to_0.9.8b.sql  \
                scripts/install \
                scripts/regnick \
                scripts/runhub  \
                scripts/setenv  \
                scripts/trigger \
                scripts/vh_runhub.in

        docinto ""
        dodoc \
                AUTHORS \
                COPYING \
                ChangeLog \
                INSTALL \
                NEWS \
                README \
                TODO \
                docs/configuring.txt \
                docs/help \
                docs/help.sql \
                docs/ascii \
                docs/params.php \
                docs/using.txt
}

pkg_postinst() {
    echo
    ewarn "Do NOT report bugs to Gentoo's bugzilla"
    einfo "Please report all bugs to http://forums.verlihub-project.org"
    einfo "Verlihub-project team"

    if [[ -f "/etc/verlihub/dbconfig" ]]
    then
	einfo "Verlihub is already configured in /etc/verlihub"
    else
	ewarn "You MUST configure verlihub before starting it:"
	ewarn "emerge --config =${CATEGORY}/${PF}"
	ewarn "That way you can [re]configure your verlihub setup"
    fi
}

pkg_config() {
    ewarn "Configuring verlihub"
    /usr/bin/vh_install
    if [[ -f "/etc/verlihub/dbconfig" ]]
    then
	ewarn "Configuration completed"
    else
	ewarn "You haven't configured verlihub succesfully. Please try again"
    fi
}
