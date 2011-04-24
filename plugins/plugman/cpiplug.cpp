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

#include "cpiplug.h"
#include "cplugs.h"
#include "cserverdc.h"

namespace nVerliHub {
	using namespace nSocket;
	namespace nPlugMan {

cpiPlug::cpiPlug()
{
	mName = "plugman";
	mVersion = PLUGMAN_VERSION;
}

void cpiPlug::OnLoad(cServerDC *server)
{
	tpiPlugBase::OnLoad(server);
	mList->mPM = &(server->mPluginManager);
	mList->mVHTime = mList->GetFileTime(mServer->mExecPath);
	mList->PluginAll(ePLUG_AUTOLOAD);
}

cpiPlug::~cpiPlug() {}

bool cpiPlug::RegisterAll()
{
	RegisterCallBack("VH_OnOperatorCommand");
	return true;
}

bool cpiPlug::OnOperatorCommand(cConnDC *conn, string *str)
{
	// Redirect command to plugman console
	if(mConsole.DoCommand(*str, conn))
		return false;
	return true;
}
	}; // namespace nPlugMan
}; // namespace nVerliHub

REGISTER_PLUGIN(nVerliHub::nPlugMan::cpiPlug);

