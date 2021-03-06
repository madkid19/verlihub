/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
*   devs at verlihub-project dot org                                      *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "cgeoip.h"

namespace nVerliHub {
namespace nUtils {

cGeoIP::cGeoIP(): mGI(GeoIP_new(GEOIP_STANDARD)) {}

bool cGeoIP::GetCC(const string &hostname, string &cc)
{
	const char *country_code;
	country_code = GeoIP_country_code_by_name(mGI, hostname.c_str());

	if (country_code == NULL) {
		cc = "--";
		return false;
	} else {
		cc = country_code;
		return true;
	}
}

bool cGeoIP::GetCN(const string &hostname, string &cn)
{
	const char *country_name;
	country_name = GeoIP_country_name_by_name(mGI, hostname.c_str());

	if (country_name == NULL) {
		cn = "--";
		return false;
	} else {
		cn = country_name;
		return true;
	}
}

cGeoIP::~cGeoIP()
{
	GeoIP_delete(mGI);
}

}; // namespace nUtils
}; // namespace nVerliHub
