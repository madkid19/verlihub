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

#include "src/ccmdr.h"
#include "src/cdccommand.h"

namespace nVerliHub {
	namespace nSocket {
		class cConnDC;
	};
	namespace nEnums {
		enum {
			eIL_LASTIP,
			eIL_HISTORY
		};
	};
	namespace nIPLogPlugin {
		class cIPLog;
		class cpiIPLog;

/**
a console that parses commands

@author Przemek Bryniak
*/
class cConsole
{
public:
	cConsole(cpiIPLog *);
	virtual ~cConsole();
	int DoCommand(const string &str, nSocket::cConnDC * conn);
	cpiIPLog *mIPLog;
protected:

	class cfBase : public cDCCommand::sDCCmdFunc {
		public:
		cpiIPLog *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mIPLog;}
	};
	class cfLastIp : public cfBase { virtual bool operator()();} mcfLastIp;
	class cfHistoryOf : public cfBase { virtual bool operator()();} mcfHistoryOf;
	nCmdr::cCommand mCmdLastIp;
	nCmdr::cCommand mCmdHistoryOf;
	nCmdr::cCmdr mCmdr;
};
	}; // namespace nIPLogPlugin
}; // namespace nVerliHub

#endif
