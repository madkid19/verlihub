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
#include "cconndc.h"
#if HAVE_LIBGEOIP
#include "cgeoip.h"
#endif
#include "creglist.h"
#include "creguserinfo.h"
#include "cbanlist.h"
#include "cserverdc.h"
#include "ccustomredirects.h"

using namespace ::nDirectConnect::nTables;
namespace nDirectConnect
{


cConnDC::cConnDC(int sd, cAsyncSocketServer *server)
	: cAsyncConn(sd,server)
{
	mpUser = NULL;
	SetClassName("ConnDC");
	mLogStatus = 0;
	memset(&mTO,0, sizeof(mTO));
	//REM mBan.mBanType = 0;
	mFeatures = 0;
	mSendNickList = false;
	mNickListInProgress = false;
	mConnType = NULL;
	mCloseReason = 0;
	SetTimeOut(eTO_LOGIN, Server()->mC.timeout_length[eTO_LOGIN], server->mTime); // timeout to login on 10 minutes
	mGeoZone = 0; // default - all other then specified countries
	mRegInfo = NULL;
	mSRCounter = 0;
}

cConnDC::~cConnDC()
{
	if (mRegInfo) delete mRegInfo;
	mRegInfo = NULL;
}

/** returns true if ok, unless false */
bool cConnDC::SetUser(cUser *usr)
{
	if(!usr)
	{
		if(ErrLog(0)) LogStream() << "Trying to add a NULL user" << endl;
		return false;
	}
	if(mpUser)
	{
		if(ErrLog(1)) LogStream() << "Trying to add user when it's actually done" << endl;
		delete usr;
		return false;
	}
	mpUser = usr;
	mpUser->mxConn = this;
	mpUser->mxServer = (cServerDC *) mxServer;
	if(Log(3)) LogStream() << "User " << usr->mNick << " connected ... " << endl;	
	return true;
}

/** Send raw data whenever it's next possible,
		n is the size of data, if not specified, or zero, the null terminated string is mesured */
int cConnDC::Send(string & data, bool IsComplete, bool Flush)
{
	if(!mWritable)
		return 0;

	if(data.size() >= MAX_SEND_SIZE-1) {
		if(Log(2))
			LogStream() << "Truncating too long message from: "
				<< data.size() << " to " << MAX_SEND_SIZE -1 << " Message starts with: " << data.substr(0,10) << endl;
		data.resize(MAX_SEND_SIZE -1,' ');
	}
	if(Log(5)) LogStream() << "OUT: " << data.substr(0,100) << endl;
	
	if(msLogLevel >= 3)
		Server()->mNetOutLog << data.size() << " "
			<< data.size() << " "
			<< 1 << " " << data.substr(0,10) << endl;

	if(IsComplete) data.append("|");
	
	string dataToSend = data;
	if(/*GetLSFlag(eLS_LOGIN_DONE) == eLS_LOGIN_DONE &&*/ mFeatures & eSF_ZLIB) {
		// If data should be buffered append content to zlib buffer
		if(!Flush) {
			Server()->mZLib->AppendData(dataToSend.c_str(), dataToSend.size());
			return 1;
		}
		size_t compressedDataLen = 0;
		char *compressedData = Server()->mZLib->Compress(dataToSend.c_str(), dataToSend.size(), compressedDataLen);
		// Compression failed
		if(compressedData == NULL) {
			if(Log(5))
				LogStream() << "Error compressing data with ZLib. Fall back to uncompressed data" << endl;
		} else {
			dataToSend.assign(compressedData, compressedDataLen);
		}
	}
	int ret = Write(dataToSend, Flush);
	mTimeLastAttempt.Get();
	if (mxServer) {
		// Server()->mUploadZone[mGeoZone].Dump();
		Server()->mUploadZone[mGeoZone].Insert(Server()->mTime, (int) dataToSend.size());
		// Server()->mUploadZone[mGeoZone].Dump();
	}
	if(IsComplete) data.erase(data.size()-1,1);
	return ret;
}

/** log the event */
int cConnDC::StrLog(ostream & ostr, int level)
{
	if(cObj::StrLog(ostr,level))
	{
		LogStream()   << "(" << mAddrIP ;//<< ":" << mAddrPort;
		if(mAddrHost.length())
			LogStream() << " " << mAddrHost;
		LogStream()   << ") ";
		if(mpUser)
			LogStream() << "[ " << mpUser->mNick << " ] ";
		return 1;
	}
	return 0;
}

/** set log status flag to a given one in the user*/
void cConnDC::SetLSFlag(unsigned int st)
{
	mLogStatus |= st;
}

/** set log status flag to a given one in the user*/
void cConnDC::ReSetLSFlag(unsigned int st)
{
	mLogStatus = st;
}

/** get log status flag to a given one in the user */
unsigned int cConnDC::GetLSFlag(unsigned int st)
{
	return mLogStatus & st;
}

/** this is called every period of time */
int cConnDC::OnTimer(cTime &now)
{
	ostringstream os;
	string omsg;
	// check the timeouts
	int i;
	for(i=0; i < eTO_MAXTO; i++)
	{
		if(!CheckTimeOut(tTimeOut(i), now))
		{
			os << Server()->mL.operation_timeout << " (" << Server()->mL.timeout_text[tTimeOut(i)] << ")";
			if(Log(2)) LogStream() << "Operation timeout (" << tTimeOut(i) << ")" << endl;
			Server()->ConnCloseMsg(this,os.str(),6000, eCR_TIMEOUT);
			break;
		}
	}
	if (mTimeLastIOAction.Sec() < (mTimeLastAttempt.Sec() - 270)) {
		os << Server()->mL.timeout_any;
		if(Log(2)) LogStream() << "Any action timeout.." << endl;
		Server()->ConnCloseMsg(this,os.str(),6000, eCR_TO_ANYACTION);
	}

	// check frozen users, send to every user, every minute an empty message
	cTime ten_min_ago;
	ten_min_ago = ten_min_ago - 600;
	if(
		Server()->MinDelay(mT.ping,Server()->mC.delayed_ping) &&
		mpUser &&
		mpUser->mInList &&
		mpUser->mT.login < ten_min_ago
		)
	{
		omsg="";
		Send(omsg,true);
	}

	// upload line optimisation  - upload userlist slowlier
	if(mpUser && mpUser->mQueueUL.size())
	{
		unsigned long pos = 0,ppos=0;
		string buf,nick;
		cUser *other;
		for(i=0; i < Server()->mC.ul_portion; i++)
		{
			pos=mpUser->mQueueUL.find_first_of('|',ppos);
			if(pos == mpUser->mQueueUL.npos) break;

			nick = mpUser->mQueueUL.substr(ppos, pos-ppos);
			other = Server()->mUserList.GetUserByNick( nick );
			ppos=pos+1;

			// check if user found
			if(!other)
			{
				if(nick != Server()->mC.hub_security && nick != Server()->mC.opchat_name)
				{
					cDCProto::Create_Quit(buf, nick);
				}
			}
			else
			{
				buf.append(Server()->mP.GetMyInfo(other, mpUser->mClass));
			}
		}
		Send(buf,true);

		if(pos != mpUser->mQueueUL.npos) pos++;
		// I can spare some RAM here by copying it to intermediate buffer and back
		mpUser->mQueueUL.erase(0,pos);
		mpUser->mQueueUL.reserve(0);
	}
	
	return 0;
}

/** storno the timeout */
int cConnDC::ClearTimeOut(tTimeOut to)
{
	if(to >= eTO_MAXTO) return 0;
	mTO[to].Disable();
	return 1; //ok
}

int cConnDC::SetTimeOut(tTimeOut to, double Sec, cTime &now)
{
	if(to >= eTO_MAXTO) return 0;
	if(Sec == 0.) return 0;
	mTO[to].mMaxDelay = Sec;
	mTO[to].Reset(now);
	return 1;
}

/** return true if time is not out yet */
int cConnDC::CheckTimeOut(tTimeOut to, cTime &now)
{
	if(to >= eTO_MAXTO) return 0;
	return 0 == mTO[to].Check(now);
	return 1;
}


/** this is called when write buffer gets empty */
void cConnDC::OnFlushDone()
{
	mBufSend.erase(0,mBufSend.size());
	if (mNickListInProgress)
	{
		SetLSFlag(eLS_NICKLST);
		mNickListInProgress = false;
		if(!ok || !mWritable)
		{
			if(Log(2)) LogStream() << "Connection closed during nicklist" << endl;
		}
		else
		{
			if(Log(2)) LogStream() << "Login after nicklist" << endl;
			Server()->DoUserLogin(this);
		}
	}
}

/** function called before closing nicely */
int cConnDC::OnCloseNice()
{
	if(mxServer) {
		string address = Server()->mCo->mRedirects->MatchByType(this->mCloseReason);
		if(!address.empty()) {
			string omsg = "$ForceMove " + address;
			Send(omsg,true);
		}
	}
	return 0;
}

void cConnDC::CloseNow(int Reason)
{
	this->mCloseReason = Reason;
	this->cAsyncConn::CloseNow();
}

void cConnDC::CloseNice(int msec, int Reason)
{
	this->mCloseReason = Reason;
	this->cAsyncConn::CloseNice(msec);
}

bool cConnDC::NeedsPassword()
{
	return
	   (
		mRegInfo &&
		(mRegInfo->mEnabled || mRegInfo->mClass == eUC_MASTER )&&
		(mRegInfo->mClass != eUC_PINGER) &&
		(!mRegInfo->mPwdChange ||
		 (
		   mRegInfo->mPasswd.size() &&
		   Server()->mC.allways_ask_password
		 )
		)
	);
}

/***************************************
   DC Connection Factory
****************************************/

cDCConnFactory::cDCConnFactory(cServerDC *server) : cConnFactory(&(server->mP)), mServer(server)
{}


cAsyncConn *cDCConnFactory::CreateConn(tSocket sd)
{
	cConnDC *conn;
	if (!mServer) return NULL;

	conn = new cConnDC(sd, mServer);
	conn->mxMyFactory = this;
#if HAVE_LIBGEOIP
	if (
		mServer->sGeoIP.GetCC(conn->AddrIP(),conn->mCC) &&
		mServer->mC.cc_zone[0].size()
	){
		for (int i = 0; i < 3; i ++)
		{
			if(
		 		(conn->mCC == mServer->mC.cc_zone[i]) ||
				(mServer->mC.cc_zone[i].find(conn->mCC) != mServer->mC.cc_zone[i].npos)
			){
				conn->mGeoZone = i+1;
				break;
			}
		}
	}
#endif
	long IPConn, IPMin, IPMax;
	IPConn = cBanList::Ip2Num(conn->AddrIP());
	if (mServer->mC.ip_zone4_min.size())
	{
		IPMin = cBanList::Ip2Num(mServer->mC.ip_zone4_min);
		IPMax = cBanList::Ip2Num(mServer->mC.ip_zone4_max);
		if( (IPMin <= IPConn) && (IPMax >= IPConn)) conn->mGeoZone = 4;
	}
	if (mServer->mC.ip_zone5_min.size())
	{
		IPMin = cBanList::Ip2Num(mServer->mC.ip_zone5_min);
		IPMax = cBanList::Ip2Num(mServer->mC.ip_zone5_max);
		if( (IPMin <= IPConn) && (IPMax >= IPConn)) conn->mGeoZone = 5;
	}
	if (mServer->mC.ip_zone6_min.size())
	{
		IPMin = cBanList::Ip2Num(mServer->mC.ip_zone6_min);
		IPMax = cBanList::Ip2Num(mServer->mC.ip_zone6_max);
		if( (IPMin <= IPConn) && (IPMax >= IPConn)) conn->mGeoZone = 6;
	}
	conn->mxProtocol = mProtocol;
	return (cAsyncConn*) conn;
}

void cDCConnFactory::DeleteConn(cAsyncConn * &Conn)
{
	cConnDC *conn = (cConnDC*)Conn;
	if (conn)
	{
		if (conn->GetLSFlag(eLS_ALOWED))
		{
			mServer->mUserCountTot --;
			mServer->mUserCount[conn->mGeoZone] --;
			if (conn->mpUser)
				mServer->mTotalShare -= conn->mpUser->mShare;
		}
		if (conn->mpUser)
		{
			mServer->RemoveNick(conn->mpUser);
			if (conn->mpUser->mClass) mServer->mR->Logout(conn->mpUser->mNick);
			delete conn->mpUser;
			conn->mpUser  = NULL;
		}
		#ifndef WITHOUT_PLUGINS
		mServer->mCallBacks.mOnCloseConn.CallAll(conn);
		#endif
	}
	cConnFactory::DeleteConn(Conn);
}

};

