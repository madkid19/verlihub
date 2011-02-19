
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
#include "src/i18n.h"

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
	if (!GetPI()->online) {
		(*mOS) << _("Python interpreter is not loaded.");
		return true;
	}
	(*mOS) << _("Loaded Python scripts:") << "\r\n";
	for(int i = 0; i < GetPI()->Size(); i++) {
		(*mOS) << " [ " << GetPI()->mPython[i]->id << " ] " << GetPI()->mPython[i]->mScriptName << "\r\n";
	}
	return true;
}

bool cConsole::cfDelPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1,scriptfile);
	
	if (!GetPI()->online) {
		(*mOS) << _("Python interpreter is not loaded.");
		return true;
	}
	
	bool found = false;
	bool number = false;
	int num = 0;
	if (GetPI()->IsNumber(scriptfile.c_str())) {
		num = atoi(scriptfile.c_str());
		number = true;
	}
	
	vector<cPythonInterpreter *>::iterator it;
	cPythonInterpreter *li;
	for(it = GetPI()->mPython.begin(); it != GetPI()->mPython.end(); ++it) {
		li = *it;
		if(number && num == li->id || !number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile) == 0 ) {
			found = true;
			(*mOS) << autosprintf(_("Script: [ %d ] %s unloaded."), li->id,  li->mScriptName.c_str()) << "\r\n";
			delete li;
			GetPI()->mPython.erase(it);
			
			break;
		}
	}
	
	if(!found)
		if(number)
			(*mOS) << autosprintf(_("Script n° %s not unloaded because not found."), scriptfile.c_str()) << "\r\n";
		else
			(*mOS) << autosprintf(_("Script %s not unloaded because not found."), scriptfile.c_str()) << "\r\n";
	return true;
}

bool cConsole::cfAddPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1, scriptfile);
	
	if(!GetPI()->online) {
		(*mOS) << _("Python interpreter is not loaded.");
		return true;
	}
	
	cPythonInterpreter *ip = new cPythonInterpreter(scriptfile);
	if(!ip) {
		(*mOS) << _("Failed to allocate new Python interpreter.");
		return true;
	}
	
	GetPI()->mPython.push_back(ip);
	if(ip->Init())
		(*mOS) << autosprintf(_("Script %s successfully loaded and initialized."), ip->mScriptName.c_str()) << "\r\n";
	else {
		(*mOS) << autosprintf(_("Script %s not found or could not be parsed!."), scriptfile.c_str()) << "\r\n";
		GetPI()->mPython.pop_back();
		delete ip;
	}
	
	return true;
}

bool cConsole::cfLogPythonScript::operator()()
{
	if (!GetPI()->online) {
		(*mOS) << _("Python interpreter is not loaded.");
		return true;
	}
	string level;
	GetParStr(1, level);
	ostringstream ss;
   	ss << cpiPython::log_level;
   	string oldValue = ss.str();
	ss.str("");
	ss << level;
   	string newValue = ss.str();
	(*mOS) << autosprintf(_("Updated %s.%s from '%s' to '%s'"), "pi_lua", "log_level", oldValue.c_str(), newValue.c_str()) << "\r\n";
	cpiPython::me->LogLevel( atoi(level.c_str()) );
	return true;
}

bool cConsole::cfReloadPythonScript::operator()()
{
	string scriptfile;
	GetParStr(1,scriptfile);
	
	if (!GetPI()->online) {
		(*mOS) << _("Python interpreter is not loaded.");
		return true;
	}
	
	bool found = false;
	bool number = false;
	int num = 0;
	if (GetPI()->IsNumber(scriptfile.c_str())) { 
		num = atoi(scriptfile.c_str());
		number = true;
	}
	
	vector<cPythonInterpreter *>::iterator it;
	cPythonInterpreter *li;
	string name;
	for(it = GetPI()->mPython.begin(); it != GetPI()->mPython.end(); ++it) {
		li = *it;
		if( number && num == li->id || !number && StrCompare(li->mScriptName,0,li->mScriptName.size(),scriptfile) == 0 ) {
			found = true;
			name = li->mScriptName;
			(*mOS) << autosprintf(_("Script: [ %d ] %s unloaded."), li->id, li->mScriptName.c_str()) << "\r\n";
			delete li;
			GetPI()->mPython.erase(it);
			break;
		}
	}
	
	if(!found) {
		if(number)
			(*mOS) << autosprintf(_("Script n° %s not unloaded because not found or not loaded."), scriptfile.c_str()) << "\r\n";
		else
			(*mOS) << autosprintf(_("Script %s not unloaded  because not found or not loaded."), scriptfile.c_str()) << "\r\n";
		return false;
	} else {
		cPythonInterpreter *ip = new cPythonInterpreter(name);
		if(!ip) {
			(*mOS) << _("Failed to allocate new Python interpreter.");
			return true;
		}
	
		GetPI()->mPython.push_back(ip);
		if(ip->Init())
			(*mOS) << autosprintf(_("Script %s successfully loaded and initialized."), ip->mScriptName.c_str()) << "\r\n";
		else {
			(*mOS) << autosprintf(_("Script %s not found or could not be parsed!."), scriptfile.c_str()) << "\r\n";
			GetPI()->mPython.pop_back();
			delete ip;
		}
		return true;
	}
}

};
