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

#include "cpimessanger.h"
#include "cmsglist.h"
#include "src/cserverdc.h"
#include "src/cdcproto.h"


using namespace ;

cpiMessanger::cpiMessanger() : mConsole(this), mMsgs(NULL), mReloadTimer(30., 0., cTime())
{
	mName = "Messanger";
	mVersion = MESSENGER_VERSION;
}

bool cpiMessanger::OnTimer()
{
	if (mReloadTimer.Check(mServer->mTime, 1)==0) {
		mMsgs->UpdateCache();
	}
	return true;
}


bool cpiMessanger::OnUserLogin(cUser *user)
{
	int iNewMsgs = mMsgs->CountMessages(user->mNick, false);
	if (iNewMsgs) {
		mMsgs->DeliverMessagesForUser(user);
	}
	return true;
}

void cpiMessanger::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);
	mMsgs = new cMsgList(server);
	mMsgs->CreateTable();
	mMsgs->CleanUp();
}

bool cpiMessanger::RegisterAll()
{
	//When user logins checkout his mailbox and eventualy tell him about messages
	//RegisterCallBack("VH_OnNewUser");
	//treat messages that use to post offline msgs, remove them, read etc...
	RegisterCallBack("VH_OnUserCommand");
	RegisterCallBack("VH_OnUserLogin");
	RegisterCallBack("VH_OnTimer");
	return true;
}

cpiMessanger::~cpiMessanger()
{
	if (mMsgs) delete mMsgs;
}

bool cpiMessanger::OnUserCommand(cConnDC *conn, string *str)
{
	if( mConsole.DoCommand(*str, conn) ) return false;
	return true;
}

REGISTER_PLUGIN(cpiMessanger);

