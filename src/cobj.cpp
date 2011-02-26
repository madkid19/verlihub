/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
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
#include "cobj.h"
#include "ctime.h"
#include <iostream>
using namespace std;
using nUtils::cTime;
int cObj::msLogLevel = 4;
int cObj::msCounterObj = 0;
const string cObj::mEmpty;
#ifndef left
#define left ""
#endif
#ifndef right
#define right ""
#endif

/** with name constructor */
cObj::cObj(const char *name) : mClassName(name), mToLog(&cout)
{
	msCounterObj++;//cout << "Constructor cObj(char*), count = " << msCounterObj << endl;
}
cObj::cObj(): mClassName("Obj"), mToLog(&cout)
{
	msCounterObj++;//cout << "Constructor cObj(), count = " << msCounterObj << endl;
}
cObj::~cObj()
{
	msCounterObj--;//cout << "Destructor cObj, count = " << msCounterObj << endl;
}

/** log something into a given stream */
int cObj::StrLog(ostream &ostr, int level)
{
	cTime now;
	if(level <= msLogLevel)
	{
		ostr << " (" << level << ") ";
		if(1)
		{
			ostr.width(26);
			ostr << left << now.AsDate() << " # ";
		}
		ostr.width(15);
		ostr << right << mClassName;
		ostr.width(0);
		ostr << left << " - " ;
		return 1;
	}
	return 0;
}

int cObj::Log(int level)
{
	mToLog = &Log();
	return StrLog(cout, level);
}

/** error Log or not an event */
int cObj::ErrLog(int level)
{
	mToLog = &ErrLog();
	return StrLog(cerr, level);
}

/** return the streal where logging  goes to */
ostream & cObj::Log()
{
	return cout;
}

/** error log stream */
ostream & cObj::ErrLog()
{
	return cerr;
}

/** return selected log stream */
ostream & cObj::LogStream()
{
	return *mToLog;
}
