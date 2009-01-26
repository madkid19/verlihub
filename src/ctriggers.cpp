/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ctriggers.h"
#include "cconfigitembase.h"
#include "cserverdc.h"
#include <time.h>

namespace nDirectConnect {
namespace nTables {

cTriggers::cTriggers( cServerDC *server ) :
	tMySQLMemoryList<cTrigger, cServerDC>(server->mMySQL, server, "file_trigger")
{
	SetClassName("nDC::cTriggers");
}

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
	AddCol("last_trigger", "int(15)", "0", true, mModel.mLastTrigger);
	mMySQLTable.mExtra = "PRIMARY KEY(command)";
	SetBaseTo(&mModel);
}

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
		cout << "[::] "<< trigger->mCommand << " Next: " << next << " Now: " << now << endl;
		if (next < now) {
		    trigger->mLastTrigger = now;
		    //UpdateData(*trigger);
		    trigger->DoIt(is, NULL, *mOwner, true);
		}
	 }
}

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

bool cTriggers::CompareDataKey(const cTrigger &D1, const cTrigger &D2)
{
	return D1.mCommand == D2.mCommand;
}

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

cTriggerConsole::cTriggerConsole(cDCConsole *console) : tTriggerConsoleBase(console)
{
	this->AddCommands();
}

cTriggerConsole::~cTriggerConsole()
{}

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
	string sTimeout;
	cmd->GetParStr(eADD_TIMEOUT,sTimeout);
	if(sTimeout == "0") data.mSeconds = 0;
	else {
		data.mSeconds = mOwner->mServer->Str2Period(sTimeout,*cmd->mOS);
		if(!data.mSeconds)
			 return false;
	}

	if (!mOwner->mServer->mDBConf.allow_exec_mod && (data.mFlags & cTrigger::eTF_EXECUTE)) {
		*cmd->mOS << "Execute command flag is disabled from config";
		return false;
	}
	return true;
}

cTriggers *cTriggerConsole::GetTheList()
{
	return mOwner->mTriggers;
}

const char *cTriggerConsole::CmdSuffix(){ return "trigger";}
const char *cTriggerConsole::CmdPrefix(){ return "!";}

void cTriggerConsole::ListHead(ostream *os)
{
	*os << "Existing triggers are:\r\n";
}

bool cTriggerConsole::IsConnAllowed(cConnDC *conn,int cmd)
{
	return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
}




};
};

