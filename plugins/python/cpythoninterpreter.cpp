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
#include "cpythoninterpreter.h"
#include "cpipython.h"
//#include <iostream>
#include "src/cserverdc.h"
#include "src/cuser.h"

using namespace std;
namespace nVerliHub {
	namespace nPythonPlugin {

cPythonInterpreter::cPythonInterpreter(string scriptname) : mScriptName(scriptname)
{
	id = -1;
	online = false;
	return;
}

cPythonInterpreter::~cPythonInterpreter()
{
	if(!cpiPython::lib_unload || !cpiPython::lib_callhook) {
		log("PY: cPythonInterpreter can't use vh_python_wrapper!\n");
		return;
	}
	online = false;
	if(id > -1)
		cpiPython::lib_unload (id);
}

bool cPythonInterpreter::Init()
{
	if (!cpiPython::lib_reserveid || !cpiPython::lib_load || !cpiPython::lib_pack) {
		log("PY: cPythonInterpreter can't use vh_python_wrapper!\n");
		return false;
	}
	id = cpiPython::lib_reserveid();
	w_Targs *a = cpiPython::lib_pack( "lssssl", id, mScriptName.c_str(), cpiPython::botname.c_str(), cpiPython::opchatname.c_str(), cpiPython::me->server->mConfigBaseDir.c_str(), (long)cpiPython::me->server->mStartTime.Sec() );
	if(!a) {
		id = -1;
		return false;
	}
	id = cpiPython::lib_load(a);
	if(a)
		free(a);
	if(id > -1) {
		log1("PY: cPythonInterpreter loaded script %d:%s\n", id, mScriptName.c_str());
		online = true;
		return true;
	}
	return false;
}

w_Targs* cPythonInterpreter::CallFunction(int func, w_Targs *args)
{
	if(!cpiPython::lib_hashook || !cpiPython::lib_callhook) {
		log("PY: cPythonInterpreter can't use vh_python_wrapper!\n");
		return NULL;
	}
	if(id < 0 || !online) {
		log2("PY: cPythonInterpreter script is unavailable\n");
		return NULL;
	}
	if(!cpiPython::lib_hashook(id, func))
		return NULL; // true == further processing by other plugins
	w_Targs* res = cpiPython::lib_callhook (id , func, args);
	return res;
}
	}; // namespace nPythonPlugin
}; // namespace nVerliHub
