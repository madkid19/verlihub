/***************************************************************************
 *   Copyright (C) 2004 by Daniel Muller                                   *
 *   dan at verliba dot cz                                                 *
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
#ifndef NMESSANGERCCONSOLE_H
#define NMESSANGERCCONSOLE_H

#include "src/ccmdr.h"

class cpiMessanger;
using namespace nCmdr;

namespace nDirectConnect { class cConnDC; };
using namespace nDirectConnect;

namespace nMessanger
{


/**
a console that parses commands

@author Daniel Muller
*/
class cConsole
{
public:
	cConsole(cpiMessanger *);

	virtual ~cConsole();
	int DoCommand(const string &str, cConnDC * conn);

	cpiMessanger *mMessanger;
protected:
	enum {eMSG_SEND, eMSG_Read };
  class cfBase : public cCommand::sCmdFunc {
		public:
		cpiMessanger *GetMessanger(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mMessanger;}
	};
	class cfMessageSend : public cfBase { virtual bool operator()();} mcfMsgSend;
	class cfMessageRead : public cfBase { virtual bool operator()();} mcfMsgRead;
	cCommand mCmdMsgSend;
	cCommand mCmdMsgRead;
	cCmdr mCmdr;
};

};

#endif