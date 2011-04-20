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
#include "cuser.h"
#include "cdcproto.h"
#include "cchatconsole.h"
#include "cserverdc.h"
#include "i18n.h"
#include "stringutils.h"

#define PADDING 25

namespace nVerliHub {
	using namespace nProtocol;
	using namespace nUtils;
	using namespace nEnums;

cTime user_global_time;

cUserBase::cUserBase() :
	cObj((const char *)"User"),
	mClass(eUC_NORMUSER),
	mInList(false)
{}

cUserBase::~cUserBase() {}

cUserBase::cUserBase(const string &nick) :
	cObj((const char *)"User"),
	mNick(nick),
	mClass(eUC_NORMUSER),
	mInList(false)
{}

bool cUserBase::CanSend()
{
	return false;
}

void cUserBase::Send(string &data, bool, bool)
{}

cUser::cUser() :
	mShare(0),
	mSearchNumber(0),
	mFloodPM(0.00,30.,10,user_global_time.Get())
{
	mxConn = NULL;
	mxServer = NULL;
	SetRight( eUR_CHAT,0, true );
	SetRight( eUR_PM,0, true );
	SetRight( eUR_SEARCH, 0, true );
	SetRight( eUR_CTM, 0, true );
	SetRight( eUR_KICK, 0, false );
	SetRight( eUR_REG, 0, false );
	SetRight( eUR_OPCHAT, 0, false );
	SetRight( eUR_DROP, 0, false );
	SetRight( eUR_TBAN, 0, false );
	SetRight( eUR_PBAN, 0, false );
	SetRight( eUR_NOSHARE, 0, false );
	mProtectFrom = 0;
	mHideKick = false;
	mHideShare = false;
	IsPassive = false;
	memset(mFloodHashes, 0 ,sizeof(mFloodHashes));
	memset(mFloodCounters, 0 ,sizeof(mFloodCounters));
}

/** Constructor */
cUser::cUser(const string &nick) :
	cUserBase( nick ),
	mxConn(NULL),
	mxServer(NULL),
	mShare(0),
	mSearchNumber(0),
	mHideKicksForClass(eUC_NORMUSER),
	mFloodPM(0.00,30.,30,user_global_time.Get())
{
	SetClassName("cUser");
	IsPassive = false;
	mRights = 0;
	mToBan = false;
	mVisibleClassMin = eUC_NORMUSER;
	mOpClassMin = eUC_NORMUSER;
	SetRight( eUR_CHAT, 0, true );
	SetRight( eUR_PM, 0, true );
	SetRight( eUR_SEARCH, 0, true );
	SetRight( eUR_CTM, 0, true );
	SetRight( eUR_KICK, 0, false );
	SetRight( eUR_REG, 0, false );
	SetRight( eUR_OPCHAT, 0, false );
	SetRight( eUR_DROP, 0, false );
	SetRight( eUR_TBAN, 0, false );
	SetRight( eUR_PBAN, 0, false );
	SetRight( eUR_NOSHARE, 0, false );
	mProtectFrom = 0;
	mHideKick = false;
	mHideShare = false;
	memset(mFloodHashes, 0 , sizeof(mFloodHashes));
	memset(mFloodCounters, 0 ,sizeof(mFloodCounters));
}

cUser::~cUser() {}

bool cUser::CanSend()
{
	return mInList && mxConn && mxConn->ok;
}

void cUser::Send(string &data, bool pipe, bool flush)
{
	mxConn->Send(data, pipe, flush);
}

/** return true if user needs a password and the password is correct */
bool cUser::CheckPwd(const string &pwd)
{
	if(!mxConn || !mxConn->mRegInfo)
		return false;
	return mxConn->mRegInfo->PWVerify(pwd);
}

/** perform a registration: set class, rights etc... precondition: password was al right */
void cUser::Register()
{
	if(!mxConn || !mxConn->mRegInfo)
		return;
	if(mxConn->mRegInfo->mPwdChange)
		return;
	mClass = (tUserCl)mxConn->mRegInfo->mClass;
	mProtectFrom = mxConn->mRegInfo->mClassProtect;
	mHideKicksForClass = mxConn->mRegInfo->mClassHideKick;
	mHideKick = mxConn->mRegInfo->mHideKick;
	mHideShare = mxConn->mRegInfo->mHideShare;
	if (mClass == eUC_PINGER) {
		SetRight( eUR_CHAT, 0 , false );
		SetRight( eUR_PM, 0, false );
		SetRight( eUR_SEARCH, 0, false );
		SetRight( eUR_CTM, 0, false );
		SetRight( eUR_KICK, 0, false );
		SetRight( eUR_REG, 0, false );
		SetRight( eUR_OPCHAT, 0, false );
		SetRight( eUR_DROP, 0, false );
		SetRight( eUR_TBAN, 0, false );
		SetRight( eUR_PBAN, 0, false );
		SetRight( eUR_NOSHARE, 0, true );
	}
}

};

