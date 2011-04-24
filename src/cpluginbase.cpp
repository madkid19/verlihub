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
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#include "cpluginbase.h"
#include "cpluginmanager.h"
namespace nVerliHub {
	namespace nPlugin {

cPluginBase::cPluginBase():cObj("PluginBase")
{
	mIsAlive = true;
	mManager = NULL;
}

cPluginBase::~cPluginBase(){}

int cPluginBase::StrLog(ostream & ostr, int level)
{
	if(cObj::StrLog(ostr,level)) {
		LogStream()   << "(" << mName << ") ";
		return 1;
	}
	return 0;
}

bool cPluginBase::RegisterCallBack(string id)
{
	if(mManager)
		return mManager->RegisterCallBack(id, this);
	else
		return false;
}

bool nPlugin::cPluginBase::UnRegisterCallBack(string id)
{
	if(mManager)
		return mManager->UnregisterCallBack(id, this);
	else
		return false;
}
	}; // namespace nPlugin
}; // namespace nVerliHub