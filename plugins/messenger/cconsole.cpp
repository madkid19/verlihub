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

#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "src/i18n.h"
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
	if(mCmdr.ParseAll(str, os, conn) >= 0) {
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
	if ((receiver != NULL) && (receiver->mxConn != NULL)) {
		GetMessanger()->mMsgs->DeliverOnline(receiver, msg);
		(*mOS) << autosprintf(_("Sending message to %s because he is online."), msg.mReceiver.c_str());
	} else  {
		GetMessanger()->mMsgs->AddMessage(msg);
		(*mOS) << _("Message saved.");
	}
	return true;
}

bool cConsole::cfMessageRead::operator ( )()
{
	int messages = this->GetMessanger()->mMsgs->CountMessages(((cConnDC*) this->mExtra)->mpUser->mNick, false);
	if (messages) {
		(*mOS) << autosprintf(ngettext("You have %d message in your box.", "You have %d messages in your box.", messages), messages) << "\r\n";
		this->GetMessanger()->mMsgs->PrintSubjects(*mOS, ((cConnDC*) this->mExtra)->mpUser->mNick, false);
	}
	else
		(*mOS) << _("You have no new message in your box.");
	return true;
}

};
