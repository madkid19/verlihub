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

#ifndef NREPLACECCONSOLE_H
#define NREPLACECCONSOLE_H

#include "src/ccmdr.h"
#include "src/cconndc.h"

namespace nVerliHub {
	namespace nReplacePlugin {
		class cpiReplace;
/**
a console that parses commands
@author Daniel Muller
changes by Pralcio
*/
class cConsole
{
public:
	cConsole(cpiReplace *);
	virtual ~cConsole();
	int DoCommand(const string &str, nSocket::cConnDC * conn);
protected:
	cpiReplace *mReplace;
	enum {eMSG_SEND, eMSG_Read };
	class cfBase : public nCmdr::cCommand::sCmdFunc {
		public:
		cpiReplace *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mReplace;}
	};
	class cfAddReplacer : public cfBase { virtual bool operator()();} mcfReplaceAdd;
	class cfGetReplacer : public cfBase { virtual bool operator()();} mcfReplaceGet;
	class cfDelReplacer : public cfBase { virtual bool operator()();} mcfReplaceDel;
	nCmdr::cCommand mCmdReplaceAdd;
	nCmdr::cCommand mCmdReplaceGet;
	nCmdr::cCommand mCmdReplaceDel;
	nCmdr::cCmdr mCmdr;
};
	}; // namespace nReplacePlugin
}; // namespace nReplacePlugin

#endif
