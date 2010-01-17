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
#ifndef CDCCONSOLE_H
#define CDCCONSOLE_H
#include <string>
#include "ctriggers.h"
#include "ccustomredirects.h"
#include "cdcclients.h"
#include "cobj.h"
#include "cban.h"
#include "ccmdr.h"
#include "cdccommand.h"
#include "cinfoserver.h"
#include "tlistconsole.h"
#include "cconntypes.h"

using namespace std;
using namespace nCmdr;

namespace nDirectConnect {

namespace nTables {
	class cTriggers;
	class cTriggerConsole;
	class cRedirects;
	class cDCClients;
	class cRedirectConsole;
	class cDCClientConsole;
}
using namespace nTables;
using namespace nInfo;

class cConnDC;
class cServerDC;

/**
  * \brief Command interpreter for operators and normal users
  * @author Daniel Muller
  * Contains all functions that are executed for !commands or +commands (except the triggers)
  */

class cDCConsole : public cDCConsoleBase
{
public:
	cDCConsole(cServerDC *, cMySQL &);
	virtual ~cDCConsole();
	/** act on op's command */
	virtual int OpCommand(const string &, cConnDC*);
	/** act on usr's command */
	virtual int UsrCommand(const string & , cConnDC * );
	/** get user's ip */
	int  CmdGetip(istringstream &, cConnDC *);
	/** get user's host */
	int CmdGethost(istringstream & , cConnDC * );
	/** get user's host and ip */
	int CmdGetinfo(istringstream &cmd_line , cConnDC *conn );
	/** quit program */
	int CmdQuit(istringstream &, cConnDC * conn,int code);
	/** show all variables along with their values */
	int CmdGetconfig(istringstream & cmd_line, cConnDC * conn);
	/** send help message corresponding to connection */
	int CmdHelp(istringstream & cmd_line, cConnDC * conn);
	int CmdCCBroadcast(istringstream & cmd_line, cConnDC * conn,int cl_min, int cl_max);
	int CmdRegMyPasswd(istringstream & cmd_line, cConnDC * conn);
	int CmdUInfo(istringstream & cmd_line, cConnDC * conn);
	int CmdRInfo(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +myinfo. This command returns the information about the user.
	    * @param cmd_line The stream. Not used.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1.
	 */
	int CmdMyInfo(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +myip. This command returns the IP address of the user.
	    * @param cmd_line The stream. Not used.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1.
	 */
	int CmdMyIp(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +me <message>. This command lets an user to talk in 3rd person.
	    * @param cmd_line The stream that contains the message.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return 0 if an error occured or 1 otherwise.
	 */	
	int CmdMe(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +regme <password>. This command sends a report to OpChat in order to ask registration to Hub Operator or register an user automatically if autoreg_class config variable is set properly.
	    * @param cmd_line The stream that contains the password.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return 0 if an error occured or 1 otherwise.
	 */
	int CmdRegMe(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +kick <user> <reason>. This command will kick an user with the given reason.
	    * @param cmd_line The stream that contains user and the reason.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1.
	 */
	int CmdKick(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle +chat and +nochat. These two commands are used to talk in mainchat.
	    * @param cmd_line The stream. Not Used.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @param switchon. If set to true add the user to mChatUsers list that contains the users that can talk in mainchat. False value does the opposite.
	    * @return 0 if the user does not exist or 1 otherwise.
	 */
	int CmdChat(istringstream & cmd_line, cConnDC * conn, bool switchon);
	
	/**
	    * Handle !hideme or !hm <class>. This command will hide any commands for users with class lower than <class>.
	    * @param cmd_line The stream that contains the class.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdHideMe(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle !ul(imit) <users> <time>. This command will progressively increase the max allowed users in the hub in <time>. The time must be speficied in minutes; this value can be ommited and default value is 60 minutes.
	    * @param cmd_line The stream that contains the number of users and the time.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdUserLimit(istringstream & cmd_line, cConnDC * conn);
	
	/**
	    * Handle !unhidekick <user>. This command will un-hide kick made by <user>, previously hidden by using !hidekick <user> command.
	    * @param cmd_line The stream that contains the username.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdUnHideKick(istringstream &cmd_line, cConnDC *conn);
	
	/**
	    * Handle !hidekick <user>. This command will hide kick made by <user> until he reconnects to the hub.
	    * @param cmd_line The stream that contains the username.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdHideKick(istringstream &cmd_line, cConnDC *conn);

	/**
	    * Handle !class <nick> <new_class>. This command will change temporarily the user's class.
	    * @param cmd_line The stream that contains the data like the username and the class.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdClass(istringstream &cmd_line, cConnDC *conn);
	
	
	/**
	    * Handle !protect <user> <class> command. This command protects an user against another one with lower class than <class>.
	    * @param cmd_line The stream that contains the data like the username to protect and the class.
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdProtect(istringstream &cmd_line, cConnDC *conn);
	
	/**
	    * Handle !reload command to reload VerliHub cache like triggers, custom redirects, configuration and reglist.
	    * @param cmd_line The stream. Not used
	    * @param conn Pointer to user's connection which to send the result message.
	    * @return Always 1
	 */
	int CmdReload (istringstream &cmd_line, cConnDC *conn);
	
	/**
	    * Handle !commands or !cmds to show the list of available and register commands in VerliHub console.
	    * @param cmd_line The stream. Not used
	    * @param conn Pointer to user's connection which to send the list of command.
	    * @return Always 1
	 */
	int CmdCmds (istringstream &cmd_line, cConnDC *conn);
	
	/**
	    * Handle !topic <msg> command to set the hub topic for the hub. The topic will be appended after the hub name and look like this: <HUB NAME> - <TOPIC>
	    * @param cmd_line The stream the contains the topic.
	    * @param conn Pointer to user's connection which set the hub topic. It is used to send error message.
	    * @return Always 1
	 */
	 int CmdTopic(istringstream & cmd_line, cConnDC * conn); 

	static cPCRE mIPRangeRex;
	static bool GetIPRange(const string &range, unsigned long &from, unsigned long &to);

	typedef cDCCommand::sDCCmdFunc cfDCCmdBase;
	typedef cDCCommand cDCCmdBase;

	enum{ eCM_CMD, eCM_BAN, eCM_GAG, eCM_TRIGGER, eCM_CUSTOMREDIR, eCM_DCCLIENT, eCM_SET, eCM_REG, eCM_INFO, eCM_RAW, eCM_WHO, eCM_KICK, eCM_PLUG, eCM_REPORT, eCM_BROADCAST, eCM_CONNTYPE, eCM_TRIGGERS, eCM_GETCONFIG, eCM_CLEAN };
	
	cServerDC *mServer;

	cTriggers *mTriggers;
	
	cRedirects *mRedirects;
	
	cDCClients *mDCClients;
private:
	cCmdr mCmdr;
	cCmdr mUserCmdr;
	struct cfBan : cfDCCmdBase { virtual bool operator()(); } mFunBan;
	cDCCmdBase mCmdBan;
	struct cfGag : cfDCCmdBase { virtual bool operator()(); } mFunGag;
	cDCCmdBase mCmdGag;
	struct cfTrigger : cfDCCmdBase { virtual bool operator()(); } mFunTrigger;
	cDCCmdBase mCmdTrigger;
	struct cfSetVar : cfDCCmdBase { virtual bool operator()(); } mFunSetVar;
	cDCCmdBase mCmdSetVar;
	struct cfRegUsr : cfDCCmdBase { virtual bool operator()(); } mFunRegUsr;
	cDCCmdBase mCmdRegUsr;
	struct cfRaw : cfDCCmdBase { virtual bool operator()(); } mFunRaw;
	cDCCmdBase mCmdRaw;
	struct cfCmd : cfDCCmdBase { virtual bool operator()(); } mFunCmd;
	cDCCmdBase mCmdCmd;
	struct cfWho : cfDCCmdBase { virtual bool operator()(); } mFunWho;
	cDCCmdBase mCmdWho;
	struct cfKick : cfDCCmdBase { virtual bool operator()(); } mFunKick;
	cDCCmdBase mCmdKick;
	struct cfInfo : cfDCCmdBase {
		virtual bool operator()();
		cInfoServer mInfoServer;
	} mFunInfo;
	cDCCmdBase mCmdInfo;
	cDCCmdBase mCmdPlug;
	struct cfPlug : cfDCCmdBase { virtual bool operator()(); } mFunPlug;
	struct cfReport : cfDCCmdBase { virtual bool operator()(); } mFunReport;
	cDCCmdBase mCmdReport;
	struct cfBc : cfDCCmdBase { virtual bool operator()(); } mFunBc;
	cDCCmdBase mCmdBc;
	struct cfGetConfig : cfDCCmdBase { virtual bool operator()(); } mFunGetConfig;
	cDCCmdBase mCmdGetConfig;
	struct cfClean : cfDCCmdBase { virtual bool operator()(); } mFunClean;
	cDCCmdBase mCmdClean;
	
	// redirection consoles to other console
	struct cfRedirToConsole : cfDCCmdBase {
		virtual bool operator()();
		tConsoleBase<cDCConsole> *mConsole;
	} mFunRedirConnType, mFunRedirTrigger, mFunCustomRedir, mFunDCClient;
	cDCCmdBase mCmdRedirConnType;
	cDCCmdBase mCmdRedirTrigger;
	cDCCmdBase mCmdCustomRedir;
	cDCCmdBase mCmdDCClient;
	cConnTypeConsole mConnTypeConsole;
	cTriggerConsole *mTriggerConsole;
	cRedirectConsole *mRedirectConsole;
	cDCClientConsole *mDCClientConsole;
};

};

#endif
