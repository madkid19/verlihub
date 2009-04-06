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

#ifndef CSERVERDC_H
#define CSERVERDC_H
#include "casyncsocketserver.h"
#include "cmysql.h"
#if defined _WIN32
#include <winsock2.h>
#endif
#include <fstream>
#include "cuser.h"
#include "cmessagedc.h"
#include "cconfigbase.h"
#include "cdcconf.h"
#include "cdbconf.h"
#include "cdcproto.h"
#include "csetuplist.h"
#include "ctempfunctionbase.h"
#if HAVE_LIBGEOIP
#include "cgeoip.h"
#endif
#include "cusercollection.h"
#include "cvhpluginmgr.h"
#include "cmeanfrequency.h"
#include "cworkerthread.h"

using namespace std;
using nMySQL::cMySQL;
#if HAVE_LIBGEOIP
using nUtils::cGeoIP;
#endif
using namespace ::nPlugin;
using namespace nThreads;

#define USER_ZONES 6

namespace nDirectConnect
{

namespace nEnums {

typedef enum
{
	eVN_OK,
	eVN_CHARS,
	eVN_SHORT,
	eVN_LONG,
	eVN_USED,
	eVN_BANNED,
	eVN_PREFIX,
	eVN_NOT_REGED_OP,
	//eVN_RESERVED
} tVAL_NICK;

typedef enum
{
	eVI_OK,
	eVI_UNKNOWN,
	eVI_PRIVATE,
	eVI_BAN,
	eVI_T_BAN,
	eVI_BAN_RANGE,
	eVI_BAN_HOST
} tVAL_IP;

typedef enum
{
	eMA_PROCEED,
	eMA_LIMITED,
	eMA_LATER,
	eMA_WARNING,
	eMA_IGNORE,
	eMA_HANGUP,
	eMA_HANGUP1,
	eMA_TBAN,
	eMA_ERROR

} tMsgAct;

typedef enum
{
	eSL_COOL,	// no problem
	eSL_HURRY,	// starting getting full
	eSL_SQEEZY,	// we are full, but still a little bit alive
	eSL_CRITICAL, // almost dead
	eSL_SYSTEM_DOWN // this actually never happens ;o)
} tSysLoad;

enum
{
	eULO_NONE = 0,
	eULO_GETINFO = 1
};

enum
{
   eCR_DEFAULT = 0,  //< default value, means not closed or for unknown reason
   eCR_INVALID_USER, //< means bad nick, or banned nick or ip or whatever
   eCR_CHAT_NICK, //< means user used different nick in chat
   eCR_KICKED, //< operator kicked user
   eCR_FORCEMOVE, //< $OpForceMove (the redir menu item)
   eCR_QUIT, //< user quits himself
   eCR_HUB_LOAD, //< critical hub load, no new users accepted
   eCR_TIMEOUT, //< some kind of timeout
   eCR_TO_ANYACTION, //< user did nothing for too long time
   eCR_USERLIMIT, //< user limit exceeded for this user
   eCR_SHARE_LIMIT, //< min or max share limit
   eCR_TAG_NONE, //< no tags in description (or badly parsed)
   eCR_TAG_INVALID, //< tags not validated (general), slots or hubs or limiter, or version etc..
   eCR_PASSWORD, //< wrong password
   eCR_LOGIN_ERR, //< error in login sequence
   eCR_SYNTAX, // < syntax error in some message
   eCR_INVALID_KEY,
};
};

namespace nTables{
	class cConnTypes;
	class cBanList;
	class cUnBanList;
	class cPenaltyList;
	class cRegList;
	class cKickList;
	class cDCConf;
};

// forward declarations
using namespace ::nDirectConnect::nEnums;
using namespace nConfig;
using namespace nUtils;
using namespace nDirectConnect;
using namespace nServer;
using namespace nProtocol;
using namespace ::nDirectConnect::nTables;
using namespace ::nDirectConnect::nPlugin;

using ::nDirectConnect::nTables::cDCConf;

class cConnDC;
class cDCConnFactory;
class cUser;
class cUserRobot;
class cChatRoom;
class cDCConsole;

using ::nDirectConnect::cDCConsole;

/**A Direct Connect Verlihub server
  *@author Daniel Muller
  * the one ;)
  */
class cServerDC : public cAsyncSocketServer
{
	friend class ::nDirectConnect::cConnDC;
	friend class ::nDirectConnect::cDCConnFactory;
	friend class ::nDirectConnect::cDCConsole;
	friend class ::nDirectConnect::nProtocol::cDCProto;
	friend class ::nDirectConnect::cDCConf;
	friend class ::nDirectConnect::nTables::cRegList;
	friend class ::nDirectConnect::nTables::cDCBanList;
	friend class ::nDirectConnect::cUser;
public:
	string mConfigBaseDir;
	cDBConf mDBConf; // database configuration
	// mysql database connection
	cMySQL mMySQL;
	cDCConf mC; // configuration
	cDCLang mL; // language
	cSetupList mSetupList; // setup loader
	cDCProto mP; // protocol
	class cDCConsole *mCo; // console
	class cRegList *mR; // reg users
	cPenaltyList *mPenList; // penalties and temp rights
	/** banlist */
	cBanList *mBanList;
	cUnBanList *mUnBanList;
	cKickList *mKickList;
	cChatRoom *mOpChat;
	cConnTypes *mConnTypes;
	string mExecPath; // the filename of this process
public:
	cServerDC(string CfgBase = string("./.verlihub"), const string &ExecPath= "");
	virtual ~cServerDC();
	/** override default listen */
	virtual int StartListening(int OverrideDefaultPort=0);
	/** sends a public chat message to a given connection */
	int DCPublic(const string &from, const string &msg,class cConnDC *conn);
	int DCPublicToAll(const string &from, const string &txt, int min_class=1, int max_class=10);
	/** send a hub security public message */
	int DCPublicHS(const string &text, cConnDC *conn);
	/** tells if a user with such nick can login into the hub */
	tVAL_NICK ValidateNick(const string &nick, bool registered);
	/** return 1 if user is valid and can go in
	return 0 otherwise, and also close the connection */
	int ValidateUser(cConnDC *conn, const string &nick);

