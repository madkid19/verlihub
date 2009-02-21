/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
*   netcelli@verlihub-project.org                                         *
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

#include "ctriggers.h"
#include "cconfigitembase.h"
#include "cserverdc.h"
#include <time.h>

namespace nDirectConnect {
namespace nTables {

  /**
  
  Class constructor.
  
  @param[in] server A pointer to cServerDC object
  */
  
cTriggers::cTriggers( cServerDC *server ) :
	tMySQLMemoryList<cTrigger, cServerDC>(server->mMySQL, server, "file_trigger")
{
	SetClassName("nDC::cTriggers");
}

  /**

  Create columns for file_trigger table. The columns are:
  - command (CHAR - primary key - length 15) : the command that is used to show a trigger message
  - send_as (CHAR - length 15) : the optional sender name
  - def (TEXT) : the text of the trigger if it is not contained in a file
  - descr (TEXT) : the trigger's description
  - min_class (INT - length 2) : the min class to trigger
  - max_class (INT - length 2) : the max class to trigger
  - flags (INT - length 2) : the mask in order to speficy trigger's options
  - seconds (INT - length 15) :  timeout for trigger. 0 means the timer is not actived
  */

void cTriggers::AddFields()
{
	AddCol("command", "varchar(15)", "", false, mModel.mCommand);
	AddPrimaryKey("command");
	AddCol("send_as", "varchar(15)", "hub-security", true, mModel.mSendAs);
	AddCol("def", "text", "", true, mModel.mDefinition);
	AddCol("descr", "text", "", true, mModel.mDescription);
	AddCol("min_class", "int(2)", "", true, mModel.mMinClass);
	AddCol("max_class", "int(2)", "10", true, mModel.mMaxClass);
	AddCol("flags", "int(2)", "0", true, mModel.mFlags);
	AddCol("seconds", "int(15)", "0", true, mModel.mSeconds);
	//AddCol("last_trigger", "int(15)", "0", true, mModel.mLastTrigger);
	mMySQLTable.mExtra = "PRIMARY KEY(command)";
	SetBaseTo(&mModel);
}

  /**

  Check if timeout for a trigger is expired. If so run it

  @param[in] now The current Unix Time in seconds
  */

void cTriggers::OnTimer(long now)
{
	 istringstream is;
	 iterator it;
	 cTrigger *trigger;
	 for (it= begin(); it != end(); ++it)
	 {
		trigger = *it;
		if(!trigger->mSeconds) continue;
		long next = trigger->mLastTrigger + trigger->mSeconds;
		if (next < now) {
		    trigger->mLastTrigger = now;
		    trigger->DoIt(is, NULL, *mOwner, true);
		}
	 }
}

  /**

  Trigger all existing trigger by a given mask. There are 2 cases when this method is called:
  1) When a user logs in
  2) When +help is triggered

  @param[in] FlagMask The mask
  @param[in] conn A pointer to a user's connection
  */

void cTriggers::TriggerAll(int FlagMask, cConnDC *conn)
{
	istringstream is;
	iterator it;
	cTrigger *trigger;
	for (it= begin(); it != end(); ++it)
	{
		trigger = *it;
		if (trigger->mFlags & FlagMask)
			trigger->DoIt(is, conn, *mOwner);
	}
}

  /**

  Compare 2 cTrigger objects to determine if they have the same key. This method is used to forbid duplicated entry when when a new trigger is added

  @param[in,out] D1 The frist trigger
  @param[in,out] D2 The second trigger
  */

bool cTriggers::CompareDataKey(const cTrigger &D1, const cTrigger &D2)
{
	return D1.mCommand == D2.mCommand;
}

  /**

  Run a trigger by a given command

  @param[in,out] conn The user's connection
  @param[in,out] cmd The sent command by the user
  @param[in,out] cmd_line The stream or command line
  @param[in,out] server Reference to cServerDC
  @return False on failure or true on success
  */

bool cTriggers::DoCommand(cConnDC *conn, const string &cmd, istringstream &cmd_line, cServerDC &server)
{
	cTrigger * curTrigger;
	for( int i = 0; i < Size(); ++i )
	{
		curTrigger = (*this)[i];
		if(
			curTrigger->mMinClass <= conn->mpUser->mClass &&
			cmd == curTrigger->mCommand
		)
		{
			if(Log(3)) LogStream() << "trigger found " << cmd << endl;
			return curTrigger->DoIt (cmd_line, conn, server);
		}
	}
	return false;
}

  /**

  Class constructor

  */

cTriggerConsole::cTriggerConsole(cDCConsole *console) : tTriggerConsoleBase(console)
{
	this->AddCommands();
}

  /**

  Class destructor
  
  */

cTriggerConsole::~cTriggerConsole()
{}

  /**

  Show help to the user if he is wrong when he types a command

  @param[in] cmd The type of the command (list, add, mod or del)
  @param[in,out] os The stream where to store the output
  
  */

void cTriggerConsole::GetHelpForCommand(int cmd, ostream &os)
{
	string help_str;
	switch(cmd)
	{
		case eLC_LST: 
		help_str = "!lsttrigger\r\nGive a list of triggers"; 
		break;
		case eLC_ADD: 
		case eLC_MOD:
		help_str = "!(add|mod)trigger <trigger>"
			"[ -d <\"definition\">]"
			"[ -h <help_desc>]"
			"[ -f <flags>]"
			"[ -n <sendas_nick>]"
			"[ -c <min_class>]"
			"[ -C <max_class>]";
		break;
		case eLC_DEL:
		help_str = "!deltrigger <trigger>"; break;
		default: break;
	}
	cDCProto::EscapeChars(help_str,help_str);
	os << help_str;
}

