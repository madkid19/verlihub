/***************************************************************************
 *   Copyright (C) 2004 by Daniel Muller                                   *
 *   dan at verliba dot cz                                                 *
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

using namespace nDirectConnect;

cpiPlug::cpiPlug()
{
	mName = "plugman";
	mVersion = "1.0";
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
	RegisterCallBack("VH_OnNewConn");
	RegisterCallBack("VH_OnOperatorCommand");
}

bool cpiPlug::OnNewConn(cConnDC * conn)
{
	if (mServer->mC.host_header ==1) {
	mServer->DCPublicHS("This hub is enhanced by " + mName + " for Verlihub.", conn);
	}
	return true; 
}

bool cpiPlug::OnOperatorCommand(cConnDC *conn, string *str)
{
	if( mConsole.DoCommand(*str, conn) ) return false;
	return true;
}

REGISTER_PLUGIN(cpiPlug);

