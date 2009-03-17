/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cgeoip.h"

namespace nUtils {

cGeoIP::cGeoIP()
{
// 	const char * hostname;
// const char * country_code;
// const char * country_name;
// GeoIPRegion * region;
// GeoIPRecord * gir;
// const char * org;
// char * db_info;
	mGI = GeoIP_new(GEOIP_STANDARD);
}

bool cGeoIP::GetCC(const string &hostname, string &CC)
{
	const char * country_code;
	country_code = GeoIP_country_code_by_name(mGI, hostname.c_str());
	if (country_code == NULL) {
		CC = "--";
		return false;
	} else {
		CC=country_code;
		return true;
	}
}

cGeoIP::~cGeoIP()
{
	GeoIP_delete(mGI);
}


};
