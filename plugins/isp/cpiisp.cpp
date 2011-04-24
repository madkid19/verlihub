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
#include "cpiisp.h"
#include <stringutils.h>

namespace nVerliHub {
	using namespace nUtils;
	using namespace nEnums;
	using namespace nSocket;
	namespace nIspPlugin {
cpiISP::cpiISP()
{
	mName = "ISP";
	mVersion = ISP_VERSION;
	mCfg= NULL;
}

void cpiISP::OnLoad(cServerDC *server)
{
	if (!mCfg) mCfg = new cISPCfg(server);
	mCfg->Load();
	mCfg->Save();
	tpiISPBase::OnLoad(server);
}

cpiISP::~cpiISP() {
	if (mCfg != NULL) delete mCfg;
	mCfg = NULL;
}

bool cpiISP::RegisterAll()
{
	RegisterCallBack("VH_OnParsedMsgMyINFO");
	RegisterCallBack("VH_OnParsedMsgValidateNick");
	RegisterCallBack("VH_OnOperatorCommand");
	return true;
}

bool cpiISP::OnParsedMsgMyINFO(cConnDC * conn, cMessageDC *msg)
{
	cISP *isp;
	if (conn->mpUser && (conn->GetTheoricalClass() <= mCfg->max_check_isp_class))
	{
		isp = mList->FindISP(conn->AddrIP(), conn->mCC);
		if (!isp)
		{
			if (!mCfg->allow_all_connections)
			{
				// no results;
				mServer->DCPublicHS(mCfg->msg_no_isp, conn);
				conn->CloseNice(500);
				return false;
			} else return true;
		}


		if (!conn->mpUser->mInList)
		{
			if (conn->GetTheoricalClass() <= mCfg->max_check_conn_class)
			{
				if (!isp->CheckConn(msg->ChunkString(eCH_MI_SPEED)))
				{
					string omsg = isp->mConnMessage;
					string pattern;
					cDCProto::EscapeChars(isp->mConnPattern, pattern);
					ReplaceVarInString(omsg,"pattern",omsg,pattern);
					mServer->DCPublicHS(omsg, conn);
					conn->CloseNice(500);
					return false;
				}
			}

			int share_sign = isp->CheckShare(conn->GetTheoricalClass(), conn->mpUser->mShare, mCfg->unit_min_share_bytes, mCfg->unit_max_share_bytes);
			if (share_sign)
			{
				mServer->DCPublicHS((share_sign>0)?mCfg->msg_share_more:mCfg->msg_share_less, conn);
				conn->CloseNice(500);
				return false;
			}
		}

		if (conn->GetTheoricalClass() <= mCfg->max_insert_desc_class)
		{
			string &desc = msg->ChunkString(eCH_MI_DESC);
			string desc_prefix;
			if (isp->mAddDescPrefix.length() > 0)
			{
				ReplaceVarInString(isp->mAddDescPrefix,"CC",desc_prefix,conn->mCC);
				ReplaceVarInString(desc_prefix,"CLASS",desc_prefix,conn->GetTheoricalClass());
				desc = desc_prefix + desc;
				msg->ApplyChunk(eCH_MI_DESC);
			}
		}
	}
	return true;
}

bool cpiISP::OnParsedMsgValidateNick(cConnDC * conn, cMessageDC *msg)
{
	cISP *isp;
	if (conn->GetTheoricalClass() <= mCfg->max_check_nick_class)
	{
		string & nick = msg->ChunkString(eCH_1_PARAM);
		isp = mList->FindISP(conn->AddrIP(),conn->mCC);
		// TEST cout << "Checking nick: " << nick << endl;
		if (isp && !isp->CheckNick(nick, conn->mCC))
		{
			string omsg;
			//cout << "ISP - got a wrong nick" << endl;
			ReplaceVarInString(isp->mPatternMessage, "pattern", omsg, isp->mNickPattern);
			ReplaceVarInString(omsg, "nick", omsg, nick);
			ReplaceVarInString(omsg, "CC", omsg, conn->mCC);

			mServer->DCPublicHS(omsg,conn);
			conn->CloseNice(500);
			return false;
		}
		//mServer->DCPublicHS(isp->mName, conn);
	}
	return true;
}

bool cpiISP::OnOperatorCommand(cConnDC *conn, string *str)
{
	if( mConsole.DoCommand(*str, conn) ) return false;
	return true;
}
	}; // namespace nIspPlugin
}; // namespace nVerliHub

REGISTER_PLUGIN(nVerliHub::nIspPlugin::cpiISP);

