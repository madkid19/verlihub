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

using namespace nDirectConnect;
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
	(*mOS) << "\r\n[::] " << autosprintf(_("Lua version: %s"), LUA_VERSION) << "\r\n";
	(*mOS) << "[::] " << autosprintf(_("Lua library version: %s"), LUA_RELEASE) << "\r\n";
	(*mOS) << "[::] " << autosprintf(_("Copyrigh:t %s"), LUA_COPYRIGHT) << "\r\n";
	(*mOS) << "[::] " << autosprintf(_("Authors: %s"), LUA_AUTHORS) << "\r\n";
	
	return true;
}

bool cConsole::cfInfoLuaScript::operator()()
{
	int size = 0;
	if(GetPI()->Size() > 0) size = lua_getgccount(GetPI()->mLua[0]->mL);
	
	(*mOS) << "\n" << "[::] " << autosprintf(_("Version date: %s"), __CURR_DATE_TIME__);
	(*mOS) << "[::] " << autosprintf(_("Loaded scripts: %d"), GetPI()->Size())  << "\r\n";
	(*mOS) << "[::] " << autosprintf(_("Memory used: %s"), convertByte(size*1024, false).c_str()) << "\r\n";
	return true;
}

bool cConsole::cfGetLuaScript::operator()()
{
	(*mOS) << _("Loaded LUA scripts:") << "\r\n";
	for(int i = 0; i < GetPI()->Size(); i++) {
		(*mOS) << "[ " << i << " ] " << GetPI()->mLua[i]->mScriptName << "\r\n";
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
	
	if (GetPI()->IsNumber(scriptfile.c_str())) {  num = atoi(scriptfile.c_str()); number = true; }
		
	for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it, ++i) {
		li = *it;
		if ((number && num == i) || (!number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0)) {
			found = true;
			scriptfile = li->mScriptName;
			delete li;
			GetPI()->mLua.erase(it);
			(*mOS) << autosprintf(_("Script: [ %d ] %s unloaded."), num, scriptfile.c_str()) << "\r\n";
			break;
		}
	}
	
	if(!found) {
		if(number)
			(*mOS) << autosprintf(_("Script n° %s not unloaded because not found."), scriptfile.c_str()) << "\r\n";
		else
			(*mOS) << autosprintf(_("Script %s not unloaded because not found."), scriptfile.c_str()) << "\r\n";
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
			(*mOS) << autosprintf(_("Failed loading directory %s."), pathname.c_str()) << "\r\n";
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
					(*mOS) << autosprintf(_("Script: %s is already loaded."), scriptfile.c_str()) << "\r\n";
					delete ip;
					return false;
				}
			}
			(*mOS) << autosprintf(_("Script %s successfully loaded and initialized."), scriptfile.c_str()) << "\r\n";
			GetPI()->AddData(ip);
			ip->Load();
		} else {
			(*mOS) << autosprintf(_("Script %s not found or could not be parsed!."), scriptfile.c_str()) << "\r\n";
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
	if (GetPI()->IsNumber(scriptfile.c_str())) {  num = atoi(scriptfile.c_str()); number = true; }

	vector<cLuaInterpreter *>::iterator it;
	cLuaInterpreter *li;
	for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it, ++i) {
		li = *it;
		if ((number && num == i) || (!number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0)) {
			found = true;
			scriptfile = li->mScriptName;
			delete li;
			GetPI()->mLua.erase(it);
			(*mOS) << autosprintf(_("Script: [ %d ] %s unloaded."), num, scriptfile.c_str()) << "\r\n";
			break;
		}
	}
	
	if(!found) {
		if(number)
			(*mOS) << autosprintf(_("Script n° %s not unloaded because not found or not loaded."), scriptfile.c_str()) << "\r\n";
		else
			(*mOS) << autosprintf(_("Script %s not unloaded  because not found or not loaded."), scriptfile.c_str()) << "\r\n";
		return false;
	} else {
		cLuaInterpreter *ip = new cLuaInterpreter(scriptfile);
		if(ip) {
			if(ip->Init()) {
				(*mOS) << autosprintf(_("Script %s successfully loaded and initialized."), scriptfile.c_str()) << "\r\n";
				GetPI()->AddData(ip);
				ip->Load();
			} else {
				(*mOS) << autosprintf(_("Script %s not found or could not be parsed!."), scriptfile.c_str()) << "\r\n";
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
