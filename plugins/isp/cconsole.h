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
#ifndef NFORBIDCCONSOLE_H
#define NFORBIDCCONSOLE_H

#include "src/tlistconsole.h"
#include "cisps.h"

class cpiISP;
using namespace nCmdr;

namespace  { class cConnDC; };
using namespace ;

typedef class tListConsole<cISP, cISPs, cpiISP> tISPConsoleBase;
/**
a console that parses commands

@author Daniel Muller
*/
class cISPConsole : public tISPConsoleBase
{
public:
	cISPConsole(cVHPlugin *pi) : tISPConsoleBase(pi)
	{
		AddCommands();
	}

	virtual ~cISPConsole();
	virtual const char *GetParamsRegex(int cmd);
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual cISPs *GetTheList();
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cISP &data);
	virtual void GetHelpForCommand(int cmd, ostream &os);
};

#endif
