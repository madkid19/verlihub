/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz)                *
*                    Janos Horvath (bourne at freemail dot hu) 2004-05    *
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
extern "C"
{
#include <lua.h>
}

#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "cconsole.h"
#include "cpilua.h"
#include "cluainterpreter.h"
#include "curr_date_time.h"
#include "src/stringutils.h"
#include "src/i18n.h"
#include "src/stringutils.h"
#include <dirent.h>

#define PADDING 25

using namespace ;
using namespace nStringUtils;

namespace nScripts
{

cConsole::cConsole(cpiLua *lua) :
	mLua(lua),
	mCmdLuaScriptGet(0,"!lualist", "", &mcfLuaScriptGet),
	mCmdLuaScriptAdd(1,"!luaload ", "(.*)", &mcfLuaScriptAdd),
	mCmdLuaScriptDel(2,"!luaunload ", "(.*)", &mcfLuaScriptDel),
	mCmdLuaScriptRe(3,"!luareload ", "(.*)", &mcfLuaScriptRe),
	mCmdLuaScriptLog(4,"!lualog", "(.*)", &mcfLuaScriptLog), //(\\d+)
	mCmdLuaScriptInfo(6,"!luainfo", "", &mcfLuaScriptInfo),
	mCmdLuaScriptVersion(5,"!luaversion", "", &mcfLuaScriptVersion),
	mCmdr(this)
{
	mCmdr.Add(&mCmdLuaScriptAdd);
	mCmdr.Add(&mCmdLuaScriptDel);
	mCmdr.Add(&mCmdLuaScriptGet);
	mCmdr.Add(&mCmdLuaScriptRe);
	mCmdr.Add(&mCmdLuaScriptInfo);
	mCmdr.Add(&mCmdLuaScriptLog);
	mCmdr.Add(&mCmdLuaScriptVersion);
}

cConsole::~cConsole()
{
}

int cConsole::DoCommand(const string &str, cConnDC * conn)
{
	ostringstream os;
	
	if(mCmdr.ParseAll(str, os, conn) >= 0)
	{
		mLua->mServer->DCPublicHS(os.str().c_str(),conn);
		return 1;
	}
	return 0;
}

bool cConsole::cfVersionLuaScript::operator()()
{
	(*mOS) << "\r\n[*] " << setw(PADDING) << setiosflags(ios::left) << _("Lua version") << LUA_VERSION << endl;
	(*mOS) << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Lua library version") << LUA_RELEASE << endl;
	(*mOS) << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Copyright") << LUA_COPYRIGHT << endl;
	(*mOS) << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Authors") << LUA_AUTHORS << endl;
	
	return true;
}

bool cConsole::cfInfoLuaScript::operator()()
{
	int size = 0;
	if(GetPI()->Size() > 0) size = lua_getgccount(GetPI()->mLua[0]->mL);
	
	(*mOS) << "\n" << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Version date") << __CURR_DATE_TIME__ << endl;
	(*mOS) << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Running scripts") << GetPI()->Size() << endl;
	(*mOS) << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Memory used") << convertByte(size*1024, false).c_str() << endl;
	return true;
}

bool cConsole::cfGetLuaScript::operator()()
{
	(*mOS) << _("Running LUA scripts:") << "\r\n";
	(*mOS) << "\n ";
	(*mOS) << setw(6) << setiosflags(ios::left) << "ID";
	(*mOS) << toUpper(_("Script")) << "\n";
	(*mOS) << " " << string(6+20,'=') << endl;
	for(int i = 0; i < GetPI()->Size(); i++) {
		(*mOS) << " " << setw(6) << setiosflags(ios::left) << i << GetPI()->mLua[i]->mScriptName << "\r\n";
	}
	return true;
}

bool cConsole::cfDelLuaScript::operator()()
{
	string scriptfile;
	GetParStr(1,scriptfile);
	
	bool found = false;
	bool number = false;
	int i = 0, num = 0;
	vector<cLuaInterpreter *>::iterator it;
	cLuaInterpreter *li;
	
	if (GetPI()->IsNumber(scriptfile.c_str())) {
		num = atoi(scriptfile.c_str());
		number = true;
	}
		
	for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it, ++i) {
		li = *it;
		if ((number && num == i) || (!number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0)) {
			found = true;
			scriptfile = li->mScriptName;
			(*mOS) << autosprintf(_("Script %s stopped."), li->mScriptName.c_str()) << " ";
			delete li;
			GetPI()->mLua.erase(it);
			break;
		}
	}
	
