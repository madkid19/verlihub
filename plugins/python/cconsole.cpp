
//
// Verlihub Python Plugin v.1.0
//
// Created in 2007 by Frog, frg@otaku-anime.net
// Code template based on Lua plugin, (c) 2004 by Janos Horvath, bourne@freemail.hu
// Code licensed under the GNU Library General Public License
//

#include "cpipython.h"
#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "cconsole.h"
#include "cpythoninterpreter.h"
#include "src/stringutils.h"

using namespace nDirectConnect;
using namespace nStringUtils;

namespace nScripts
{

cConsole::cConsole(cpiPython *pyt) :
	mPython(pyt),
	mCmdPythonScriptAdd(1,"!pyload ", "(\\S+)", &mcfPythonScriptAdd),
	mCmdPythonScriptGet(0,"!pylist", "", &mcfPythonScriptGet),
	mCmdPythonScriptDel(2,"!pyunload ", "(\\S+)", &mcfPythonScriptDel),
	mCmdPythonScriptRe(3,"!pyreload ", "(\\S+)", &mcfPythonScriptRe),
	mCmdPythonScriptLog(4,"!pylog ", "(\\d+)", &mcfPythonScriptLog),
	mCmdr(this)
{
	mCmdr.Add(&mCmdPythonScriptAdd);
	mCmdr.Add(&mCmdPythonScriptDel);
	mCmdr.Add(&mCmdPythonScriptGet);
	mCmdr.Add(&mCmdPythonScriptRe);
	mCmdr.Add(&mCmdPythonScriptLog);
}

cConsole::~cConsole()
{
}

int cConsole::DoCommand(const string &str, cConnDC * conn)
{
	ostringstream os;
	if(mCmdr.ParseAll(str, os, conn) >= 0)
	{
		mPython->mServer->DCPublicHS(os.str().c_str(),conn);
		return 1;
	}
	return 0;
}

bool cConsole::cfGetPythonScript::operator()()
{
	if (!GetPI()->online) { (*mOS) << "Python plugin is not online! (check console for details)  " ; return true; }
	(*mOS) << "Loaded Python scripts:" << "\r\n";
	for(int i = 0; i < GetPI()->Size(); i++)
	{
		(*mOS) << " [ " << GetPI()->mPython[i]->id << " ] " << GetPI()->mPython[i]->mScriptName << "\r\n";
	}
	return true;
}

bool cConsole::cfDelPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1,scriptfile);
	
	if (!GetPI()->online) { (*mOS) << "Python plugin is not online! (check console for details)  " ; return true; }
	
	bool found = false;
	bool number = false;
	int num = 0;
	if (GetPI()->IsNumber(scriptfile.c_str()))
	{  num = atoi(scriptfile.c_str()); number = true; }
	
	vector<cPythonInterpreter *>::iterator it;
	cPythonInterpreter *li;
	for(it = GetPI()->mPython.begin(); it != GetPI()->mPython.end(); ++it)
	{
		li = *it;
		if ( number && num == li->id || !number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile) == 0 )
		{
			found = true;
			(*mOS) << "Script: [ " << li->id << " ] " << li->mScriptName << " unloaded.  ";
			delete li;
			GetPI()->mPython.erase(it);
			
			break;
		}
	}
	
	if(!found)
		(*mOS) << "Script: " << scriptfile << " not unloaded, because not found.  ";
	
	return true;
}

bool cConsole::cfAddPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1, scriptfile);
	
	if (!GetPI()->online) { (*mOS) << "Python plugin is not online! (check console for details)  " ; return true; }
	
	cPythonInterpreter *ip = new cPythonInterpreter(scriptfile);
	if(!ip) { (*mOS) << "Failed to allocate new Interpreter class instance  "; return true; }
	
	GetPI()->mPython.push_back(ip);
	if(ip->Init())
		(*mOS) << "Script: [ " << ip->id << " ] " << ip->mScriptName << " successfully loaded & initialized.  ";
	else
	{
		(*mOS) << "Script: " << scriptfile << " not found or could not be parsed!  ";
		GetPI()->mPython.pop_back();
		delete ip;
	}
	
	return true;
}

bool cConsole::cfLogPythonScript::operator()()
{
	if (!GetPI()->online) { (*mOS) << "Python plugin is not online! (check console for details)  " ; return true; }
	string level;
	ostringstream msg;
	msg << "PY: Changing logging level from " << cpiPython::log_level;
	GetParStr(1, level);
	cpiPython::me->LogLevel( atoi(level.c_str()) );
	msg << " to " << cpiPython::log_level << "  ";
	(*mOS) << msg.str();
	return true;
}

bool cConsole::cfReloadPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1,scriptfile);
	
	if (!GetPI()->online) { (*mOS) << "Python plugin is not online! (check console for details)  " ; return true; }
	
	bool found = false;
	bool number = false;
	int num = 0;
	if (GetPI()->IsNumber(scriptfile.c_str()))
	{  num = atoi(scriptfile.c_str()); number = true; }
	
	vector<cPythonInterpreter *>::iterator it;
	cPythonInterpreter *li;
	string name;
	for(it = GetPI()->mPython.begin(); it != GetPI()->mPython.end(); ++it)
	{
		li = *it;
		if ( number && num == li->id || !number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile) == 0 )
		{
			found = true;
			name = li->mScriptName;
			(*mOS) << "Script: [ " << li->id << " ] " << li->mScriptName << " unloaded.  ";
			delete li;
			GetPI()->mPython.erase(it);
			break;
		}
	}
	
	if(!found)
	{
		(*mOS) << "Script: " << scriptfile << " not unloaded, because not found or not loaded.  ";
	}
	else
	{
		cPythonInterpreter *ip = new cPythonInterpreter(name);
		if(!ip) { (*mOS) << "Failed to allocate new Interpreter class instance  "; return true; }
	
		GetPI()->mPython.push_back(ip);
		if(ip->Init())
			(*mOS) << "Script: [ " << ip->id << " ] " << ip->mScriptName << " successfully loaded & initialized.  ";
		else
		{
			(*mOS) << "Script: " << scriptfile << " not found or could not be parsed!  ";
			GetPI()->mPython.pop_back();
			delete ip;
		}
		return true;
	}
}

};