	/** try to add user into userlist, return true on success */
	bool AddToList(cUser *usr);
	bool AddRobot(cUserRobot *robot);
	bool DelRobot(cUserRobot *robot);
	/** removes user with given nick from uselist, return true on success */
	bool RemoveNick(cUser *);
	/** find user with given nick, if not there return NULL */
	//cUser * FindUserNick(const string &nick);
	/** send the same data to every user */
	void SendToAll(string &str, int cm,int cM);
	/** sends to every user data composed by start+nick+end */
	int SendToAllWithNick(const string &start,const string &end, int cm,int cM);
	void SendToAllWithNickCC(const string &start,const string &end, int cm,int cM, const string &cc_zone);
	/** The message filter, decides what to do with the message */
	tMsgAct Filter( tDCMsg msg, cConnDC * conn );
	int SaveFile(const string &file, const string &text);
	/** this is called every period of time */
	virtual int OnTimer(cTime &now);
	/** converts number with units to a time period in seconds */
	unsigned Str2Period(const string &, ostream &);
	// send HubSecurity Message to public chat to all users
	void DCPublicHSToAll(const string &text);
	/** send a hub security public message */
	int DCPrivateHS(const string & text, cConnDC * conn,string *from = NULL);
	/** does the registration on given hublist */
	int RegisterInHublist(string host, int port, cConnDC *conn);
	int DoRegisterInHublist(string host, int port, string NickForReply);
	/** No descriptions */
	__int64 GetTotalShareSize();
	/** fill in the list of nicks with given ip */
	int WhoIP(unsigned long ip_min, unsigned long ip_max, string &dest, const string&separator, bool exact=true);
	int WhoCC(string CC, string &dest, const string&separator);
	/** buffer to send to all */
	string mSendAllBuf;
	void ReportUserToOpchat(cConnDC *, const string &Msg, bool ToMain = false);
	bool ShowUserToAll(cUserBase *user);

public: // Public attributes
	static cServerDC *sCurrentServer;
	/** system load indicator */
	tSysLoad mSysLoad;
	/** last op that used the broadcast function */
	string LastBCNick;
	/** string containing all myinfos in a row */
	string mNickInfoString;
	/** network output log */
	ofstream mNetOutLog;

