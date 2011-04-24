/***************************************************************************
 *   Copyright (C) 2004 by Janos Horvath                                   *
 *   bourne@freemail.hu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "cpifloodprot.h"

namespace nVerliHub {
	using namespace nSocket;
	using namespace nEnums;
	namespace nFloodProtectPlugin {
cpiFloodprot::cpiFloodprot() : mFloodprotect(0)
{
	mName = "Floodprotect";
	mVersion ="1.6";
}

cpiFloodprot::~cpiFloodprot()
{
	if(mFloodprotect)
	{
		delete mFloodprotect;
		mFloodprotect = 0;
	}
}

void cpiFloodprot::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);
	mFloodprotect = new cFloodprotect(server);
}

bool cpiFloodprot::RegisterAll()
{
	RegisterCallBack("VH_OnTimer");
	RegisterCallBack("VH_OnNewConn");
	RegisterCallBack("VH_OnUserLogin");
	RegisterCallBack("VH_OnUserLogout");
	RegisterCallBack("VH_OnCloseConn");
	RegisterCallBack("VH_OnParsedMsgChat");
	RegisterCallBack("VH_OnParsedMsgPM");
	RegisterCallBack("VH_OnParsedMsgSearch");
	RegisterCallBack("VH_OnParsedMsgMyINFO");
	return true;
}

bool cpiFloodprot::OnNewConn(cConnDC *conn)
{
	if(!mFloodprotect->AddConn(conn, 1))
	{
		string omsg("Sorry, the limit of connections with your ip has been exceeded.");
		conn->Send(omsg,true);
		conn->CloseNice(500); // not sure if this is needed
		return false;
	}
	return true;
}

bool cpiFloodprot::OnUserLogin(cUser *user)
{
	if(!mFloodprotect->AddConn(user->mxConn, 1))
	{
		string omsg("Sorry, the limit of unregistered connections with your ip has been exceeded.");
		user->mxConn->Send(omsg,true);
		user->mxConn->CloseNice(500); // not sure if this is needed
		return false;
	}
	return true;
}

bool cpiFloodprot::OnUserLogout(cUser *user)
{
	mFloodprotect->AddConn(user->mxConn, -1);
	return true;
}

bool cpiFloodprot::OnCloseConn(cConnDC *conn)
{
	mFloodprotect->AddConn(conn, -1);
	return true;
}


bool cpiFloodprot::OnTimer()
{
	if(!mFloodprotect->CleanUp(30))
	{
		return false;
	}

	return true;
}

bool cpiFloodprot::OnParsedMsgChat(cConnDC *conn, cMessageDC *msg)
{
	if(!mFloodprotect->CheckFlood(conn, eFT_CHAT))
		return false;

	return true;
}

bool cpiFloodprot::OnParsedMsgPM(cConnDC *conn, cMessageDC *msg)
{
	if(!mFloodprotect->CheckFlood(conn, eFT_PRIVATE))
		return false;

	return true;
}

bool cpiFloodprot::OnParsedMsgSearch(cConnDC *conn, cMessageDC *msg)
{
	if(!mFloodprotect->CheckFlood(conn, eFT_SEARCH))
		return false;

	return true;
}

bool cpiFloodprot::OnParsedMsgMyINFO(cConnDC *conn, cMessageDC *msg)
{
	if(!mFloodprotect->CheckFlood(conn, eFT_MYINFO))
		return false;

	return true;
}
	}; // namespace nFloodProtectPlugin
}; // namespace nVerliHub
REGISTER_PLUGIN(nVerliHub::nFloodProtectPlugin::cpiFloodprot);
