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
#ifndef CCONSOLE_H
#define CCONSOLE_H

#include "src/ccommandcollection.h"

namespace nVerliHub {
	namespace nSocket {
		class cConnDC;
	};
	namespace nLuaPlugin {
		class cpiLua;
class cConsole
{
public:
	cConsole(cpiLua *);
	virtual ~cConsole();
	int DoCommand(const string &str, nSocket::cConnDC * conn);
	cpiLua *mLua;
protected:
	//enum {eMSG_SEND, eMSG_Read };
	class cfBase : public nCmdr::cCommand::sCmdFunc {
		public:
		cpiLua *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mLua;}
	};

	class cfGetLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptGet;
	class cfAddLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptAdd;
	class cfDelLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptDel;
	class cfReloadLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptRe;
	class cfLogLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptLog;
	class cfInfoLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptInfo;
	class cfVersionLuaScript : public cfBase { virtual bool operator()();} mcfLuaScriptVersion;

	nCmdr::cCommand mCmdLuaScriptGet;
	nCmdr::cCommand mCmdLuaScriptAdd;
	nCmdr::cCommand mCmdLuaScriptDel;
	nCmdr::cCommand mCmdLuaScriptRe;
	nCmdr::cCommand mCmdLuaScriptLog;
	nCmdr::cCommand mCmdLuaScriptInfo;
	nCmdr::cCommand mCmdLuaScriptVersion;
	nCmdr::cCommandCollection mCmdr;
};

	}; // namespace nLuaPlugin
}; // namespace nVerliHub

#endif
