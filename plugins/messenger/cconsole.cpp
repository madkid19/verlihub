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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "cconsole.h"
#include "cpimessanger.h"
#include "cmsglist.h"

using namespace nDirectConnect;

namespace nMessanger
{

cConsole::cConsole(cpiMessanger *msn) :
	mMessanger(msn),
	mCmdMsgSend(0,"\\+msgsend ", "(\\S+)([^\\m\\n\\-]*)?(\\m?\\n|--)(.*)", &mcfMsgSend),
	mCmdMsgRead(1,"\\+msgread", "", &mcfMsgRead),
	mCmdr(this)
{
	mCmdr.Add(&mCmdMsgSend);
	mCmdr.Add(&mCmdMsgRead);
}


cConsole::~cConsole()
{}

int cConsole::DoCommand(const string &str, cConnDC * conn)
{
	ostringstream os;
	if(mCmdr.ParseAll(str, os, conn) >= 0)
	{
		mMessanger->mServer->DCPublicHS(os.str().data(),conn);
		return 1;
	}
	return 0;
}

bool cConsole::cfMessageSend::operator ( )()
{
	sMessage msg;
	cUser * receiver;
	msg.mSender = ((cConnDC*)this->mExtra)->mpUser->mNick;
	msg.mDateSent = cTime().Sec();
	msg.mDateExpires = msg.mDateSent + 7 * 24* 3600;
	msg.mSenderIP = ((cConnDC*)this->mExtra)->AddrIP();
	this->GetParStr(1,msg.mReceiver);
	this->GetParStr(2,msg.mSubject);
	this->GetParStr(4,msg.mBody);
	
	receiver = GetMessanger()->mServer->mUserList.GetUserByNick(msg.mReceiver);
	if ((receiver != NULL) && (receiver->mxConn != NULL))
	{
		GetMessanger()->mMsgs->DeliverOnline(receiver, msg);
		(*mOS) << msg.mReceiver << " is online, sending directly...";
	}
	else 
	{
		GetMessanger()->mMsgs->AddMessage(msg);
		(*mOS) << "Message saved.";
	}
	return true;
}

bool cConsole::cfMessageRead::operator ( )()
{
	if (this->GetMessanger()->mMsgs->CountMessages(((cConnDC*) this->mExtra)->mpUser->mNick, false)) 
	{
		(*mOS) << "You have some messages.\r\n";
		this->GetMessanger()->mMsgs->PrintSubjects(*mOS, ((cConnDC*) this->mExtra)->mpUser->mNick, false);
	}
	else (*mOS) << "You have no new messages.";

	return true;
}

};
