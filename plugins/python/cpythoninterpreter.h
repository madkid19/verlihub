
//
// Verlihub Python Plugin v.1.1
//
// Created in 2007 by Frog, frg@otaku-anime.net
// Code template based on Lua plugin, (c) 2004 by Janos Horvath, bourne@freemail.hu
// Code licensed under the GNU Library General Public License
//

#ifndef CPYTHONINTERPRETER_H
#define CPYTHONINTERPRETER_H

#include "wrapper.h"
#include <string>
#include <iostream>
#include "src/cserverdc.h"
#include "src/cuser.h"
#include "src/script_api.h"
#include <iostream>
#include <string>

using namespace std;
using namespace nDirectConnect;

namespace nScripts
{

class cPythonInterpreter
{
public:
	cPythonInterpreter(string scriptname);
	~cPythonInterpreter();
	bool Init();
	w_Targs *CallFunction(int, w_Targs*);
	string mScriptName;
	int id;
	bool online;
};

};

#endif
