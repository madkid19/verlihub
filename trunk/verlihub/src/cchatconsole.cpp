/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cchatconsole.h"
#include "cserverdc.h"
#include "cuser.h"

namespace nDirectConnect
{

cChatConsole::cChatConsole(cServerDC *server, cChatRoom *ChatRoom) :
	cDCConsoleBase(server),
	mCmdr(this),
	mChatRoom(ChatRoom)
{}


cChatConsole::~cChatConsole()
{}

cUserCollection *cChatConsole::GetTheList()
{
	return mChatRoom->mCol;
}

void cChatConsole::AddCommands()
{
	mCmdInvite.Init(eCHAT_INVITE, CmdId(eCHAT_INVITE), GetParamsRegex(eCHAT_INVITE), &mcfInvite);
	mCmdLeave.Init(eCHAT_LEAVE, CmdId(eCHAT_LEAVE), GetParamsRegex(eCHAT_LEAVE), &mcfLeave);
	mCmdOut.Init(eCHAT_OUT, CmdId(eCHAT_OUT), GetParamsRegex(eCHAT_OUT), &mcfOut);
	mCmdMembers.Init(eCHAT_MEMBERS, CmdId(eCHAT_MEMBERS), GetParamsRegex(eCHAT_MEMBERS), &mcfMembers);
	mCmdr.Add(&mCmdInvite);
	mCmdr.Add(&mCmdLeave);
	mCmdr.Add(&mCmdOut);
	mCmdr.Add(&mCmdMembers);
	mCmdr.InitAll(this);
}

int cChatConsole::DoCommand(const string &str, cConnDC * conn)
{
	ostringstream os;
	if (!conn || !conn->mpUser) return 0;
	if(mCmdr.ParseAll(str, os, conn) >= 0)
	{
		mChatRoom->SendPMTo(conn, os.str());
		return 1;
	}
	return 0;
}

const char *cChatConsole::CmdId(int cmd)
{
	static string id;
	id = CmdPrefix();
	switch(cmd)
	{
		case eCHAT_INVITE: id += "invite"; break;
		case eCHAT_LEAVE: id += "leave"; break;
		case eCHAT_OUT: id += "out"; break;
		case eCHAT_MEMBERS: id += "members"; break;
		default: id += "???";
	}
	id += CmdSuffix();

	switch(cmd)
	{
		case eCHAT_LEAVE: break;
		case eCHAT_MEMBERS: break;
		default :id += " ";
	}

	return id.c_str();
}

const char * cChatConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		// +invite <nick>[ <by these words>]
		case eCHAT_INVITE: return "^(\\S+)( (.*))?$"; break;
		case eCHAT_OUT: return "^(\\S+)( (.*))?$"; break;
		default: return "";
	}
}

cUserCollection *cChatConsole::cfBase::GetTheList()
{
	if (mCommand && mCommand->mCmdr && mCommand->mCmdr->mOwner)
		return ((cChatConsole*) mCommand->mCmdr->mOwner)->GetTheList();
	return NULL;
}

bool cChatConsole::cfOut::operator()()
{
	string nick, msg;
	cUser *user;

	GetParOnlineUser(1, user,nick);
	if(!user ||!user->mxConn || !GetTheList()->ContainsNick(nick))
	{
		*mOS << "User '" << nick << "' is not in this room.";
		return false;
	}
	if (user->mClass > mConn->mpUser->mClass)
	{
		*mOS << "You are not higher than " << nick;
		return false;
	}

	GetParStr(3, msg);

	GetTheList()->Remove(user);
	return true;
}

bool cChatConsole::cfLeave::operator()()
{
	if (mConn && mConn->mpUser)
	{
		GetTheList()->Remove(mConn->mpUser);
		return true;
	} else return false;
}

bool cChatConsole::cfMembers::operator()()
{
	string NickList;
	if (mConn && mConn->mpUser)
	{
		NickList = GetTheList()->GetNickList();
		*mOS << "Members: \r\n" << NickList;
		return true;
	} else return false;
}

bool cChatConsole::cfInvite::operator()()
{
	string nick, msg;
	cUser *user;

	GetParOnlineUser(1, user,nick);
	if(!user ||!user->mxConn)
	{
		*mOS << "User '" << nick << "' is not online, so you cannot invite him.";
		return false;
	}
	GetParStr(3, msg);

	GetTheList()->Add(user);
	return true;
}

};
