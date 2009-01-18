/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#include "cpluginbase.h"
#include "cpluginmanager.h"

namespace nPlugin {

cPluginBase::cPluginBase():cObj("PluginBase")
{
	mIsAlive = true;
	mManager = NULL;
}

cPluginBase::~cPluginBase(){}

};

int nPlugin::cPluginBase::StrLog(ostream & ostr, int level)
{
	if(cObj::StrLog(ostr,level))
	{
		LogStream()   << "(" << mName << ") ";
		return 1;
	}
	return 0;
}

bool nPlugin::cPluginBase::RegisterCallBack(string id)
{
	if(mManager) return mManager->RegisterCallBack(id, this);
	else return false;
}

bool nPlugin::cPluginBase::UnRegisterCallBack(string id)
{
	if(mManager) return mManager->UnregisterCallBack(id, this);
	else return false;
}
