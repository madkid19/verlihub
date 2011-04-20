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

#include "cpiiplog.h"
#include "src/cserverdc.h"

using namespace ;

cpiIPLog::cpiIPLog() :  mConsole(this),	mIPLog(NULL)
{
	mName = "IPLog";
	mVersion = IPLOG_VERSION;
	mLogFlags = eLT_ALL;
}

void cpiIPLog::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);
	mServer = server;
	mIPLog = new cIPLog(server);
	mIPLog->CreateTable();
}

bool cpiIPLog::RegisterAll()
{
	RegisterCallBack("VH_OnOperatorCommand");
        RegisterCallBack("VH_OnNewConn");
	RegisterCallBack("VH_OnCloseConn");
	RegisterCallBack("VH_OnUserLogin");
	RegisterCallBack("VH_OnUserLogout");
	return true;
}
	
bool cpiIPLog::OnCloseConn(cConnDC *conn)
{
	if ((1 << eLT_DISCONNECT) & mLogFlags) 
		mIPLog->Log(conn, eLT_DISCONNECT, conn->mCloseReason);
	return true;
}

bool cpiIPLog::OnNewConn(cConnDC * conn)
{
	if ((1 << eLT_CONNECT) & mLogFlags) 
		mIPLog->Log(conn, eLT_CONNECT, 0);
	return true;
}

bool cpiIPLog::OnUserLogout(cUser *user)
{
	if (user->mxConn && ((1 << eLT_LOGOUT) & mLogFlags)) 
		mIPLog->Log(user->mxConn, eLT_LOGOUT, user->mxConn->mCloseReason);
	return true;
}

bool cpiIPLog::OnUserLogin(cUser *user)
{
	if (user->mxConn && ((1 << eLT_LOGIN) & mLogFlags)) 
		mIPLog->Log(user->mxConn, eLT_LOGIN, 0);
	return true;
}

bool cpiIPLog::OnOperatorCommand(cConnDC *conn, string *str)
{
        if( mConsole.DoCommand(*str, conn) ) return false;
        return true;
}
		
cpiIPLog::~cpiIPLog()
{
	if (mIPLog)
		delete mIPLog;
	mIPLog = NULL;
}

REGISTER_PLUGIN(cpiIPLog);

