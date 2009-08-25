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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cconndc.h>
#include "cconsole.h"
#include "cpiplug.h"
#include "cplugs.h"

using namespace nDirectConnect;


cPlugs *cPlugConsole::GetTheList()
{
	return mOwner->mList;
}

void cPlugConsole::ListHead(ostream *os)
{
	(*os) << "PlugMan version(" << mOwner->Version() << ")\r\n"
		"Verlihub executable: " << mOwner->mServer->mExecPath << "\r\n"
		"Verlihub make-time: " << cTime(mOwner->mList->mVHTime,0).AsDate() << "\r\n"
		"\r\n";
}

const char *cPlugConsole::CmdSuffix()
{
	return "plug";
}

const char *cPlugConsole::CmdPrefix()
{
	return "!";
}

bool cPlugConsole::IsConnAllowed(cConnDC *conn, int cmd)
{
	if (!conn || !conn->mpUser) return false;
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_DEL:
		case eLC_MOD:
		case eLC_ON:
		case eLC_OFF:
		case eLC_RE: 
			return conn->mpUser->mClass >= eUC_ADMIN;
		break;
		case eLC_LST:
			return conn->mpUser->mClass >= eUC_OPERATOR;
		break;
		default: return false; break;
	}
}

void cPlugConsole::GetHelpForCommand(int cmd, ostream &os)
{
	string help_str;
	switch(cmd)
	{
		case eLC_LST: 
		help_str = "!lstplug\r\nGive a list of registered plugins"; 
		break;
		case eLC_ADD: 
		case eLC_MOD:
		help_str = "!(add|mod)plug <nick>"
			"[ -p <\"path\">]"
			"[ -d <\"desc\">]"
			"[ -a <autoload>]"
			"\r\n""      register or update a plugin\r\n"
		"     * name - short plugin name\r\n"
		"     * path - a relative or absolute filename of the plugin's binary (it's better to provide absoulute path)\r\n"
		"     * desc - a breif description of what the plugin does\r\n"
		"     * autoload - 1/0 to autoload plugin at startup";
		break;
		case eLC_DEL:
		help_str = "!delplug <ipmin_or_iprange>"; break;
		default: break;
	}
	cDCProto::EscapeChars(help_str,help_str);
	os << help_str;	
}

const char * cPlugConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_MOD:
			return "^(\\S+)(" // <nick>
			      "( -p ?(\")?((?(4)[^\"]+?|\\S+))(?(4)\"))|" // <"path">
			      "( -d ?(\")?((?(7)[^\"]+?|\\S+))(?(7)\"))|" // [ <desc>]
			      "( -a ?([01]))|"
			      ")*\\s*$" // the end of message
			      ; break;
		case eLC_DEL: return "(\\S+)"; break;
		case eLC_ON: return "(\\S+)"; break;
		case eLC_OFF: return "(\\S+)"; break;
		case eLC_RE: return "(\\S+)"; break;
		default : return ""; break;
	};
}

const char *cPlugConsole::CmdWord(int cmd)
{
	switch(cmd)
	{
		case eLC_ON : return "on";
		case eLC_OFF: return "off";
		case eLC_RE : return "re";
		default: return tPlugConsoleBase::CmdWord(cmd);
	}
}

bool cPlugConsole::ReadDataFromCmd(cfBase *cmd, int id, cPlug &data)
{
	/// regex parts for add command
	enum {aADD_ALL, eADD_NICK, eADD_CHOICE, 
		eADD_PATHp, eADD_QUOTE , eADD_PATH, 
		eADD_DESCP, eADD_QUOTE2, eADD_DESC,
		eADD_AUTOp, eADD_AUTO};

	cmd->GetParStr(eADD_NICK, data.mNick);
	if ((data.mNick.size() > 10) && (id == eLC_ADD))
	{
		*cmd->mOS << "Plugin name must be max 10 characters long; please provide another one";
		return false;
	}
	cmd->GetParUnEscapeStr(eADD_PATH, data.mPath);
	if(data.mPath.size() < 1 && (id == eLC_ADD)) {
		*cmd->mOS << "Please provide a valid path for the plugin";
		return false;
	}
	cmd->GetParStr(eADD_DESC, data.mDesc);
	cmd->GetParBool(eADD_AUTO, data.mLoadOnStartup);
	return true;
}

void cPlugConsole::AddCommands()
{
	tPlugConsoleBase::AddCommands();
	mCmdOn.Init(eLC_ON, CmdId(eLC_ON), GetParamsRegex(eLC_ON), &mcfOn);
	mCmdOff.Init(eLC_OFF, CmdId(eLC_OFF), GetParamsRegex(eLC_OFF), &mcfOff);
	mCmdRe.Init(eLC_RE, CmdId(eLC_RE), GetParamsRegex(eLC_RE), &mcfRe);
	mCmdr.Add(&mCmdOn);
	mCmdr.Add(&mCmdOff);
	mCmdr.Add(&mCmdRe);

}

bool cPlugConsole::cfOn::operator()()
{
	cPlug Data;
	if ( GetConsole() && GetConsole()->ReadDataFromCmd(this, eLC_ON, Data))
	{
		cPlug *Plug = GetTheList()->FindData(Data);
		if (Plug) {
			bool res = Plug->Plugin();
			// Show an error if it fails
			if(!res) *mOS << Plug->mLastError;
			return res;
		}
		*mOS << "Plugin '" << Data.mNick << "' not found. ";
	}
	return false;
}

bool cPlugConsole::cfOff::operator()()
{
	cPlug Data;
	if ( GetConsole() && GetConsole()->ReadDataFromCmd(this, eLC_ON, Data))
	{
		cPlug *Plug = GetTheList()->FindData(Data);
		if (Plug) return Plug->Plugout();
		*mOS << "Plugin '" << Data.mNick << "' not found. ";
	}
	return false;
}

bool cPlugConsole::cfRe::operator()()
{
	cPlug Data;
	if ( GetConsole() && GetConsole()->ReadDataFromCmd(this, eLC_ON, Data))
	{
		cPlug *Plug = GetTheList()->FindData(Data);
		if (Plug) return Plug->Replug();
		*mOS << "Plugin '" << Data.mNick << "' not found. ";
	}
	return false;
}

cPlugConsole::~cPlugConsole(){}

