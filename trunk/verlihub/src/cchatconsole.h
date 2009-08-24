/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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
#ifndef NDIRECTCONNECTCCHATCONSOLE_H
#define NDIRECTCONNECTCCHATCONSOLE_H
#include "cdccommand.h"
#include "ccmdr.h"

using namespace nCmdr;

namespace nDirectConnect
{

class cConnDC;
class cChatRoom;
class cUserCollection;

/**
contains the commands that are accepted thourh the pm of any chatroom, like for example the OpChat

@author Daniel Muller
*/
class cChatConsole : public cDCConsoleBase
{
public:
	cChatConsole(cServerDC *, cChatRoom *ChatRoom = NULL);
	virtual ~cChatConsole();

	virtual void AddCommands();
	virtual int DoCommand(const string &str, cConnDC *conn);
	virtual int OpCommand(const string &str, cConnDC *conn){ return this->DoCommand(str,conn);}
	virtual int UsrCommand(const string &str , cConnDC *conn){ return this->DoCommand(str,conn);}

	virtual cUserCollection *GetTheList();
	virtual const char * GetParamsRegex(int);
	virtual const char *CmdSuffix() {return "";}
	virtual const char *CmdPrefix() {return "\\+";}
	virtual const char *CmdId(int cmd);

	// commands id's
	enum { eCHAT_INVITE, eCHAT_LEAVE, eCHAT_OUT, eCHAT_MEMBERS };

protected:

	class cfBase : public cDCCommand::sDCCmdFunc
	{
		public:
		virtual cUserCollection *GetTheList();
	};

	class cfInvite : public cfBase { virtual bool operator()(); } mcfInvite;
	class cfLeave : public cfBase { virtual bool operator()(); } mcfLeave;
	class cfOut : public cfBase { virtual bool operator()(); } mcfOut;
	class cfMembers : public cfBase { virtual bool operator()(); } mcfMembers;

	cDCCommand mCmdInvite;
	cDCCommand mCmdLeave;
	cDCCommand mCmdOut;
	cDCCommand mCmdMembers;

	cCmdr mCmdr;
	cChatRoom *mChatRoom;
};

};

#endif