	/** hublist registration thread */
	cWorkerThread mHublistReg;

	cMeanFrequency<unsigned long, 10> mUploadZone[USER_ZONES+1];
	
	string externalIP;

	/********** typedefs */

	/** temporary time functions */
	/** trigger functions type */
	typedef int tDC_MsgFunc(cMessageDC * msg, cConnDC * conn);
	typedef vector<cTempFunctionBase *> tTmpFunc;
	typedef tTmpFunc::iterator tTFIt;

	/** list of temporary functions */
	tTmpFunc mTmpFunc;

	/** the userlist */
	typedef cUserCollection::tHashType tUserHash;
	cCompositeUserCollection mUserList;
	cUserCollection mInProgresUsers;
	
	enum { eKCK_Drop = 1, eKCK_Reason = 2, eKCK_PM = 4, eKCK_TBAN = 8};
	void DCKickNick(ostream *, cUser *OP, const string &Nick, const string &Reason, int flags);
public: //Private attributes
	/** secondary lists of users */
	cCompositeUserCollection mOpList;
	cUserCollection mOpchatList;
	cUserCollection mActiveUsers;
	cUserCollection mHelloUsers;
	cUserCollection mChatUsers;
	cUserCollection mRobotList;
	unsigned mUsersPeak;

#if HAVE_LIBGEOIP
	/** country code database lib */
	static cGeoIP sGeoIP;
#endif
protected: // Protected methods
	/** create somehow a string to get line for given connection, ad return th pointer */
	virtual string * FactoryString(cAsyncConn * );
	/** treat message for given connection */
	void OnNewMessage(cAsyncConn * , string * );
	/** check if user is logged in with same nick, eventually decide if old conn should be closed, return tru if conn can login */
	bool VerifyUniqueNick(cConnDC *conn);
	/** start the user becomiing in the userlist */
	bool BeginUserLogin(cConnDC *conn);
	/** is called when user is about to get into the userslist */
	void DoUserLogin(cConnDC *conn);
	/** Things done when the protocol login is done */
	void AfterUserLogin(cConnDC *conn);
	/** close a given connection with sending a message as a reason, and do it at least in to_usec microseconds */
	void ConnCloseMsg(cConnDC *conn, const string &msg, int to_msec=4000, int Reason = eCR_DEFAULT);
	/** send the hello messahe, on which response we'll get MyINFO */
	int DCHello(const string & nick, cConnDC * conn, string *info=NULL);
	/** return true if the condition of minimal delay is verified
	    and modify the timestamp */
	bool MinDelay(cTime &what, int min);
	bool MinDelayMS(cTime &what, long min);
	/** return true if accept is allowed - override */
	bool AllowNewConn();
	/** return negative if conn should be removed */
	int OnNewConn(cAsyncConn * );

public: // Protected attributes
	/** the number of allowed users */
	int mUserCount[USER_ZONES+1];
	int mUserCountTot;
	__int64 mTotalShare;
	/** when hub started */
	cTime mStartTime;
	cTimeOut mSlowTimer;
	cTimeOut mHublistTimer;
	cTimeOut mReloadcfgTimer;
	cVHPluginMgr mPluginManager;
private:
	struct sCallBacks
	{