  /**

  Show a complete help when user type (+|!)htrigger

  @param[in,out] os The stream where to store the output

  */

void cTriggerConsole::GetHelp(ostream &os)
{
	string help;
	
	help = "Available trigger flags are:\r\n";
	help += "0  \tSend to main chat (visible to user only)\r\n";
	help += "1  \tExecute command\r\n";
	help += "2  \tMessage is sent to PM\r\n";
	help += "4  \tAutomatically trigger when user logs in\r\n";
	help += "8  \tTrigger on +help/!help command\r\n";
	help += "16\tThe definition is the text\r\n";
	help += "32\tAllow replacing of variables\r\n";
	help += "64\tMessage is sent to everyone in the main chat window\r\n";
		
	help += "\r\nRemember to make the sum of the selected flags above\n(for more help please visit http://www.verlihub-project.org/doku.php?id=configuring_your_hub#faq_rules_help_motd_file_trigger_encoding )\r\n";
	cDCProto::EscapeChars(help,help);
	os << help;
}

  /**

  Return the regex for a given command

  @param[in] cmd The type of the command (list, add, mod or del)

  */

const char * cTriggerConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_MOD:
			return "^(\\S+)("
			"( -d ?(\")?((?(4)[^\"]+?|\\S+))(?(4)\"))?|" //[ -d<definition>|"<<definition>>]"
			"( -h ?(\")?((?(7)[^\"]+?|\\S+))(?(7)\"))?|" //[ -h(<help>|"<<help>>")]
			"( -f ?(-?\\d+))?|" //[ -f<flags>]
			"( -n ?(\\S+))?|" // [ -n<sendas_nick>]
			"( -c ?(-?\\d+))?|" //[ -c<min_class>]
			"( -C ?(-?\\d+))?|" //[ -c<max_class>]
			"( -t ?(\\S+))?|" //[ -t<timeout>]
			")*\\s*$"; // the end of message
		case eLC_DEL:
			return "(\\S+)";
		default: return "";break;
	};
}

  /**

  Read, extract data from a trigger command and save the trigger

  @param[in] cmd cfBase object
  @param[in] CmdID Not used here
  @param[in,out] data The trigger to add or modify

  */

bool cTriggerConsole::ReadDataFromCmd(cfBase *cmd, int CmdID, cTrigger &data)
{
	enum {eADD_ALL, eADD_CMD, eADD_CHOICE,
		eADD_DEFp, eADD_QUOTE, eADD_DEF, 
		eADD_DESCp, eADD_QUOTE2, eADD_DESC, 
		eADD_FLAGSp, eADD_FLAGS,
		eADD_NICKp, eADD_NICK,
		eADD_CLASSp, eADD_CLASS,
		eADD_CLASSXp, eADD_CLASSX,
		eADD_TIMEOUTp, eADD_TIMEOUT
		};
	
	cmd->GetParStr(eADD_CMD,data.mCommand);
	cmd->GetParUnEscapeStr(eADD_DEF,data.mDefinition);
	cmd->GetParStr(eADD_DESC,data.mDescription);
	cmd->GetParStr(eADD_NICK,data.mSendAs);
	cmd->GetParInt(eADD_FLAGS, data.mFlags);
	cmd->GetParInt(eADD_CLASS, data.mMinClass);
	cmd->GetParInt(eADD_CLASSX, data.mMaxClass);
	string sTimeout("0");
	cmd->GetParStr(eADD_TIMEOUT,sTimeout);
	if(sTimeout == "0")
		data.mSeconds = 0;
	else
		data.mSeconds = mOwner->mServer->Str2Period(sTimeout,*cmd->mOS);
	

	if (!mOwner->mServer->mDBConf.allow_exec_mod && (data.mFlags & cTrigger::eTF_EXECUTE)) {
		*cmd->mOS << "Execute command flag is disabled from config";
		return false;
	}
	return true;
}

  /**

  Return a pointer to cTriggers object

  @return cTriggers pointer

  */

cTriggers *cTriggerConsole::GetTheList()
{
	return mOwner->mTriggers;
}

  /**

  Return the command suffix

  @return The string of suffix

  */

const char *cTriggerConsole::CmdSuffix(){ return "trigger";}

  /**

  Return the command prefix

  @todo Use config variable for prefix  (cmd_start_user)
  @return The string of prefix

  */

const char *cTriggerConsole::CmdPrefix(){ return "!";}

  /**

  The header of the message to show when !lsttrigger is sent

  @param[in] os The stream where to store the message

  */

void cTriggerConsole::ListHead(ostream *os)
{
	*os << "Existing triggers are:\r\n";
}

  /**

  Return true if a given connection has rights to show, add, edit and remove triggers

  @todo Add mod_trigger_class in config
  @param[in] conn The connection
  @param[in] cmd Not used here

  */

bool cTriggerConsole::IsConnAllowed(cConnDC *conn,int cmd)
{
	return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
}

};
};

