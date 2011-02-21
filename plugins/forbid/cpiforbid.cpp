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

#include "cpiforbid.h"
#include "src/cserverdc.h"

using namespace nDirectConnect;

cpiForbid::cpiForbid() : mCfg(NULL)
{
	mName = "ForbiddenWords";
	mVersion = FORBID_VERSION;
}

void cpiForbid::OnLoad(cServerDC *server)
{
	tpiForbidBase::OnLoad(server);
	mCfg = new cForbidCfg(mServer);
	mCfg->Load();
	mCfg->Save();
}

bool cpiForbid::RegisterAll()
{
	RegisterCallBack("VH_OnOperatorCommand");
	RegisterCallBack("VH_OnParsedMsgChat");
	RegisterCallBack("VH_OnParsedMsgPM");
	return true;
}

cpiForbid::~cpiForbid()
{
	if (mCfg) delete mCfg;
	mCfg = NULL;
}

bool cpiForbid::OnParsedMsgPM(cConnDC *conn, cMessageDC *msg)
{
	string text = msg->ChunkString(eCH_PM_MSG);
	
	cUser *dest = mServer->mUserList.GetUserByNick(msg->ChunkString(eCH_PM_TO));
	if(dest && dest->mxConn && (dest->mClass > mCfg->max_class_dest))
		return true;

	/** Check that the user inputs forbidden words into PM */
	if(mList->ForbiddenParser(text, conn , cForbiddenWorker::eCHECK_PM) == 0)
		return false;

	return true;
}

bool cpiForbid::OnParsedMsgChat(cConnDC *conn, cMessageDC *msg)
{
	string text = msg->ChunkString(eCH_CH_MSG);

	/** Check that the user inputs forbidden words into GLOBAL*/
	if(mList->ForbiddenParser(text, conn , cForbiddenWorker::eCHECK_CHAT) == 0)
		return false;
	
	if(conn->mpUser->mClass < eUC_OPERATOR)
	{
		/** Check repeating characters */
		if((mCfg->max_repeat_char != 0) && (!mList->CheckRepeat(text, mCfg->max_repeat_char)))
		{
			/** send only to the bitch */
			mServer->DCPublic(conn->mpUser->mNick,text,conn);
			return false;
		}
		
		if(!mList->CheckUppercasePercent(text, mCfg->max_upcase_percent))
		{
			/** send only to the bitch */
			mServer->DCPublic(conn->mpUser->mNick,text,conn);
			return false;
		}
	}
	return true;
}

bool cpiForbid::OnOperatorCommand(cConnDC *conn, string *str)
{
	if( mConsole.DoCommand(*str, conn) ) return false;
	return true;
}

REGISTER_PLUGIN(cpiForbid);
