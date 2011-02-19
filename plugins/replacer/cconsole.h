/***************************************************************************
 *   Copyright (C) 2004 by Pralcio                                         *
 *   based on "Forbid" code made by                                        *
 *   Daniel Muller                                                         *
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
#ifndef NREPLACECCONSOLE_H
#define NREPLACECCONSOLE_H

#include "src/ccmdr.h"

class cpiReplace;
using namespace nCmdr;

namespace nDirectConnect { class cConnDC; };
using namespace nDirectConnect;

namespace nReplace
{


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
	int DoCommand(const string &str, cConnDC * conn);
protected:
	cpiReplace *mReplace;
	enum {eMSG_SEND, eMSG_Read };
	class cfBase : public cCommand::sCmdFunc {
		public:
		cpiReplace *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mReplace;}
	};
	class cfAddReplacer : public cfBase { virtual bool operator()();} mcfReplaceAdd;
	class cfGetReplacer : public cfBase { virtual bool operator()();} mcfReplaceGet;
	class cfDelReplacer : public cfBase { virtual bool operator()();} mcfReplaceDel;
	cCommand mCmdReplaceAdd;
	cCommand mCmdReplaceGet;	
	cCommand mCmdReplaceDel;
	cCmdr mCmdr;
};

};

#endif