		sCallBacks(cVHPluginMgr * mgr) :
			mOnNewConn(mgr, "VH_OnNewConn", &cVHPlugin::OnNewConn),
			mOnCloseConn(mgr, "VH_OnCloseConn", &cVHPlugin::OnCloseConn),
			mOnUnknownMsg( mgr, "VH_OnUnknownMsg", &cVHPlugin::OnUnknownMsg),
			mOnParsedMsgAny( mgr, "VH_OnParsedMsgAny", &cVHPlugin::OnParsedMsgAny),
			mOnParsedMsgPM(mgr, "VH_OnParsedMsgPM", &cVHPlugin::OnParsedMsgPM),
			mOnParsedMsgChat(mgr, "VH_OnParsedMsgChat", &cVHPlugin::OnParsedMsgChat),
			mOnParsedMsgSearch(mgr, "VH_OnParsedMsgSearch", &cVHPlugin::OnParsedMsgSearch),
			mOnParsedMsgMyINFO(mgr, "VH_OnParsedMsgMyINFO", &cVHPlugin::OnParsedMsgMyINFO),
			mOnParsedMsgValidateNick(mgr, "VH_OnParsedMsgValidateNick", &cVHPlugin::OnParsedMsgValidateNick),
			mOnParsedMsgConnectToMe(mgr, "VH_OnParsedMsgConnectToMe", &cVHPlugin::OnParsedMsgConnectToMe),
			mOnParsedMsgRevConnectToMe(mgr, "VH_OnParsedMsgRevConnectToMe", &cVHPlugin::OnParsedMsgRevConnectToMe),
			mOnOperatorCommand(mgr, "VH_OnOperatorCommand", &cVHPlugin::OnOperatorCommand),
			mOnOperatorKicks( mgr, "VH_OnOperatorKicks", &cVHPlugin::OnOperatorKicks),
			mOnOperatorDrops( mgr, "VH_OnOperatorDrops", &cVHPlugin::OnOperatorDrops),
			mOnUserCommand(mgr, "VH_OnUserCommand", &cVHPlugin::OnUserCommand),
			mOnUserLogin  (mgr, "VH_OnUserLogin",   &cVHPlugin::OnUserLogin ),
			mOnUserLogout  (mgr, "VH_OnUserLogout",   &cVHPlugin::OnUserLogout ),
			mOnValidateTag(mgr, "VH_OnValidateTag", &cVHPlugin::OnValidateTag),
			mOnTimer( mgr, "VH_OnTimer", &cVHPlugin::OnTimer ),
			mNickListNicks( mgr, "VH_OnCreateUserNickList", &cVHPlugin::OnCreateUserNickList),
			mNickListInfos( mgr, "VH_OnCreateUserInfoList", &cVHPlugin::OnCreateUserInfoList),
			mOpListNicks( mgr, "VH_OnCreateOpList", &cVHPlugin::OnCreateOpList),
			mOnNewReg(mgr, "VH_OnNewReg", &cVHPlugin::OnNewReg),
			mOnDelReg(mgr, "VH_OnDelReg", &cVHPlugin::OnDelReg),
			mOnUpdateClass(mgr, "VH_OnUpdateClass", &cVHPlugin::OnUpdateClass),
			mOnHubName(mgr, "VH_OnHubName", &cVHPlugin::OnHubName),
			mOnNewBan(mgr, "VH_OnNewBan", &cVHPlugin::OnNewBan),
			mOnUnBan(mgr, "VH_OnUnBan", &cVHPlugin::OnUnBan)
			
		{};
		cVHCBL_Connection	mOnNewConn;
		cVHCBL_Connection	mOnCloseConn;
		cVHCBL_Message		mOnUnknownMsg;
		cVHCBL_Message		mOnParsedMsgAny;
		cVHCBL_Message		mOnParsedMsgPM;
		cVHCBL_Message		mOnParsedMsgChat;
		cVHCBL_Message		mOnParsedMsgSearch;
		cVHCBL_Message		mOnParsedMsgMyINFO;
		cVHCBL_Message		mOnParsedMsgValidateNick;
		cVHCBL_Message		mOnParsedMsgConnectToMe;
		cVHCBL_Message		mOnParsedMsgRevConnectToMe;
		cVHCBL_ConnText		mOnOperatorCommand;
		cVHCBL_UsrUsrStr	mOnOperatorKicks;
		cVHCBL_UsrUsr		mOnOperatorDrops;
		cVHCBL_ConnText		mOnUserCommand;
		cVHCBL_User		mOnUserLogin;
		cVHCBL_User		mOnUserLogout;
		cVHCBL_ConnTag		mOnValidateTag;
		cVHCBL_Simple 		mOnTimer;
		cVHCBL_String		mNickListNicks;
		cVHCBL_String		mNickListInfos;
		cVHCBL_String		mOpListNicks;
		cVHCBL_Strings		mOnHubName;
		cVHCBL_StringInt	mOnNewReg;
		cVHCBL_StrIntInt	mOnUpdateClass;
		cVHCBL_StringInt	mOnDelReg;
		cVHCBL_Ban		mOnNewBan;
		cVHCBL_StrStrStr	mOnUnBan;
	};

	sCallBacks mCallBacks;
};

};
#endif
