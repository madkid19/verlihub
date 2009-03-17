/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CPIDIA_H
#define CPIDIA_H

#include <cvhplugin.h>
#include <cconndc.h>

using namespace nDirectConnect;
using namespace nDirectConnect::nPlugin;
class cDiaCfg;

/**
a verlihub's diakritica plugin

@author Daniel Muller
*/
class cpiDia : public cVHPlugin
{
public:
	cpiDia();
	virtual ~cpiDia();
	virtual void OnLoad(cServerDC* server);

	virtual bool RegisterAll();
	virtual bool OnParsedMsgChat(cConnDC * conn, cMessageDC *);
	cDiaCfg *mCfg;
	char replace_array[256];
};

#endif
