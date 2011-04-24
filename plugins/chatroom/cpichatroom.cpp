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

#include "cpichatroom.h"
#include "crooms.h"
#include "src/cserverdc.h"
#include "src/cdcproto.h"

namespace nVerliHub {
	using namespace nConfig;
	namespace nChatRoom {

cpiChatroom::cpiChatroom()
{
	mName = "Chatroom";
	mVersion = CHATROOM_VERSION;
	mCfg = NULL;
}

cpiChatroom::~cpiChatroom()
{
	if (mCfg != NULL) delete mCfg;
	mCfg = NULL;
}

void cpiChatroom::OnLoad(cServerDC *server)
{
	cUserCollection::iterator it;
	cUser *user;

	if (!mCfg) mCfg = new cRoomCfg(server);
	mCfg->Load();
	mCfg->Save();

	tpiChatroomBase::OnLoad(server);
	for (it = mServer->mUserList.begin(); it !=  mServer->mUserList.end() ; ++it) {
		user =(cUser*)*it;
		if(user && user->mxConn)
			mList->AutoJoin(user);
	}
}

bool cpiChatroom::OnUserLogin(cUser *user)
{
	mList->AutoJoin(user);
	return true;
}

bool cpiChatroom::OnUserLogout(cUser *user)
{
	cRooms::iterator it;
	for (it = mList->begin(); it != mList->end(); ++it) {
		if(*it) (*it)->DelUser(user);
	}
	return true;
}

bool cpiChatroom::RegisterAll()
{
	RegisterCallBack("VH_OnUserLogin");
	RegisterCallBack("VH_OnUserLogout");
	RegisterCallBack("VH_OnOperatorCommand");
	return true;
}

bool cpiChatroom::OnUserCommand(cConnDC *conn, string *str)
{
	if(mConsole.DoCommand(*str, conn))
		return false;
	return true;
}

bool cpiChatroom::OnOperatorCommand(cConnDC *conn, string *str)
{
	if(mConsole.DoCommand(*str, conn))
		return false;
	return true;
}

	}; // namespace nChatRoom
}; // namespace nVerliHub
REGISTER_PLUGIN(nVerliHub::nChatRoom::cpiChatroom);
