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
#ifndef CROOMS_H
#define CROOMS_H

#include "src/tlistplugin.h"
#include "src/cuser.h"
#include "src/cusercollection.h"

using namespace nConfig;
namespace nDirectConnect { class cUser; class cServerDC; };
using namespace nDirectConnect;
class cpiChatroom;

class cRoom;

class cXChatRoom : public nDirectConnect::cChatRoom::cChatRoom
{
public:
	cXChatRoom(const string &nick, cRoom *room);
	virtual bool IsUserAllowed(cUser *);
	cRoom *mRoom;
};

class cpiChatroom;

class cRoom
{
public:
	// -- stored data
	string mNick;
	string mTopic;
	string mCreator;
	string mAutoCC;
	int mMinClass;
	int mAutoClassMin;
	int mAutoClassMax;

	// -- memory data
	cChatRoom  *mChatRoom;
	cUserCollection *mUsers;
	cServerDC *mServer;
	cpiChatroom *mPlugin;
	
	// -- required methods
	cRoom();
	virtual ~cRoom();
	virtual void OnLoad();
	virtual void AddUser(cUser *);
	virtual void DelUser(cUser *);
	virtual bool IsUserAutoJoin(cUser *);
	friend ostream& operator << (ostream &, const cRoom &room);
};

class cRoomCfg : public cConfigBase
{
public:
	cRoomCfg(cServerDC *);
	int min_class_add;
	int min_class_mod;
	int min_class_del;
	int min_class_lst;
	
	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};


typedef class tList4Plugin<cRoom, cpiChatroom> tRoomListBase;

/**
@author Rongic
@author Daniel Muller
*/
class cRooms : public tRoomListBase
{
public:
	// -- usable methods
	void AutoJoin(cUser *user);
	
	// -- required methods
	cRooms(cVHPlugin *pi);
	virtual ~cRooms();
	virtual void AddFields();
	virtual void OnLoadData(cRoom &);
	virtual bool CompareDataKey(const cRoom &D1, const cRoom &D2);
};

#endif
