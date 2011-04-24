/**************************************************************************
*   Original Author: Frog (frg at otaku-anime dot net) 2007-209           *
*                                                                         *
*   Copyright (C) 2010-2011 by Verlihub Project                           *
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

#include "src/ccmdr.h"
#include <string>
#include <sstream>

namespace nVerliHub {
	namespace nSocket {
		class cConnDC;
	};
	namespace nPythonPlugin {
		class cpiPython;
class cConsole
{
public:
	cConsole(cpiPython *);
	virtual ~cConsole();
	int DoCommand(const string &str, nSocket::cConnDC * conn);
	cpiPython *mPython;
protected:
	class cfBase : public nCmdr::cCommand::sCmdFunc {
		public:
		cpiPython *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mPython;}
	};

	class cfAddPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptAdd;
	class cfGetPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptGet;
	class cfDelPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptDel;
	class cfReloadPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptRe;
	class cfLogPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptLog;

	nCmdr::cCommand mCmdPythonScriptAdd;
	nCmdr::cCommand mCmdPythonScriptGet;
	nCmdr::cCommand mCmdPythonScriptDel;
	nCmdr::cCommand mCmdPythonScriptRe;
	nCmdr::cCommand mCmdPythonScriptLog;
	nCmdr::cCmdr mCmdr;
};

	}; // namespace nPythonPlugin
}; // namespace nVerliHub

#endif
