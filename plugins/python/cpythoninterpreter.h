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
using namespace ;

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
