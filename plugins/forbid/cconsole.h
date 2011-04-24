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
#include "cforbidden.h"

namespace nVerliHub {

	namespace  nSocekt {
		class cConnDC;
	};
	namespace nForbidPlugin {


/**
a console that parses commands

@author Daniel Muller
*/
class cForbidConsole : public nConfig::tListConsole<cForbiddenWorker, cForbidden, cpiForbid>
{
public:
	cForbidConsole(nPlugin::cVHPlugin *pi) : tListConsole<cForbiddenWorker, cForbidden, cpiForbid>(pi)
	{
		AddCommands();
	}

	virtual ~cForbidConsole();
	virtual const char *GetParamsRegex(int cmd);
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual cForbidden *GetTheList();
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cForbiddenWorker &data);
	virtual void GetHelpForCommand(int cmd, ostream &os);
};

	}; // namespace nForbid
}; // namespace nVerliHub

#endif
