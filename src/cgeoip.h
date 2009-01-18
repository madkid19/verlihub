/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NUTILSCGEOIP_H
#define NUTILSCGEOIP_H

#include <GeoIP.h>
#include <string>
using std::string;

namespace nUtils {

/**
class is able to convert ip address to country code

@author Daniel Muller
*/
class cGeoIP{
public:
	cGeoIP();
	~cGeoIP();
	bool GetCC(const string &hostname, string &CC);
private:
	GeoIP * mGI;
};

};

#endif
