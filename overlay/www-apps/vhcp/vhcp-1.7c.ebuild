# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

inherit webapp eutils depend.php

DESCRIPTION="Verlihub Contro Panel is a tool for the administration of your hubsoft VerliHub"
HOMEPAGE="http://vhcp.verlihub-project.org/"
SRC_URI="mirror://sourceforge/vhcp/${PN}_v${PV}.tar.gz"
LICENSE="GPL-2"
KEYWORDS="amd64 hppa ppc sparc x86"
IUSE=""
RDEPEND=">=net-www/apache-2.0.58-r2
         >=dev-lang/php-5.1.6-r11
         >=dev-db/mysql-3.23.23"

S="${WORKDIR}/${PN}"

need_php

pkg_setup() {
	webapp_pkg_setup

	require_php_with_any_use mysql mysqli
	require_php_with_use pcre gdbm session zip bzip2 gd
}

src_install() {

	webapp_src_preinst

	einfo "Installing main files"
	cp -r * ${D}${MY_HTDOCSDIR}
	einfo "Done"


	webapp_serverowned ${MY_HTDOCSDIR}/index.php

	webapp_serverowned ${MY_HTDOCSDIR}
	webapp_src_install
}
