/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CPITEST_H
#define CPITEST_H

#include <cvhplugin.h>
#include <cconndc.h>

using namespace nDirectConnect;
using namespace nDirectConnect::nPlugin;

/**
a verlihub's test  plugin

@author Daniel Muller
*/
class cpiTest : public cVHPlugin
{
public:
	cpiTest();
	virtual ~cpiTest();

	virtual void OnLoad(cServerDC* server);
	virtual bool RegisterAll();
	virtual bool OnNewConn(cConnDC * conn);
	virtual bool OnCloseConn(cConnDC * conn);
};

#endif