	if(!found) {
		if(number)
			(*mOS) << autosprintf(_("Script #%s not stopped because it is not running."), scriptfile.c_str()) << " ";
		else
			(*mOS) << autosprintf(_("Script %s not stopped because it is not running."), scriptfile.c_str()) << " ";
		return false;
	}
	return true;
}

bool cConsole::cfAddLuaScript::operator()()
{
	string scriptfile, pathname, filename;
	bool number = false;
	int num = 0;
	GetParStr(1, scriptfile);
	vector<cLuaInterpreter *>::iterator it;
	cLuaInterpreter *li;	

	if (GetPI()->IsNumber(scriptfile.c_str())) {
		num = atoi(scriptfile.c_str());
		number = true;
	}
	cServerDC *server= cServerDC::sCurrentServer;
	pathname = server->mConfigBaseDir;
	
	
	if(number) {
		DIR *dir = opendir(pathname.c_str());
		int i = 0;
		if(!dir) {
			(*mOS) << autosprintf(_("Failed loading directory %s."), pathname.c_str()) << " ";
			return false;
		}
		struct dirent *dent = NULL;
		
		while(NULL != (dent=readdir(dir))) {
			
			filename = dent->d_name;
			if((filename.size() > 4) && (StrCompare(filename,filename.size()-4,4,".lua")==0)) {
				if(i == num)
					scriptfile = pathname + "/" + filename;
				i++;
			}
			
		}
	}
	cLuaInterpreter *ip = new cLuaInterpreter(scriptfile);
	if(ip) {
		if(ip->Init()) {
			for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it) {
				li = *it;
				if (StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0) {
					(*mOS) << autosprintf(_("Script %s is already running."), scriptfile.c_str()) << " ";
					delete ip;
					return false;
				}
			}
			(*mOS) << autosprintf(_("Script %s is now running."), scriptfile.c_str()) << " ";
			GetPI()->AddData(ip);
			ip->Load();
		} else {
			(*mOS) << autosprintf(_("Script %s not found or could not be parsed."), scriptfile.c_str()) << " ";
			delete ip;
			return false;
		}
	}
	return true;
}

bool cConsole::cfReloadLuaScript::operator()()
{
	string scriptfile;
	bool found = false, number = false;
	int i, num;
	i = num = 0;
	
	GetParStr(1,scriptfile);
	if (GetPI()->IsNumber(scriptfile.c_str())) {
		num = atoi(scriptfile.c_str());
		number = true;
	}

	vector<cLuaInterpreter *>::iterator it;
	cLuaInterpreter *li;
	for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it, ++i) {
		li = *it;
		if ((number && num == i) || (!number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0)) {
			found = true;
			(*mOS) << autosprintf(_("Script %s stopped."), li->mScriptName.c_str()) << " ";
			scriptfile = li->mScriptName;
			delete li;
			GetPI()->mLua.erase(it);
			break;
		}
	}
	
	if(!found) {
		if(number)
			(*mOS) << autosprintf(_("Script #%s not stopped because it is not running."), scriptfile.c_str()) << " ";
		else
			(*mOS) << autosprintf(_("Script %s not stopped  because it is not running."), scriptfile.c_str()) << " ";
		return false;
	} else {
		cLuaInterpreter *ip = new cLuaInterpreter(scriptfile);
		if(ip) {
			if(ip->Init()) {
				(*mOS) << autosprintf(_("Script %s is now running."), scriptfile.c_str()) << " ";
				GetPI()->AddData(ip);
				ip->Load();
			} else {
				(*mOS) << autosprintf(_("Script %s not found or could not be parsed."), scriptfile.c_str()) << " ";
				delete ip;
			}
		}
		return true;
	}
}

bool cConsole::cfLogLuaScript::operator()()
{
	int level;
	ostringstream msg;
	if(GetParInt(1, level)) {
		stringstream ss;
   		ss << cpiLua::log_level;
   		string oldValue = ss.str();
		ss.str("");
		ss << level;
   		string newValue = ss.str();
		(*mOS) << autosprintf(_("Updated %s.%s from '%s' to '%s'"), "pi_lua", "log_level", oldValue.c_str(), newValue.c_str()) << "\r\n";
		cpiLua::me->SetLogLevel(level);
	} else {
		msg << "Current setting is " << cpiLua::log_level;
		(*mOS) << msg.str();
	}
	return true;
}

};
