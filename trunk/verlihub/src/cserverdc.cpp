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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "cserverdc.h"
#include "cinterpolexp.h"
#include "cconndc.h"
#include "creglist.h"
#include "cbanlist.h"
#include "ckicklist.h"
#include "cpenaltylist.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h> /* for strtoll */
#include <stdio.h> /* for remove and rename */
#include <fstream>
#include <cctype>
#include <algorithm>
#include "curr_date_time.h"
#include "cthreadwork.h"
#include "stringutils.h"
#include "cconntypes.h"
#include "cdcconsole.h"
#include "ctriggers.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define HUB_VERSION_CLASS "(" __CURR_DATE_TIME__ ")"
#define HUB_VERSION_STRING VERSION
#define LOCK_VERSION PACKAGE
#if !defined _WIN32
#define HUB_VERSION_NAME "VerliHub"
#else
#define HUB_VERSION_NAME "WinVerliHub"
#endif

using namespace std;
using namespace nUtils;
using namespace nThreads;
using namespace nStringUtils;
namespace nDirectConnect
{

cServerDC * cServerDC::sCurrentServer = NULL;
using namespace nTables;
#if HAVE_LIBGEOIP
cGeoIP cServerDC::sGeoIP;
#endif


cServerDC::cServerDC( string CfgBase , const string &ExecPath):
	cAsyncSocketServer(), // create parent class
	mConfigBaseDir(CfgBase),
	mDBConf(CfgBase +"/dbconfig"), // load the db config
	mMySQL(
		mDBConf.db_host,
		mDBConf.db_user,
		mDBConf.db_pass,
		mDBConf.db_data), // connect to mysql
	mC(*this), // create the config object
	mL(*this),
	mSetupList(mMySQL),
	mP(this),
	mCo(NULL), //  create console and it's tables
	mR(NULL),
	mPenList(NULL),
	mBanList(NULL),
	mUnBanList(NULL),
	mKickList(NULL),
	mOpChat(NULL),
	mExecPath(ExecPath),
	mSysLoad(eSL_COOL),
	mUserList(true,true, true, &mCallBacks.mNickListNicks, &mCallBacks.mNickListInfos),
	mOpList(true, false, false, &mCallBacks.mOpListNicks, NULL),
	mOpchatList(true),
	mUserCountTot(0),
	mTotalShare(0),
	mSlowTimer(30.0, 0.0, mTime),
	mHublistTimer(0.0,0.0, mTime),
	mReloadcfgTimer(0.0,0.0,mTime),
	mPluginManager(this, CfgBase + "/plugins"),
	mCallBacks(&mPluginManager)
{
	sCurrentServer = this;
	mSetupList.CreateTable();
	mC.AddVars();
	mC.Save();
	mC.Load();
	mL.AddVars();
	if( mC.save_lang ) mL.Save();
	mL.Load();
	mConnTypes = new cConnTypes(this);
	mCo = new cDCConsole(this,mMySQL);
	mR = new cRegList(mMySQL,this);
	mBanList = new cBanList(this);
	mUnBanList = new cUnBanList(this);
	mPenList = new cPenaltyList(mMySQL);
	mKickList = new cKickList(mMySQL);

	int i;
	for ( i = 0; i <= USER_ZONES; i++ ) mUserCount[i]=0;
	for ( i = 0; i <= USER_ZONES; i++ ) mUploadZone[i].SetPeriod(60.);

	SetClassName("cServerDC");

	struct hostent *he = gethostbyname(mC.hublist_host.c_str());
	if(he != NULL) externalIP = (char *) inet_ntoa(*(reinterpret_cast<in_addr*>(he->h_addr_list[0])));
	else externalIP = "";
	
	mR->CreateTable();
	if(mC.use_reglist_cache) mR->ReloadCache();
	mBanList->CreateTable();
	mBanList->Cleanup();
	mUnBanList->CreateTable();
	mUnBanList->Cleanup();
	mBanList->SetUnBanList(mUnBanList);
	mKickList->CreateTable();
	mKickList->Cleanup();
	mPenList->CreateTable();
	mPenList->Cleanup();	
	mConnTypes->OnStart();
	if(mC.use_penlist_cache) mPenList->ReloadCache();

	// setup userlists
	string nctmp;
	nctmp="$NickList ";
	mUserList.SetNickListStart(nctmp);
	nctmp="$OpList ";
	mOpList.SetNickListStart(nctmp);
	nctmp="$$";
	mUserList.SetNickListSeparator(nctmp);
	mOpList.SetNickListSeparator(nctmp);
	mOpchatList.SetNickListSeparator("\r\n");
	nctmp="$ActiveList ";
	mActiveUsers.SetNickListStart(nctmp);

	// add the users
	string speed(/*"Hub\x9"*/" "),mail(""),share("0");
	cUser *VerliHub;
	VerliHub=new cMainRobot(mC.hub_security, this);
	VerliHub->mClass=tUserCl(10);
	mP.Create_MyINFO(VerliHub->mMyINFO ,VerliHub->mNick,mC.hub_security_desc,speed,mail,share);
	VerliHub->mMyINFO_basic = VerliHub->mMyINFO;
	AddRobot((cMainRobot*)VerliHub);

	if (mC.opchat_name.size())
	{
		mOpChat=new cOpChat(this);
		mOpChat->mClass=tUserCl(10);
		mP.Create_MyINFO(mOpChat->mMyINFO, mOpChat->mNick,mC.opchat_desc,speed,mail,share);
		mOpChat->mMyINFO_basic = mOpChat->mMyINFO;
		AddRobot((cMainRobot*)mOpChat);
	}
	string net_log(mConfigBaseDir);
	net_log.append( "/net_out.log");
	mNetOutLog.open(net_log.c_str(),ios::out);
	mTotalShare = 0;

	mFactory = new cDCConnFactory(this);

	try {
		mPluginManager.LoadAll();
	} catch (...)
	{
		if(ErrLog(1)) LogStream() << "Plugin loading error" << endl;
	}
	mUsersPeak = 0;
}

cServerDC::~cServerDC()
{
	if(Log(1)) LogStream() << "Destructor cServerDC" << endl;
	mNetOutLog.close();
	// remove all users
	cUserCollection::iterator it;
	cUser *user;
	for (it=mUserList.begin(); it != mUserList.end();)
	{
		user = (cUser*)*it;
		++it;
		if (user->mxConn){
			delConnection(user->mxConn);
		} else {
			this->RemoveNick(user);
		}
	}

	// destruct the lists of pointers
	for (tTFIt i = mTmpFunc.begin(); i != mTmpFunc.end(); i++ ) if(*i) delete *i;
	close();
	if (mFactory) delete mFactory;
	mFactory = NULL;
	if (mConnTypes) delete mConnTypes;
	mConnTypes = NULL;
	if (mR) delete mR;
	mR= NULL;
	if (mBanList) delete mBanList;
	mBanList = NULL;
	if (mUnBanList) delete mUnBanList;
	mUnBanList = NULL;
	if (mPenList) delete mPenList;
	mPenList = NULL;
	if (mKickList) delete mKickList;
	mKickList = NULL;
	if (mCo) delete mCo;
	mCo = NULL;
}

int cServerDC::StartListening(int OverrideDefaultPort)
{
	int _result = cAsyncSocketServer::StartListening(OverrideDefaultPort);
	istringstream is(mC.extra_listen_ports);
	int i = 1;
	while(i)
	{
		i = 0;
		is >> i;
		if (i) cAsyncSocketServer::Listen(i, false);
	}
	return _result;
}

/**
 * The rules for allowed messages
 * ------------------------------
 *  *** USER IS NOT LOGGED IN USERLIST ***
 *  . mask = KEY | VALIDATENICK | MYPASS | VERSION | GETNICKLIST | MYINFO
 *  . (they may apprear in given order)
 *  . GETNICKLIST only if eLS_KEYOK && eLS_VALNICK && eLS_PASSWD && eLS_VERSION
 *  *** USER IS IN USERLIST ***
 *  . EVERYTHING except KEY VALIDATENICK MYPASS VERSION
 *  -------
 *  *** hub is loaded ***
 *	SOME MAY BE REMOVED: SEARCH*, SR, MYINFO_ALL , CHAT, GETNICKLIST
 *
 **/

tMsgAct cServerDC::Filter( tDCMsg msg, cConnDC *conn )
{
	tMsgAct result = eMA_PROCEED;
	if(!conn) // this is strange
	{
		if(ErrLog(0)) LogStream() << "Got NULL conn into filter" << endl;
		return eMA_ERROR;
	}
	if(!conn->mpUser || !conn->mpUser->mInList)
	{
		switch(msg)
		{
			case eDC_KEY:
			case eDC_VALIDATENICK:
			case eDC_MYPASS:
			case eDC_VERSION:
			case eDC_GETNICKLIST:
			case eDC_MYNIFO:
			case eDC_UNKNOWN:
			break;
			default: result = eMA_HANGUP;
		}
	}
	else
	{
		switch(msg)
		{
			case eDC_KEY:
			case eDC_VALIDATENICK:
			case eDC_MYPASS:
			case eDC_VERSION:
				result=eMA_HANGUP;
			break;
			default: break;
		}
	}

	switch( mSysLoad )
	{
		case eSL_SYSTEM_DOWN: return eMA_TBAN; break; // ignore and pray
		case eSL_CRITICAL: return eMA_HANGUP1; break;
		case eSL_SQEEZY: break;
		case eSL_HURRY: break;
		case eSL_COOL: break;
		default: break;
	}
	return result; 
}

/** sends a public chat message to a given connection */
int cServerDC::DCPublic(const string &from, const string &txt, cConnDC *conn)
{
	static string msg;
	msg.erase();
	cDCProto::Create_Chat(msg, from, txt);
	if (conn) {
		conn->Send( msg , true);
		return 1;
	} else {
		return 0;
	}
}

int cServerDC::DCPublicToAll(const string &from, const string &txt, int min_class, int max_class)
{
	static string msg;
	msg.erase();
	cDCProto::Create_Chat(msg, from, txt);
	//TODO: Use constant instead of num
	if(min_class !=0 && max_class != 10)
		mUserList.SendToAllWithClass(msg, min_class, max_class, true, true);
	else
		mUserList.SendToAll(msg, true, true);
	return 1;
}

/** send a hub security public message */
int cServerDC::DCPublicHS(const string &text, cConnDC *conn)
{
	return DCPublic( mC.hub_security, text,  conn );
}

/** send a hub security public message */
void cServerDC::DCPublicHSToAll(const string &text)
{
	static string msg;
	msg.erase();
	cDCProto::Create_Chat(msg, mC.hub_security , text);
	mUserList.SendToAll(msg, true, true);
}

/** send a hub security private message */
int cServerDC::DCPrivateHS(const string & text, cConnDC * conn, string *from)
{
	string msg;
	mP.Create_PM(msg, mC.hub_security, conn->mpUser->mNick, (from!=NULL)?(*from):(mC.hub_security), text);
	return conn->Send(msg, true);
}

/** \brief adds a robot user to the list
  *
  * the robot user will receive him designed events in a special way
  * by calling this function you add a user into a robot list mRobotList
  */
bool cServerDC::AddRobot(cUserRobot *robot)
{
	if (AddToList(robot))
	{
		mRobotList.Add(robot);
		robot->mxServer = this;
		return true;
	}
	else return false;
}

bool cServerDC::DelRobot(cUserRobot *robot)
{
	if (this->RemoveNick(robot))
	{
		mRobotList.Remove(robot);
		return true;
	}
	else return false;
}

/** adds a user into a user list */
bool cServerDC::AddToList(cUser *usr)
{
	if(!usr)
	{
		if(ErrLog(1)) LogStream() << "Adding a NULL user to userlist" << endl;
		return false;
	}
	if(usr->mInList)
	{
		if(ErrLog(2)) LogStream() << "User is already in the user list, he says it " << endl;
		return false;
	}

	tUserHash Hash = mUserList.Nick2Hash(usr->mNick);

	if( !mUserList.AddWithHash(usr, Hash) )
	{
		if(ErrLog(2)) LogStream() << "Adding twice user with same hash " << usr->mNick << endl;
		usr->mInList = false;
		return false;
	}
	
	usr->mInList = true;
	if( !usr->IsPassive ) mActiveUsers.AddWithHash(usr, Hash);
	if( usr->mClass >= eUC_OPERATOR && !
		( usr->mxConn && usr->mxConn->mRegInfo && usr->mxConn->mRegInfo->mHideKeys))
		mOpList.AddWithHash(usr, Hash);
	if( usr->Can(eUR_OPCHAT, mTime.Sec()) ) mOpchatList.AddWithHash(usr, Hash);
	if( usr->mxConn && !(usr->mxConn->mFeatures & eSF_NOHELLO) )
		mHelloUsers.AddWithHash(usr, Hash);
	if( (usr->mClass >= eUC_OPERATOR) || mC.chat_default_on )
		mChatUsers.AddWithHash(usr, Hash);
	else
		DCPublicHS(mC.msg_chat_onoff, usr->mxConn);

	if(usr->mxConn && usr->mxConn->Log(3)) usr->mxConn->LogStream() << "Adding at the end of Nicklist" << endl;

	if(usr->mxConn && usr->mxConn->Log(3)) usr->mxConn->LogStream() << "Becomes in list" << endl;

	return true;
}

/** removes user with given nick from uselist, return true on success */
bool cServerDC::RemoveNick(cUser *User)
{
	tUserHash Hash = mUserList.Nick2Hash(User->mNick);

	//cout << "Leave: " << User->mNick << " count = " << this->mUserList.size() << " then ";
	if(mUserList.ContainsHash(Hash)) {
		#ifndef WITHOUT_PLUGINS
		if(User->mxConn->GetLSFlag(eLS_LOGIN_DONE)) mCallBacks.mOnUserLogout.CallAll(User);
		#endif
                // make sure that the user we want to remove is the correct one!
                cUser *other = mUserList.GetUserByNick(User->mNick);
		if(!User->mxConn)
		{
			//cout << "Removing robot user" << endl;
			mUserList.RemoveByHash(Hash);
		}
                else if(other && other->mxConn && User->mxConn && other->mxConn == User->mxConn)
                {
            		//cout << "Leave: " << User->mNick << " count = " << mUserList.size() << endl;
			mUserList.RemoveByHash(Hash);
		}
		else
		{
			// this may cause problems when user is robot with 0 connection
			//cout << "NOT found the correct user!, skip removing: " << User->mNick << endl;
	                return false;
		}
	}
	if(mOpList.ContainsHash(Hash)) mOpList.RemoveByHash(Hash);
	if(mOpchatList.ContainsHash(Hash)) mOpchatList.RemoveByHash(Hash);
	if(mActiveUsers.ContainsHash(Hash)) mActiveUsers.RemoveByHash(Hash);
	if(mHelloUsers.ContainsHash(Hash)) mHelloUsers.RemoveByHash(Hash);
	if(mChatUsers.ContainsHash(Hash)) mChatUsers.RemoveByHash(Hash);
	if(mInProgresUsers.ContainsHash(Hash)) mInProgresUsers.RemoveByHash(Hash);

	if (User->mInList)
	{
		User->mInList=false;

		//cout << this->mUserList.size() << endl;
		static string omsg;
		omsg = "";
		cDCProto::Create_Quit(omsg, User->mNick);

		// delayed myinfo implies delay of quit too, otherwise there would be mess in peoples userslists
		mUserList.SendToAll(omsg,mC.delayed_myinfo, true);
		if(mC.show_tags == 1)
		{
			mOpchatList.SendToAll(omsg,mC.delayed_myinfo, true);
		}
	}
	return true;
}

/** send the same data to every user */
void cServerDC::SendToAll(string &data, int cm,int cM)
{
	cConnDC *conn;
	tCLIt i;
	int size = data.size();

	// prepare data
	if(size >= MAX_SEND_SIZE-1)
	{
		if(Log(2))
			LogStream() << "Truncating too long message from: "
				<< data.size() << " to " << MAX_SEND_SIZE -1 << " Message starts with: " << data.substr(0,10) << endl;
		data.resize( MAX_SEND_SIZE -1,' ');
		size = MAX_SEND_SIZE -1;
	}
	if(data[data.size()-1] !='|')
	{
		data.append("|");
		size ++;
	}

	int count=0;
	for(i=mConnList.begin(); i!= mConnList.end(); i++)
	{
		conn=(cConnDC *)(*i);
		if(conn && conn->ok && conn->mWritable && conn->mpUser && conn->mpUser->mInList)
		{
			conn->Write(data, true);
			mUploadZone[conn->mGeoZone].Insert(mTime,data.size());
			++count;
		}
	}
	if(Log(5)) LogStream() << "ALL << " << data.substr(0,100) << endl;
	if(msLogLevel >= 3)
		mNetOutLog << ((unsigned long)count) *data.size() << " "
			<< data.size() << " "
			<< count << " " << data.substr(0,10) << endl;
}

/** sends to every user data composed by start+nick+end */
int cServerDC::SendToAllWithNick(const string &start,const string &end, int cm,int cM)
{
	static string str;
	cConnDC *conn;
	tCLIt i;
	int counter = 0;
	for(i=mConnList.begin(); i!= mConnList.end(); i++)
	{
		conn=(cConnDC *)(*i);
		if(
			conn &&
			conn->ok &&
			conn->mpUser &&
			conn->mpUser->mInList &&
			conn->mpUser->mClass >= cm &&
			conn->mpUser->mClass <= cM
			)
		{
			str=start + conn->mpUser->mNick + end + "|";
			conn->Send(str, false);
			counter ++;
		}
	}
	return counter;
}

int cServerDC::SendToAllWithNickCC(const string &start,const string &end, int cm,int cM, const string &cc_zone)
{
	static string str;
	cConnDC *conn;
	tCLIt i;
	int counter = 0;
	for(i=mConnList.begin(); i!= mConnList.end(); i++)
	{
		conn=(cConnDC *)(*i);
		if(
			conn &&
			conn->ok &&
			conn->mpUser &&
			conn->mpUser->mInList &&
			conn->mpUser->mClass >= cm &&
			conn->mpUser->mClass <= cM &&
			cc_zone.npos != cc_zone.find(conn->mCC)
			)
		{
			str=start + conn->mpUser->mNick + end + "|";
			conn->Send(str, false);
			counter++;
		}
	}
	return counter;
}

/** return negative if conn should be removed */
int cServerDC::OnNewConn(cAsyncConn *nc)
{
	cConnDC *conn = (cConnDC *)nc;
	stringstream errmsg,os;
	if(!conn) return -1;
	string omsg;
	cTime runtime;
	runtime -= mStartTime;

	omsg="$Lock EXTENDEDPROTOCOL_" LOCK_VERSION " Pk=version" HUB_VERSION_STRING "|";
	os << "This hub is running version " << HUB_VERSION_STRING << mC.hub_version_special << " " << HUB_VERSION_CLASS << " of " HUB_VERSION_NAME <<  " (RunTime: "<< runtime.AsPeriod()<<" / Current user count: "<< mUserCountTot <<")|";
	cDCProto::Create_Chat(omsg,mC.hub_security,os.str());
	conn->Send(omsg, false);

	os.str(mEmpty);

	if (mSysLoad >= eSL_CRITICAL)
	{
		os << "Sorry hub is too busy.. Please try again in a few minutes.";
		DCPublicHS(os.str(), conn);
		conn->CloseNice(500, eCR_HUB_LOAD);
		return -1;
	}

#ifndef _WIN32
	if(!this->mUseDNS) conn->SetTimeOut(eTO_KEY, mC.timeout_length[eTO_KEY], mTime);
#endif
	#ifndef WITHOUT_PLUGINS
	mCallBacks.mOnNewConn.CallAll(conn);
	#endif
	return 0;
}

/** create a string to get a line for a given connection, and return the pointer */
string * cServerDC::FactoryString(cAsyncConn *conn)
{
	return conn->FactoryString();
}

/** treat message for given connection */
void cServerDC::OnNewMessage(cAsyncConn *conn, string *str)
{
	if(conn->Log(4)) conn->LogStream() << "IN: " << (*str) << "|" << endl;
	conn->mpMsgParser->Parse();
	conn->mxProtocol->TreatMsg(conn->mpMsgParser, conn);
}

/** return true if user is free to login */
bool cServerDC::VerifyUniqueNick(cConnDC *conn)
{
	string UsrKey, omsg;
	mUserList.Nick2Key(conn->mpUser->mNick,UsrKey);
	/// add user to the list or fail
	if( mUserList.ContainsKey(UsrKey) )
	{
		bool CloseOld = false;
		cUser *old_usr = mUserList.GetUserByKey(UsrKey);

		if(conn->mpUser->mClass >= eUC_REGUSER) CloseOld = true;
		if (    !CloseOld &&
			old_usr->mxConn &&
			(conn->AddrIP() == old_usr->mxConn->AddrIP()) &&
			(conn->mpUser->mShare == old_usr->mShare) &&
			(conn->mpUser->mMyINFO_basic == old_usr->mMyINFO_basic)
		) CloseOld = true;

		if (CloseOld)
		{
			if(old_usr)
			{
				if(old_usr->mxConn)
				{
					if(old_usr->mxConn->Log(2)) old_usr->mxConn->LogStream() << "Closing because of a new connection" << endl;
					omsg = "Another instance of yourself is connecting. Bye and Hi.";
					old_usr->mxConn->Send(omsg,true);
					old_usr->mxConn->CloseNow();
				} else {
					if(ErrLog(1)) LogStream() << "Closing, user " << old_usr->mNick << " , but there's no connection :(" << endl;
				}
				RemoveNick(old_usr);
			}
			else
			{
				if(ErrLog(0)) LogStream() << "Classical example of what never happens " << old_usr->mNick << "'" << endl;
				conn->CloseNow();
				return false;
			}
		}
		else
		{
			omsg = "You are already in the hub.";
			DCPublicHS(omsg, conn);
			conn->CloseNow();
			return false;
		}
	}
	return true;	
}

void cServerDC::AfterUserLogin(cConnDC *conn)
{
	string omsg;

	if(conn->Log(3)) conn->LogStream() << "Entered the hub." << endl;
	mCo->mTriggers->TriggerAll(cTrigger::eTF_MOTD, conn);

	// user has to change password
	if(conn->mRegInfo && conn->mRegInfo->mPwdChange)
	{
		omsg = mC.msg_change_pwd;
		DCPrivateHS(omsg, conn);
		DCPublicHS(omsg, conn);
		conn->SetTimeOut(eTO_SETPASS, mC.timeout_length[eTO_SETPASS], this->mTime);
	}

	// send the hub topic
	string topic("$HubTopic ");
	topic += mC.hub_desc + "|";
	conn->Send(topic, false);

	if (mC.send_user_info)
	{
		ostringstream os;
		os << "\r\n[::] Your info: \r\n";
		conn->mpUser->DisplayInfo(os, eUC_OPERATOR);
		omsg = os.str();
		DCPublicHS(omsg,conn);
	}

	if( mUserList.size() > mUsersPeak ) mUsersPeak = mUserList.size();
	#ifndef WITHOUT_PLUGINS
	mCallBacks.mOnUserLogin.CallAll(conn->mpUser);
	#endif

	if ((conn->mpUser->mClass >= eUC_NORMUSER) && (conn->mpUser->mClass <= eUC_MASTER)) {
		if (mC.msg_welcome[conn->mpUser->mClass].size()) {
			string ToSend;
			ReplaceVarInString(mC.msg_welcome[conn->mpUser->mClass], "nick", ToSend, conn->mpUser->mNick);
			ReplaceVarInString(ToSend, "CC", ToSend, conn->mCC);
			DCPublicHSToAll(ToSend);
		}
	}
}

/** is called when user is about to get into the userslist */
void cServerDC::DoUserLogin(cConnDC *conn)
{
	// verify we didn't get here by chance
	if(eLS_LOGIN_DONE != conn->GetLSFlag(eLS_LOGIN_DONE))
	{
		if(conn->ErrLog(2)) conn->LogStream() << "User Login when not all done"<<endl;
		conn->CloseNow();
		return;
	}

	// check if same nick already exists
	if (! VerifyUniqueNick(conn)) return;

	// he is not anymore in progress
	if (mInProgresUsers.ContainsNick(conn->mpUser->mNick))
	{
		mInProgresUsers.FlushForUser(conn->mpUser);
		mInProgresUsers.Remove(conn->mpUser);
	}
	
	// anti login flood temp bans
	if (conn->GetTheoricalClass() <= mC.max_class_int_login){
		mBanList->AddNickTempBan(conn->mpUser->mNick, mTime.Sec() + mC.int_login, "login later");
		mBanList->AddIPTempBan(conn->GetSockAddress(), mTime.Sec() + mC.int_login, "login later");
	}

	// users special rights and restrictions
	cPenaltyList::sPenalty pen;
	if (mPenList->LoadTo(pen, conn->mpUser->mNick) && 
			(conn->mpUser->mClass != eUC_PINGER))
		conn->mpUser->ApplyRights(pen);

	// insert user to userlist
	if(!AddToList(conn->mpUser))
	{
		conn->CloseNow();
		return;
	}

	// display user to others
	ShowUserToAll(conn->mpUser);
	
	if( mC.send_user_ip )
	{
		if( conn->mpUser->mClass >= eUC_OPERATOR )
		{
 			conn->Send(mUserList.GetIPList(),true);
		}
		else
		{
			string UserIP;
			cCompositeUserCollection::ufDoIpList DoUserIP(UserIP);
			DoUserIP.Clear();
			DoUserIP(conn->mpUser);
			mOpchatList.SendToAll(UserIP, true, true);
			conn->Send(UserIP);
		}
	}

	AfterUserLogin(conn);
	
	conn->ClearTimeOut(eTO_LOGIN);
	conn->mpUser->mT.login.Get();
}

bool cServerDC::BeginUserLogin(cConnDC *conn)
{
	// If user asks for nicklist, then the login will happen after the sending of nicklist ends
	// otherwise it will happen now
	int WantedMask;
	if (mC.delayed_login)
	 	WantedMask = eLS_LOGIN_DONE - eLS_NICKLST;
	else
		WantedMask = eLS_LOGIN_DONE;
	
	if(WantedMask == conn->GetLSFlag(WantedMask))
	{
		if(conn->Log(2)) conn->LogStream() << "Begin login" << endl;
		if(VerifyUniqueNick(conn))
		{
			if (!mC.delayed_login) 
			{
				DoUserLogin(conn);
			}
			else
			{
				mInProgresUsers.Add(conn->mpUser);
			}
			
			if (conn->mSendNickList)
			{
				// this may won't send all data at once...
				mP.NickList(conn);	// this will set mNickListInProgress
				conn->mSendNickList = false;
				return true;	// return here since we don't know that the list was sent or not
						// OnFlushDone() will do the login after the NickList is flushed
			}
			if(!conn->mpUser->mInList)
			{
				DoUserLogin(conn);
			}
		}
		else
		{
			return false;
		}
	} else {
		cout << conn->GetLSFlag(eLS_LOGIN_DONE) << " " << eLS_LOGIN_DONE << endl;
		conn->CloseNow();
		return false;
	}
}

#define UseChache true
#define OpList mOpList.GetNickList()
bool cServerDC::ShowUserToAll(cUserBase *user)
{
	string omsg;

	// only Hello users get hello message
	omsg = "$Hello ";
	omsg+= user->mNick;
	mHelloUsers.SendToAll(omsg, mC.delayed_myinfo);

	// all users get myinfo, event hose in progress 
	// (hello users in progres are ignored, they are obsolete btw)
	omsg = mP.GetMyInfo(user, eUC_NORMUSER);
	mUserList.SendToAll(omsg, mC.delayed_myinfo); // use cache -> so this can be after user is added
	mInProgresUsers.SendToAll(omsg, mC.delayed_myinfo);

	// distribute oplist
	if(user->mClass >= eUC_OPERATOR) {
		mUserList.SendToAll(OpList, UseChache);
		mInProgresUsers.SendToAll(OpList, UseChache);
	}

	// send it to all but to him
	// but why ? maybe he would be doubled in some shi** clients ?
	// anyway delayed_login will show why is it..
	// the order of flush of this before the full myinfo for ops
	if(!mC.delayed_login)	{
		user->mInList = false;
		mUserList.FlushCache();
		mInProgresUsers.FlushCache();
		user->mInList = true;
	}

	/// patch eventually for ops
	if(mC.show_tags == 1)	{
		omsg = mP.GetMyInfo(user, eUC_OPERATOR);
		mOpchatList.SendToAll(omsg, mC.delayed_myinfo); // must send after mUserList! Cached mUserList will be flushed after and will override this one!
		mInProgresUsers.SendToAll(omsg, mC.delayed_myinfo); // send later, better more people see tags, then some ops not, 
		// ops are dangerous creatures, they may have idea to kick people for not seeing their tags
	}
	return true;
}

/** close a given connection with sending a message as a reason, and do it at least in to_usec microseconds */
void cServerDC::ConnCloseMsg(cConnDC *conn, const string &msg, int msec, int reason)
{
	DCPublicHS(msg,conn);
	conn->CloseNice(msec, reason);
}

/** send the hello message, on which response we'll get MyINFO */
int cServerDC::DCHello(const string & nick, cConnDC * conn, string *info)
{
	string str("$Hello ");
	str+= nick + "|";
	conn->Send(str, false);
	if(info)
		conn->Send(*info, true);
	return 0;
}

/** return true if the condition of minimal delay is verified
and modify the timestamp */
bool cServerDC::MinDelay(cTime &then, int min)
{
	// @todo use timeins instead of mindelay, or change to microsecond resolution
	cTime now;
	cTime diff=now-then;
	if(diff.Sec() >= min)
	{
		then = now;
		return true;
	}
	return false;
}
bool cServerDC::MinDelayMS(cTime &what, long min)
{
	cTime now;
	cTime diff=now-what;
	if(diff.MiliSec() >= min)
	{
		what = now;
		return true;
	}
	return false;
}
/** return true if accept is allowed - override */
bool cServerDC::AllowNewConn()
{
	return mConnList.size() <= (unsigned) mC.max_users_total + mC.max_extra_regs + mC.max_extra_vips + mC.max_extra_ops + mC.max_extra_cheefs + mC.max_extra_admins + 300;
}

int cServerDC::SaveFile(const string &file, const string &text)
{
	string filename;
	ReplaceVarInString(file, "CFG", filename, mConfigBaseDir);
	ofstream os(file.c_str());
	if(!os.is_open()) return 0;
	os << text << endl;
	os.close();
	return 1;
}

/** return 1 if user is valid and can go in
return 0 otherwise */
int cServerDC::ValidateUser(cConnDC *conn, const string &nick)
{
	// first validate the IP and host if any
	stringstream errmsg,os;
	if(!conn) return 0;
	string omsg;
	time_t n;
	bool close=false;

	// phase 1: test nick validity
	// phase 2: test ip/host ban (registered users pass)
	// phase 3: test nickban
	// then we're done

	static cRegUserInfo *sRegInfo = new cRegUserInfo;
	if ((nick.size() < mC.max_nick * 2 ) &&
		mR->FindRegInfo(*sRegInfo,nick) &&
		!conn->mRegInfo )
	{
		conn->mRegInfo = sRegInfo;
		sRegInfo = new cRegUserInfo;
	}

	// validate nick
	
	tVAL_NICK vn=ValidateNick(nick, (conn->GetTheoricalClass() >= eUC_REGUSER ));
	if(vn != eVN_OK)
	{
		close=true;
		errmsg << "Bad nickname: ";
		if (conn->Log(2)) conn->LogStream() << "Bad nick: '" << nick << "' (" << vn << ")" << endl;
	}
	switch(vn)
	{
		case eVN_OK: break;
		case eVN_CHARS: errmsg << "unallowed characters in your nick; use these: " << mC.nick_chars; break;
		case eVN_SHORT: errmsg << "your nick is too short"; break;
		case eVN_LONG: errmsg << "your nick is too long"; break;
		case eVN_USED: errmsg << "your nick is already in use"; break;
		case eVN_PREFIX: errmsg << mC.msg_nick_prefix << mC.nick_prefix; break;
		case eVN_NOT_REGED_OP: errmsg << "not registered operator"; break;
		//case eVN_RESERVED: errmsg << "reserved"; break;
		case eVN_BANNED:
			n = mBanList->IsNickTempBanned(nick) - cTime().Sec();
			errmsg << "Wait " << n << "sec before reconnecting!!";
			break;
		default: errmsg << " unknown error"; break;
	}


	if(close)
	{
		DCPublicHS(errmsg.str(),conn);
		if (conn->Log(3)) conn->LogStream() << "Bad Nick: " << errmsg.str() << endl;
		return 0;
	}

	cBan Ban(this);
	bool banned = false;
	if(conn->GetTheoricalClass() < eUC_MASTER) { // Master class is immune
		if( conn->GetTheoricalClass() >= eUC_REGUSER ) {
			banned = mBanList->TestBan(Ban, conn, nick, cBan::eBF_NICK);
			if (banned && !((1 << Ban.mType) & (cBan::eBF_NICK |cBan::eBF_NICKIP))) banned = false;
		} else {
			// Here we can't check share ban because user hasn't sent $MyInfo string yet
			banned = mBanList->TestBan(Ban, conn, nick, cBan::eBF_NICKIP | cBan::eBF_RANGE | cBan::eBF_HOST2 | cBan::eBF_HOST1 | cBan::eBF_HOST3 | cBan::eBF_HOSTR1 | cBan::eBF_PREFIX);
		}
	}

	if(banned) {
		ReplaceVarInString(mC.msg_banned, "IP", omsg, conn->AddrIP());
		ReplaceVarInString(omsg, "nick", omsg, Ban.mNick);
		errmsg << "Banned." << endl << omsg << endl;
		Ban.DisplayUser(errmsg);
		DCPublicHS(errmsg.str(),conn);
		if(conn->Log(1)) conn->LogStream() << "Unallowed user (" << Ban.mType << "), closing" << endl;
		return 0;
	}

	if(mC.nick_prefix_cc)
	{
		if(conn->mCC.size() && conn->mCC != "--")
		{
			string Prefix("[");
			Prefix+=conn->mCC;
			Prefix+="]";
			if( 0 != StrCompare(nick,0,4,Prefix))
			{
				errmsg << "Please add " << Prefix << " in front of your nick" << endl;
				close = conn->GetTheoricalClass() < eUC_REGUSER;
			}
		}
	}

	if(close)
	{
		DCPublicHS(errmsg.str(),conn);
		return 0;
	}

	return 1;
}

/** tells if a user with such nick can login into the hub
	return
*/
tVAL_NICK cServerDC::ValidateNick(const string &nick, bool registered)
{
	cTime now;
	string ProhibitedChars("$| ");
	//ProhibitedChars.append("\0",1);
	
	if (!registered)
	{
		//if(nick.size() > strlen(nick.c_str())) return eVN_SHORT;
		if(nick.size() > mC.max_nick ) return eVN_LONG;
		if(nick.size() < mC.min_nick ) return eVN_SHORT;
		if(nick.npos != nick.find_first_of(ProhibitedChars)) return eVN_CHARS;
		if((mC.nick_chars.size()>0) && (nick.npos != nick.find_first_not_of(mC.nick_chars.c_str()))) return eVN_CHARS;
		if(StrCompare(nick,0,mC.nick_prefix.length(),mC.nick_prefix) != 0) return eVN_PREFIX;
		if(StrCompare(nick,0,4,"[OP]") == 0) return eVN_NOT_REGED_OP;
	}
	if( mBanList->IsNickTempBanned(nick) > now.Sec()) return eVN_BANNED;
	return eVN_OK;
}


/** this is called every period of time */
int cServerDC::OnTimer(cTime &now)
{
	mHelloUsers.FlushCache();
	mUserList.FlushCache();
	mOpList.FlushCache();
	mOpchatList.FlushCache();
	mActiveUsers.FlushCache();
	mChatUsers.FlushCache();
	mInProgresUsers.FlushCache();
	
	mSysLoad = eSL_COOL;
	if ( mFrequency.mNumFill > 0 ) 
	{
		double freq = mFrequency.GetMean(mTime);
		if(freq < 1.2 * mC.min_frequency) mSysLoad = eSL_HURRY;
		if(freq < 1.0 * mC.min_frequency) mSysLoad = eSL_SQEEZY;
		if(freq < 0.8 * mC.min_frequency) mSysLoad = eSL_CRITICAL;
		if(freq < 0.5 * mC.min_frequency) mSysLoad = eSL_SYSTEM_DOWN;
	}	

	if ( mC.max_upload_kbps > 0.00001) 
	{
		int zone;
		double total_upload=0.;
		for ( zone = 0; zone <= USER_ZONES; zone ++ )
			total_upload += this->mUploadZone[zone].GetMean(this->mTime);
		if ((total_upload / 1024.0) > mC.max_upload_kbps)
		{
			mSysLoad = eSL_SQEEZY;
		}
	}

	// perform all temp functions
	for (tTFIt i = mTmpFunc.begin(); i != mTmpFunc.end(); i++)
	{
		if (*i) {
			// delete finished functions
			if((*i)->done()) {
				delete *i;
				(*i) = NULL;
//				cout << " deleting tmpfunc" << endl;
			} else {
				// step the rest
				(*i)->step();
			}
		}
	}

	if (bool(mSlowTimer.mMinDelay) && mSlowTimer.Check(mTime , 1) == 0)
		mBanList->RemoveOldShortTempBans(mTime.Sec());
	if (bool(mHublistTimer.mMinDelay) && mHublistTimer.Check(mTime , 1) == 0)
		this->RegisterInHublist(mC.hublist_host, mC.hublist_port, NULL);
	if (bool(mReloadcfgTimer.mMinDelay) && mReloadcfgTimer.Check(mTime , 1) == 0)
	{
		mC.Load();
		mCo->mTriggers->ReloadAll();
		if (mC.use_reglist_cache) mR->UpdateCache();
		if (Log(2)) LogStream() << "Socket counter : " << cAsyncConn::sSocketCounter << endl;
	}
	mUserList.AutoResize();
	mHelloUsers.AutoResize();
	mActiveUsers.AutoResize();
	mChatUsers.AutoResize();
	mOpList.AutoResize();
	mOpchatList.AutoResize();
	mInProgresUsers.AutoResize();

	mBanList->mTempNickBanlist.AutoResize();
	mBanList->mTempIPBanlist.AutoResize();
	mCo->mTriggers->OnTimer(now.Sec());
	#ifndef WITHOUT_PLUGINS
	if (!mCallBacks.mOnTimer.CallAll())
		return false;
	#endif
	return true;
}

/** converts number with units to a time period in seconds */
unsigned cServerDC::Str2Period(const string &s, ostream &err)
{
	istringstream is(s);

	unsigned u=0;
	int m,n=0;
	char c=' ';
	is >> n >> c;
	if(n >= 0)
	{
		m = 1; // multiplicator
		if(c==' ') c = 'd';
		switch(c)
		{
			case 'y':
			case 'Y': m*= 12; // year = 12* month
			case 'M': m*=  4; // month = 4 * week
			case 'w':
			case 'W': m*=  7; // week = 7 days
			case 'd':
			case 'D': m*= 24; // 24 hours
			case 'h':
			case 'H': m*= 60; // 60mniutes
			case 'm': m*= 60; // 60 seconds
			case 's':
			case 'S': break;
			default:
				err << "Error: available units are: "
					 << "s'econd, d'ay, m'inute, h'our, d'ay, w'eek, M'onth, Y'ear." << endl
					 << "Default is 'd'." << endl;
				return 0;
				m=1;
			break;
		}
		u= n*m;
	}
	else err << "Please provide a positive number." << endl;
	return u;
}

/** does the registration on given hublist */
int cServerDC::DoRegisterInHublist(string host, int port, string NickForReply)
{
	ostringstream os, os2;
	char pipe='|';
#ifndef _WIN32
	unsigned long long buf = GetTotalShareSize();
#else
	char buf[32];
	sprintf(buf,"%ui64d",GetTotalShareSize());
#endif

	istringstream is(host);
	string CurHost;
	string lock, key;
	size_t pos_space;
	cAsyncConn *pHubList;

	os2 << "Hublist Register results: \r\n";
	while (CurHost = "", is >> CurHost, CurHost.size() > 0)
	{
		os2 << "Sending to " << CurHost << ":" << port;
		pHubList = new cAsyncConn(CurHost,port);

		if(!pHubList->ok)
		{
			os2 << " ..connection not established\r\n";
			pHubList->Close();
			delete pHubList;
			pHubList = 0;
			continue;
		}

		key = "";
		pHubList->SetLineToRead(&lock,pipe,1024);
		pHubList->ReadAll();
		pHubList->ReadLineLocal();
		if(lock.size() > 6) {
			pos_space = lock.find(' ',6);
			if (pos_space != lock.npos) pos_space -= 6;
			lock = lock.substr(6,pos_space);
			cDCProto::Lock2Key(lock, key);
		}
		
		// Create the registration string
		os.str(mEmpty);
		os << "$Key " << key << pipe <<  mC.hub_name // removed pipe before name
			<< pipe << mC.hub_host
			<< pipe;
		__int64 hl_minshare = mC.min_share;
		if (mC.min_share_use_hub > hl_minshare) hl_minshare = mC.min_share_use_hub;
		if (mC.hublist_send_minshare) os << "[MINSHARE:" << StringFrom(hl_minshare) << "MB] ";
		os << mC.hub_desc
			<< pipe << mUserList.size()
			<< pipe << buf
			<< pipe;


		// send it
		if(Log(2)) LogStream() << os.str() << endl;
		pHubList->Write(os.str(), true);
		if(!pHubList->ok) os2 << " ..Error sending info\r\n" << endl;
		pHubList->Close();
		delete pHubList;
		pHubList = NULL;
		os2 << " .. ok\r\n";
	}

	os2 << "Done";
	CurHost = os2.str();
	if (NickForReply.size() > 0) {
		cUser * user = mUserList.GetUserByNick(NickForReply);
		if (user && user->mxConn) DCPublicHS(CurHost, user->mxConn);
	}
	return 1;
}

int cServerDC::RegisterInHublist(string host, int port, cConnDC *conn)
{
	string NickForReply;
	DCPublicHS("Registering the hub in hublists. This may take a while, please wait...", conn);
	if(conn && conn->mpUser) NickForReply = conn->mpUser->mNick;
	
	cThreadWork *work = new tThreadWork3T<cServerDC, string, int, string>( host, port, NickForReply, this, &cServerDC::DoRegisterInHublist);
	if ( mHublistReg.AddWork(work) )
	{
		return 1;
	}
	else 
	{
		delete work;
		return 0;
	}
}

/** No descriptions */
__int64 cServerDC::GetTotalShareSize()
{
	__int64 total =0;
	cUserCollection::iterator i;
	for(i=mUserList.begin(); i!= mUserList.end(); ++i) total += ((cUser *)(*i))->mShare;
	return total;
}

int cServerDC::WhoCC(string CC, string &dest, const string&separator)
{
	cUserCollection::iterator i;
	int cnt=0;
	cConnDC *conn;
	for(i=mUserList.begin(); i != mUserList.end(); ++i) {
		conn = ((cUser*)(*i))->mxConn;
		if(conn && conn->mCC == CC) {
			dest += (*i)->mNick;
			dest += separator;
			cnt++;
		}
	}
	return cnt;
}

/** fill in the list of nicks with given ip */
int cServerDC::WhoIP(unsigned long ip_min, unsigned long ip_max, string &dest, const string&separator, bool exact)
{
	cUserCollection::iterator i;
	int cnt=0;
	cConnDC *conn;
	for(i=mUserList.begin(); i!= mUserList.end(); ++i)
	{
		conn = ((cUser*)(*i))->mxConn;
		if(conn)
		{
			unsigned long num = cBanList::Ip2Num(conn->AddrIP());
			if(exact && (ip_min == num))
			{
				dest += (*i)->mNick;
				dest += separator;
				cnt++;
			}
			else if ((ip_min <= num) && (ip_max >= num))
			{
				dest += (*i)->mNick;
				dest += " (";
				dest += conn->AddrIP();
				dest += ")";
				dest += separator;
				cnt++;
			}
		}
	}
	return cnt;
}

void cServerDC::ReportUserToOpchat(cConnDC *conn, const string &Msg, bool ToMain)
{
	ostringstream os;

	os << Msg << " -- ";
	if (conn)
	{
		if(!mUseDNS && mC.report_dns_lookup) conn->DNSLookup();
		os << "IP=' " << conn->AddrIP() <<" ' Host=' " << conn->AddrHost() <<" ' ";
		if (conn->mpUser)
			os << "User=' " << conn->mpUser->mNick << " ' ";
		if (!ToMain && this->mOpChat)
		{
			this->mOpChat->SendPMToAll(os.str(), NULL);
		} else {
			static string ChatMsg;
			ChatMsg.erase();
			cDCProto::Create_Chat(ChatMsg, mC.opchat_name,os.str());
			this->mOpchatList.SendToAll(ChatMsg, false, true);
		}
	}
}

};