/*!
    \fn nDirectConnect::cUser::ShareEnthropy()
 */
long nDirectConnect::cUser::ShareEnthropy(const string &sharesize)
{
	char diff[20];
	int count[20];

	size_t i,j;
	long score = 0;
	// calculate counts of every byte in the sharesize string
	for(i = 0; i < sharesize.size(); i++) {
		count[i] = 0;
		for(j = i+1; j < sharesize.size(); j++)
			if(sharesize[i]==sharesize[j]) ++ count[i];
	}

	// make the weighted sum
	for(i = 0; i < sharesize.size();)
		score += count[i]*++i;

	// calculate the differences
	for(i = 0; i < sharesize.size()-1; i++)
		diff[i] = 10 + sharesize[i-1] - sharesize[i];

	// calculate counts of every byte in the sharesize string differences
	for(i = 0; i < sharesize.size()-1; i++) {
		count[i] = 0;
		for(j = i+1; j < sharesize.size(); j++)
			if(diff[i]==diff[j]) ++ count[i];
	}
	for(i = 0; i < sharesize.size();)
		score += count[i]*++i;

	return score;
}



/*!
    \fn nDirectConnect::cUser::DisplayInfo(ostream &os, int DisplClass)
 */
 void nDirectConnect::cUser::DisplayInfo(ostream &os, int DisplClass)
{
	static const char *ClassName[]={"Guest","Registred", "VIP", "Operator", "Cheef", "Admin" , "6-err","7-err", "8-err","9-err","Master"};

	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Nickname") << mNick << " [" << toUpper(ClassName[this->mClass]);
	if( this->mClass != this->mxConn->GetTheoricalClass() )
			os << " - " << autosprintf(_("Default class %d"),this->mxConn->GetTheoricalClass());
	os  << "]" << endl;
	if(DisplClass >= eUC_CHEEF)
		os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("In list") << this->mInList << endl;
	if(!this->mxConn) {
		os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Special user") << endl;
	} else {
		if(DisplClass >= eUC_OPERATOR)
			os << "[*] " << setw(PADDING) << setiosflags(ios::left) << "IP" << mxConn->AddrIP() << endl;
		if(DisplClass >= eUC_OPERATOR && mxConn->AddrHost().size())
			os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Host") << mxConn->AddrHost() << endl;
		if(mxConn->mCC.size())
			os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Country Code") << mxConn->mCC << endl;
		if(mxConn->mRegInfo != NULL)
			os << *(mxConn->mRegInfo);
	}
}

bool nDirectConnect::cUser::Can(unsigned Right, long now, unsigned OtherClass)
{
	if( mClass >= nDirectConnect::nEnums::eUC_ADMIN ) return true;
	switch(Right)
	{
		case nDirectConnect::nEnums::eUR_CHAT: if(!mGag || (mGag > now)) return false; break;
		case nDirectConnect::nEnums::eUR_PM  : if(!mNoPM || (mNoPM > now)) return false; break;
		case nDirectConnect::nEnums::eUR_SEARCH: if(!mNoSearch || (mNoSearch > now)) return false; break;
		case nDirectConnect::nEnums::eUR_CTM: if(!mNoCTM || (mNoCTM > now)) return false; break;
		case nDirectConnect::nEnums::eUR_KICK   : if((mClass < nDirectConnect::nEnums::eUC_OPERATOR) && mCanKick && (mCanKick < now)) return false; break;
		case nDirectConnect::nEnums::eUR_DROP   : if((mClass < nDirectConnect::nEnums::eUC_OPERATOR) && mCanDrop && (mCanDrop < now)) return false; break;
		case nDirectConnect::nEnums::eUR_TBAN   : if((mClass < nDirectConnect::nEnums::eUC_OPERATOR) && mCanTBan && (mCanTBan < now)) return false; break;
		case nDirectConnect::nEnums::eUR_PBAN   : if((mClass < nDirectConnect::nEnums::eUC_OPERATOR) && mCanPBan && (mCanPBan < now)) return false; break;
		case nDirectConnect::nEnums::eUR_NOSHARE: if((mClass < nDirectConnect::nEnums::eUC_VIPUSER ) && mCanShare0 && (mCanShare0 < now)) return false; break;
		case nDirectConnect::nEnums::eUR_REG: if((mClass < mxServer->mC.min_class_register ) && mCanReg && (mCanReg < now)) return false; break;
		case nDirectConnect::nEnums::eUR_OPCHAT: if((mClass < eUC_OPERATOR ) && mCanOpchat && (mCanOpchat < now)) return false; break;
		default: break;
	};
	return true;
}

void nDirectConnect::cUser::SetRight(unsigned Right, long until, bool allow)
{
	switch(Right)
	{
		case nDirectConnect::nEnums::eUR_CHAT:
			if(!allow) mGag = until;
			else mGag = 1;
			break;
		case nDirectConnect::nEnums::eUR_PM:
			if(!allow) mNoPM = until;
			else mNoPM = 1;
			break;
		case nDirectConnect::nEnums::eUR_SEARCH:
			if(!allow) mNoSearch = until;
			else mNoSearch = 1;
			break;
		case nDirectConnect::nEnums::eUR_CTM:
			if(!allow) mNoCTM = until;
			else mNoCTM = 1;
			break;
		case nDirectConnect::nEnums::eUR_KICK:
			if(allow) mCanKick = until;
			else mCanKick = 1;
			break;
		case nDirectConnect::nEnums::eUR_REG:
			if(allow) mCanReg = until;
			else mCanReg = 1;
			break;
		case nDirectConnect::nEnums::eUR_OPCHAT:
			if(allow) mCanOpchat = until;
			else mCanOpchat = 1;
			break;
		case nDirectConnect::nEnums::eUR_NOSHARE:
			if(allow) mCanShare0 = until;
			else mCanShare0 = 1;
			break;
		case nDirectConnect::nEnums::eUR_DROP:
			if(allow) mCanDrop = until;
			else mCanDrop = 1;
			break;
		case nDirectConnect::nEnums::eUR_TBAN:
			if(allow) mCanTBan = until;
			else mCanTBan = 1;
			break;
		case nDirectConnect::nEnums::eUR_PBAN:
			if(allow) mCanPBan = until;
			else mCanPBan = 1;
			break;
		default: break;
	};
}
/*!
    \fn nDirectConnect::cUser::ApplyRights(cPenaltyList::sPenalty &pen)
 */
void nDirectConnect::cUser::ApplyRights(cPenaltyList::sPenalty &pen)
{
	mGag = pen.mStartChat;
	mNoPM = pen.mStartPM;
	mNoSearch = pen.mStartSearch;
	mNoCTM = pen.mStartCTM;
	mCanKick = pen.mStopKick;
	mCanShare0 = pen.mStopShare0;
	mCanReg = pen.mStopReg;
	mCanOpchat = pen.mStopOpchat;
}

bool nDirectConnect::cUserRobot::SendPMTo(cConnDC *conn, const string &msg)
{
	if (conn && conn->mpUser)
	{
		string pm;
		cDCProto::Create_PM(pm, mNick,conn->mpUser->mNick, mNick, msg);
		conn->Send(pm,true);
		return true;
	}
	return false;
}

nDirectConnect::cChatRoom::cChatRoom(const string &nick, cUserCollection *col, cServerDC *server) :
	cUserRobot(nick, server), mCol(col)
{
	mConsole = new cChatConsole(mxServer, this);
	mConsole->AddCommands();
};

nDirectConnect::cChatRoom::~cChatRoom()
{
	if (mConsole) delete mConsole;
	mConsole = NULL;
}

void nDirectConnect::cChatRoom::SendPMToAll(const string & Msg, cConnDC *FromConn)
{
	string omsg;
	string start, end, FromNick;

	if (FromConn && FromConn->mpUser) {
		FromNick = FromConn->mpUser->mNick;
	} else {
		FromNick = mNick;
	}

	if (mCol) {
		this->mxServer->mP.Create_PMForBroadcast(start, end, mNick, FromNick, Msg);
		bool temp_save_inlist;

		if (FromConn && FromConn->mpUser) {
			temp_save_inlist = FromConn->mpUser->mInList;
			FromConn->mpUser->mInList = false;
		}

		this->mCol->SendToAllWithNick(start,end);

		if (FromConn && FromConn->mpUser) {
			FromConn->mpUser->mInList = temp_save_inlist;
		}
	}
}

bool nDirectConnect::cChatRoom::ReceiveMsg(cConnDC *conn, cMessageDC *msg)
{
	ostringstream os;
	if (msg->mType == eDC_TO)
	{
		if(conn && conn->mpUser && mCol)
		{
			bool InList = mCol->ContainsNick(conn->mpUser->mNick);
			if ( InList || IsUserAllowed(conn->mpUser))
			{
				if (!InList) // Auto-join
					mCol->Add(conn->mpUser);

				string &chat = msg->ChunkString(eCH_PM_MSG);
				if (chat[0]=='+')
				{
					if (!mConsole->DoCommand(chat, conn))
						SendPMTo(conn, "Unknown Chatroom command.");

				}
				else SendPMToAll(chat, conn);
			}
			else
			{
				os << "You can't use " << mNick << " rather use main chat ;o)..";
				SendPMTo(conn, os.str());
			}
		}
	}
	return true;
}

bool nDirectConnect::cChatRoom::IsUserAllowed(cUser *)
{
	return false;
}


///////////////////////

nDirectConnect::cOpChat::cOpChat(cServerDC *server) : cChatRoom(server->mC.opchat_name, &server->mOpchatList, server)
{}

bool nDirectConnect::cOpChat::IsUserAllowed(cUser *user)
{
	if (user && (user->mClass >= eUC_OPERATOR)) return true;
	else return false;
}

///////////////////////

bool nDirectConnect::cMainRobot::ReceiveMsg(cConnDC *conn, cMessageDC *message)
{
	ostringstream os;
	if (message->mType == eDC_TO)
	{
		string &msg = message->ChunkString(eCH_PM_MSG);
		if (!mxServer->mP.ParseForCommands(msg, conn))
		{
			cUser *other = mxServer->mUserList.GetUserByNick ( mxServer->LastBCNick );
				if(other && other->mxConn)
				{
					mxServer->DCPrivateHS(message->ChunkString(eCH_PM_MSG),other->mxConn,&conn->mpUser->mNick);
				}
				else
					mxServer->DCPrivateHS("Hub-security doesn't accept pm's, but you can try a +command (or !)", conn);
		}
	}
   return true;
}

nDirectConnect::cPluginRobot::cPluginRobot(const string &nick, cVHPlugin *pi, cServerDC *server) :
	cUserRobot(nick, server), mPlugin(pi)
{}

bool nDirectConnect::cPluginRobot::ReceiveMsg(cConnDC *conn, cMessageDC *message)
{
	ostringstream os;
	if (message->mType == eDC_TO)
	{
		mPlugin->RobotOnPM( this, message, conn);
	}
	return true;
}
