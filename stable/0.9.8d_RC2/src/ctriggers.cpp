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
	mMySQLTable.mExtra = "PRIMARY KEY(command)";
	SetBaseTo(&mModel);
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
		eADD_CLASSXp, eADD_CLASSX
		};
	
	cmd->GetParStr(eADD_CMD,data.mCommand);
	cmd->GetParUnEscapeStr(eADD_DEF,data.mDefinition);
	cmd->GetParStr(eADD_DESC,data.mDescription);
	cmd->GetParStr(eADD_NICK,data.mSendAs);
	cmd->GetParInt(eADD_FLAGS, data.mFlags);
	cmd->GetParInt(eADD_CLASS, data.mMinClass);
	cmd->GetParInt(eADD_CLASSX, data.mMaxClass);
	if (!mOwner->mServer->mDBConf.allow_exec_mod && (data.mFlags & cTrigger::eTF_EXECUTE))
		return false;
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