/*!
    \fn nDirectConnect::cServerDC::DCKickNick(cUser *OP, const string &Nick, const string &Reason,
    	int flags)
 */
void nDirectConnect::cServerDC::DCKickNick(ostream *use_os,cUser *OP, const string &Nick, const string &Reason,
	int flags)
{
	ostringstream ostr;
	cUser *user = mUserList.GetUserByNick(Nick);
	string NewReason(Reason);
	cKick OldKick;
	
	// is it a kickable user?
	if(user && user->mxConn &&
		(user->mClass + (int) mC.classdif_kick <= OP->mClass) &&
		(OP->Can(eUR_KICK, mTime.Sec())) &&
		//mClass >= eUC_OPERATOR) &&
		(OP->mNick != Nick))
	{
		if (user->mProtectFrom < OP->mClass)
		{
			if(flags & eKCK_Reason)
			{
				user->mToBan = false;
				// auto kickban
				if(mP.mKickBanPattern.Exec(Reason) >= 0)
				{
					unsigned u=0;
					string bantime;
					mP.mKickBanPattern.Extract(1,Reason,bantime);
					if(bantime.size())
					{
						ostringstream os;
						if(!(u=Str2Period(bantime,os))) DCPublicHS(os.str(),OP->mxConn);
					}
					if (u > mC.tban_max) u = mC.tban_max;
					if (
						(!u && OP->Can(eUR_PBAN, this->mTime)) ||
						((u > 0) &&
						 ((u > mC.tban_kick) && OP->Can(eUR_TBAN, this->mTime)) ||
						 (u <= mC.tban_kick)
						)
					  )
						user->mToBan = true;

					user->mBanTime = u;
					if ( mC.msg_replace_ban.size())
						mP.mKickBanPattern.Replace(0, NewReason, mC.msg_replace_ban);
				}
				mKickList->AddKick(user->mxConn, OP->mNick, &Reason, OldKick);

				if(Reason.size())
				{
					string omsg;
					ostr << "<" << OP->mNick << "> is kicking " << Nick << " because: " << NewReason;
					omsg=ostr.str();
					if(!mC.hide_all_kicks && !OP->mHideKick )
						SendToAll(omsg, OP->mHideKicksForClass ,int(eUC_MASTER));

					if(flags & eKCK_PM)
					{
						ostr.str(mEmpty);
						ostr << "You are being kicked because: " << NewReason;
						DCPrivateHS(ostr.str(), user->mxConn, &OP->mNick);
					}
				}
			}


			if(flags & eKCK_Drop)
			{
				// reply to kicker
				ostr.str(mEmpty);
				ostr << ((flags &eKCK_TBAN)?"Kicked user ":"Droping user ") << Nick
					<< " IP: " << user->mxConn->AddrIP();
				if(user->mxConn->AddrHost().length())
					ostr << " Host: " << user->mxConn->AddrHost();
				ostr << " .. ;)";


				// log it
				if(user->mxConn->Log(2))
					user->mxConn->LogStream() << "Kicked by " << OP->mNick << ", ban " << mC.tban_kick << "s"<< endl;
				if(OP->Log(3)) OP->LogStream() << "Kicking " << Nick << endl;

				bool Disconnect;
				mKickList->AddKick(user->mxConn, OP->mNick, NULL, OldKick);
				if (OldKick.mReason.size())
				{
					#ifndef WITHOUT_PLUGINS
					Disconnect = mCallBacks.mOnOperatorKicks.CallAll(OP, user, &OldKick.mReason);
					#endif
				} else {
					#ifndef WITHOUT_PLUGINS
					Disconnect = mCallBacks.mOnOperatorDrops.CallAll(OP, user);
					#endif
				}
				if (Disconnect) {
					user->mxConn->CloseNice(1000, eCR_KICKED);
					if (!(flags &eKCK_TBAN))
						ReportUserToOpchat(user->mxConn,OP->mNick + " dropped ", mC.dest_drop_chat);
				}
				else ostr << "\r\nsorry, I don't wanna kick him";

				// temp ban kicked user
				if (flags & eKCK_TBAN)
				{
					cBan Ban(this);
					cKick Kick;

					mKickList->FindKick(Kick, user->mNick, OP->mNick, 30, true, true);

					if(user->mToBan)
					{
						mBanList->NewBan(Ban, Kick, user->mBanTime, cBan::eBF_NICKIP );
						ostr << "\r\nand he is being banned " ;
						Ban.DisplayKick(ostr);
					} else {
						mBanList->NewBan(Ban, Kick, mC.tban_kick, cBan::eBF_NICKIP );
						ostr << "\r\nand he is being banned ";
						Ban.DisplayKick(ostr);
					}
					mBanList->AddBan(Ban);
				}
				if (!use_os)
					DCPublicHS(ostr.str(),OP->mxConn);
				else
					(*use_os) << ostr.str();
			}
		}
		else if(flags & eKCK_Drop)
		{
			ostr.str(mEmpty);
			ostr << "Error kicking user " << Nick << " because he's protected against all classes below (" << user->mProtectFrom << ")";
			DCPublicHS(ostr.str(),OP->mxConn);
		}
	}
}

