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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <verlihub/cconndc.h>
#include <verlihub/cserverdc.h>
#include "cconsole.h"
#include "cpilua.h"
#include "cluainterpreter.h"
#include "curr_date_time.h"
#include <verlihub/stringutils.h>
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
	mCmdLuaScriptVersion(5,"!luaversion", "", &mcfLuaScriptVersion),
	mCmdLuaScriptInfo(6,"!luainfo", "", &mcfLuaScriptInfo),
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
	(*mOS) << "Lua Plugin v." << "\r";
	(*mOS) << VERSION << "\r\n";
	(*mOS) << "Lua version:" << "\r";
	(*mOS) << LUA_RELEASE << "\r\n";
	(*mOS) << "Copyright:" << "\r";
	(*mOS) << LUA_COPYRIGHT << "\r\n";
	(*mOS) << "Authors:" << "\r";
	(*mOS) << LUA_AUTHORS << "\r\n";
	return true;
}

bool cConsole::cfInfoLuaScript::operator()()
{
	int size = 0;
	if(GetPI()->Size() > 0) size = lua_getgccount(GetPI()->mLua[0]->mL);
	
	(*mOS) << "\n" << "[::] Version date: "<<  __CURR_DATE_TIME__ << "\r\n";
	(*mOS) << "[::] Loaded scripts: " << GetPI()->Size()  << "\r\n";
	(*mOS) << "[::] Memory used: " << size << " KB"  << "\r\n";
	return true;
}

bool cConsole::cfGetLuaScript::operator()()
{
	(*mOS) << "Loaded LUA scripts:" << "\r\n";
	for(int i = 0; i < GetPI()->Size(); i++)
	{
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
			(*mOS) << "Script: [ " << num << " ] " <<scriptfile << " unloaded." << "\r\n";
			break;
		}
	}
	
	if(!found) {
		if(number) (*mOS) << "Script n° " << scriptfile << " not unloaded, because not found." << "\r\n";
		else (*mOS) << "Script " << scriptfile << " not unloaded, because not found." << "\r\n";
		return false;
	}
	return true;
}

bool cConsole::cfAddLuaScript::operator()()
{
	string scriptfile, pathname, filename;
	bool number = false;
	int num;
	GetParStr(1, scriptfile);
	vector<cLuaInterpreter *>::iterator it;
	cLuaInterpreter *li;	

	if (GetPI()->IsNumber(scriptfile.c_str())) {  num = atoi(scriptfile.c_str()); number = true; }
	cServerDC *server= cServerDC::sCurrentServer;
	pathname = server->mConfigBaseDir;
	
	
	if(number) {
		DIR *dir = opendir(pathname.c_str());
		int i = 0;
		if(!dir) {
			(*mOS) << "Failed loading " << pathname << " ";
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
	if(ip) 
		if(ip->Init()) {
			for(it = GetPI()->mLua.begin(); it != GetPI()->mLua.end(); ++it) {
				li = *it;
				if (StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile)==0) {
					(*mOS) << "Script " << scriptfile << " is already loaded!" << "\r\n";
					delete ip;
					return false;
				}
			}
			(*mOS) << "Script: " << scriptfile << " successfully loaded & initialized." << "\r\n";
			GetPI()->AddData(ip);
			ip->Load();
	} else {
		(*mOS) << "Script: " << scriptfile << " not found or could not be parsed!" << "\r\n";
		delete ip;
		return false;
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
			(*mOS) << "Script: [ " << num << " ] " <<scriptfile << " unloaded." << "\r\n";
			break;
		}
	}
	
	if(!found) {
		if(number) (*mOS) << "Script n° " << scriptfile << " not unloaded, because not found or not loaded." << "\r\n";
		else (*mOS) << "Script " << scriptfile << " not unloaded, because not found or not loaded." << "\r\n";
		return false;
	} else {
		cLuaInterpreter *ip = new cLuaInterpreter(scriptfile);
		if(ip)
			if(ip->Init()) {
				(*mOS) << "Script: " << scriptfile << " successfully loaded & initialized." << "\r\n";
				GetPI()->AddData(ip);
				ip->Load();
			} else {
				(*mOS) << "Script: " << scriptfile << " not found or could not be parsed!" << "\r\n";
				delete ip;
			}
		return true;
	}
}

bool cConsole::cfLogLuaScript::operator()()
{
	int level;
	ostringstream msg;
	if(GetParInt(1, level)) {
		msg << "Switching Lua Log level from " << cpiLua::log_level;
		//cout << level << endl;
		cpiLua::me->SetLogLevel(level);
		msg << " to " << cpiLua::log_level << " ";
		(*mOS) << msg.str();
		
	} else {
		msg << "Current setting is " << cpiLua::log_level;
		(*mOS) << msg.str();
	}
	return true;
}

};
