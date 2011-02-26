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
#ifndef NCHATROOMCCONSOLE_H
#define NCHATROOMCCONSOLE_H

#include "src/tlistconsole.h"
#include "crooms.h"

class cpiChatroom;
using namespace nCmdr;

namespace nDirectConnect { class cConnDC; };
using namespace nDirectConnect;

typedef class tListConsole<cRoom, cRooms, cpiChatroom> tRoomConsoleBase;

/**
a console that parses commands
@author Lukas Ronge
@author Daniel Muller
*/

class cRoomConsole : public tRoomConsoleBase
{
public:
	// -- required methods
	cRoomConsole(cVHPlugin *pi) : tRoomConsoleBase(pi){AddCommands();}
	virtual ~cRoomConsole();
	virtual const char *GetParamsRegex(int cmd);
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual cRooms *GetTheList();
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cRoom &data);
	virtual void GetHelpForCommand(int cmd, ostream &os);
	virtual bool IsConnAllowed(cConnDC *conn,int cmd);
};

#endif
