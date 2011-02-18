
//
// Verlihub Python Plugin v.1.1
//
// Created in 2007 by Frog, frg@otaku-anime.net
// Code template based on Lua plugin, (c) 2004 by Janos Horvath, bourne@freemail.hu
// Code licensed under the GNU Library General Public License
//
#include "cpythoninterpreter.h"
#include "cpipython.h"
#include <iostream>

using namespace std;

namespace nScripts
{

cPythonInterpreter::cPythonInterpreter(string scriptname) : mScriptName(scriptname)
{
	//mQuery = new cQuery(cpiPython::me->server->mMySQL);
	id = -1;
	online = false;
	return;
}

cPythonInterpreter::~cPythonInterpreter()
{
	if (!cpiPython::lib_unload || !cpiPython::lib_callhook)
	{ log("PY: cPythonInterpreter   can't use vh_python_wrapper!\n"); return; }
	online = false;
	if (id > -1) cpiPython::lib_unload (id);
	//mQuery->Clear();
	//delete mQuery;
}

bool cPythonInterpreter::Init()
{
	if (!cpiPython::lib_reserveid || !cpiPython::lib_load || !cpiPython::lib_pack) { log("PY: cPythonInterpreter   can't use vh_python_wrapper!\n"); return false; }
	id = cpiPython::lib_reserveid();
	w_Targs *a = cpiPython::lib_pack( "lssssl", id, mScriptName.c_str(), cpiPython::botname.c_str(), cpiPython::opchatname.c_str(), cpiPython::me->server->mConfigBaseDir.c_str(), (long)cpiPython::me->server->mStartTime.Sec() );
	if (!a) { id = -1; return false; }
	id = cpiPython::lib_load(a);
	if (a) free(a);
	if (id > -1) { log1("PY: cPythonInterpreter   loaded script %d:%s\n", id, mScriptName.c_str()); online = true; return true; }
	return false;
}

w_Targs* cPythonInterpreter::CallFunction(int func, w_Targs *args)
{
	if (!cpiPython::lib_hashook || !cpiPython::lib_callhook)
	{ log("PY: cPythonInterpreter   can't use vh_python_wrapper!\n"); return NULL; }
	if (id < 0 || !online) { log2("PY: cPythonInterpreter   script is unavailable\n"); return NULL; }
	if (!cpiPython::lib_hashook(id, func)) return NULL; // true == further processing by other plugins
	w_Targs* res = cpiPython::lib_callhook (id , func, args);
	return res;
}

};
