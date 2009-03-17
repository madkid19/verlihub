/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cvhpluginmgr.h"
#include "cvhplugin.h"

namespace nDirectConnect {

namespace nPlugin {

cVHPluginMgr::cVHPluginMgr(cServerDC *server,const string pluginDir):
	cPluginManager(pluginDir), mServer(server)
{
	SetClassName("cVHPluginMgr");
	if (Log(0)) LogStream() << "using plugins in: " << pluginDir << endl;
	cout << "------------------------" << endl;
}

cVHPluginMgr::~cVHPluginMgr(){}
};

};


/*!
    \fn nDirectConnect::nPlugin::cVHPluginMgr::OnPluginLoad(cPluginBase *pi)
 */
void nDirectConnect::nPlugin::cVHPluginMgr::OnPluginLoad(cPluginBase *pi)
{
	if (Log(0)) LogStream() << "OnPluginLoad: " << pi->Name() << endl;
	((cVHPlugin *)pi)->OnLoad(mServer);
}
