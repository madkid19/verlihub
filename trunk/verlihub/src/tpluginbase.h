/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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

#ifndef NPLUGINTPLUGINBASE_H
#define NPLUGINTPLUGINBASE_H

#include <string>
#include "cobj.h"
#ifdef _WIN32
#include <windows.h>
#endif
using std::string;

namespace nPlugin {

/**
the plugin base class suitable for any application

@author Daniel Muller
*/
class tPluginBase : public cObj
{
public:
	tPluginBase();
	~tPluginBase();
	bool Open();
	bool Close();
	string Error();
	int StrLog(ostream & ostr, int level);
protected:
	string mFileName;
	string mName;
	#ifdef _WIN32
	HINSTANCE mHandle;
	#else
	void *mHandle;
	#endif

};

};

#endif
