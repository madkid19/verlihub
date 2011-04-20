/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
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

#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tlistconsole.h>

class cpiPlug;
class cPlugs;
class cPlug;
using namespace nCmdr;

namespace  {
	class cConnDC;
	namespace nPlugin{
		class cVHPlugin;
	};
};
using namespace ;
using namespace ::nPlugin;

typedef class tListConsole<cPlug, cPlugs, cpiPlug> tPlugConsoleBase;

/**
a console that parses commands

@author Daniel Muller
*/

class cPlugConsole : public tPlugConsoleBase
{
public:
	// -- required methods
	cPlugConsole(cVHPlugin *pi) : tPlugConsoleBase(pi){AddCommands();}
	virtual ~cPlugConsole();
	virtual cPlugs *GetTheList();
	virtual void ListHead(ostream *os);
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cPlug &data);

	/// ALL for commands
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual const char *GetParamsRegex(int cmd);
	virtual const char *CmdWord(int cmd);
	virtual bool IsConnAllowed(cConnDC* conn,int cmd);
	virtual void GetHelpForCommand(int cmd, ostream &os);
	
	enum {eLC_ON = eLC_FREE, eLC_OFF, eLC_RE};

	class cfOn  : public tPlugConsoleBase::cfBase{ virtual bool operator()();} mcfOn;
	class cfOff : public tPlugConsoleBase::cfBase{ virtual bool operator()();} mcfOff;
	class cfRe  : public tPlugConsoleBase::cfBase{ virtual bool operator()();} mcfRe;

	cDCCommand mCmdOn;
	cDCCommand mCmdOff;
	cDCCommand mCmdRe;

	virtual void AddCommands();
	// end of extra commands
};

#endif
