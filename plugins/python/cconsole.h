
//
// Verlihub Python Plugin v.1.1
//
// Created in 2007 by Frog, frg@otaku-anime.net
// Code template based on Lua plugin, (c) 2004 by Janos Horvath, bourne@freemail.hu
// Code licensed under the GNU Library General Public License
//

#ifndef CCONSOLE_H
#define CCONSOLE_H

#include "src/ccmdr.h"
#include <string>
#include <sstream>

class cpiPython;
using namespace nCmdr;

namespace nDirectConnect { class cConnDC; };
using namespace nDirectConnect;

namespace nScripts
{

class cConsole
{
public:
	cConsole(cpiPython *);
	virtual ~cConsole();
	int DoCommand(const string &str, cConnDC * conn);
	cpiPython *mPython;
protected:
	class cfBase : public cCommand::sCmdFunc {
		public:
		cpiPython *GetPI(){ return ((cConsole *)(mCommand->mCmdr->mOwner))->mPython;}
	};

	class cfAddPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptAdd;
	class cfGetPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptGet;
	class cfDelPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptDel;
	class cfReloadPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptRe;
	class cfLogPythonScript : public cfBase { virtual bool operator()();} mcfPythonScriptLog;
	
	cCommand mCmdPythonScriptAdd;
	cCommand mCmdPythonScriptGet;
	cCommand mCmdPythonScriptDel;
	cCommand mCmdPythonScriptRe;
	cCommand mCmdPythonScriptLog;
	cCmdr mCmdr;
};

};

#endif
