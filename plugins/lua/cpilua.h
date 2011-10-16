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
#ifndef CPILUA_H
#define CPILUA_H

#include "cluainterpreter.h"
#include "callbacks.h"
#include "cconsole.h"
#include "cquery.h"
#include "src/cconndc.h"
#include "src/cvhplugin.h"
#include <vector>

#define LUA_PI_IDENTIFIER "LuaScript"

using std::vector;
namespace nVerliHub {
	namespace nLuaPlugin {

class cpiLua : public nPlugin::cVHPlugin
{
public:
	cpiLua();
	virtual ~cpiLua();
	virtual void OnLoad(cServerDC *);
	virtual bool RegisterAll();
	virtual bool OnNewConn(nSocket::cConnDC *);
	virtual bool OnCloseConn(nSocket::cConnDC *);
	virtual bool OnParsedMsgChat(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgPM(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgSearch(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgSR(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgMyINFO(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnFirstMyINFO(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgValidateNick(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgAny(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgAnyEx(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgSupport(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgMyPass(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgConnectToMe(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgRevConnectToMe(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnUnknownMsg(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnOperatorCommand(nSocket::cConnDC *, std::string *);
	virtual bool OnOperatorKicks(cUser *, cUser *, std::string *);
	virtual bool OnOperatorDrops(cUser *, cUser *);
	virtual bool OnValidateTag(nSocket::cConnDC *, cDCTag *);
	virtual bool OnUserCommand(nSocket::cConnDC *, std::string *);
	virtual bool OnUserLogin(cUser *);
	virtual bool OnUserLogout(cUser *);
	virtual bool OnTimer();
	virtual bool OnNewReg(std::string, int);
	virtual bool OnDelReg(std::string, int);
	virtual bool OnUpdateClass(string, int, int);
	virtual bool OnNewBan(cBan *);
	virtual bool OnUnBan(string nick, string op, string reason);
	virtual bool OnHubName(std::string, std::string);
	const char * toString(int);

	bool AutoLoad();
	bool CallAll(const char *, char * []);
	int Size() { return mLua.size(); }
	void SetLogLevel(int);
	bool IsNumber(const char*);
	void Empty()
	{
		tvLuaInterpreter::iterator it;

		for(it = mLua.begin(); it != mLua.end(); ++it) {
			if(*it != NULL)
				delete *it;
			*it = NULL;
		}
		mLua.clear();
	}

	void AddData(cLuaInterpreter *ip)
	{
		mLua.push_back(ip);
	}

	cLuaInterpreter * operator[](int i)
	{
		if(i < 0 || i > Size()) return NULL;
			return mLua[i];
	}

	cConsole mConsole;
	nMySQL::cQuery *mQuery;
	typedef vector<cLuaInterpreter *> tvLuaInterpreter;
	tvLuaInterpreter mLua;

	string mScriptDir;

	static int log_level;
	static cpiLua *me;
};
	}; // namespace nLuaPlugin
}; // namespace nVerliHub
#endif
