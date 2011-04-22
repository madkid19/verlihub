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
#ifndef COBJ_H
#define COBJ_H
#include <iostream>
#include <sstream>
#ifndef _WIN32
#define __int64 long long
#endif

using namespace std;

/** Base class for every object, inque method of logging events (virtual)
    @author Daniel Muller
  */
namespace nVerliHub {

class cObj
{
	public:
		/**  */
		cObj();
		/** with name constructor */
		cObj(const char *name);
		/** destructor */
		virtual ~cObj();

		/** Logs an event into a given stream */
		virtual int StrLog(ostream & ostr, int level);
		/** Log or not an event?? */
		int Log(int level);
		/** error Log or not an event */
		int ErrLog(int level);

		/** No descriptions */
		void SetClassName(const char *name){mClassName=name;}
		/** return selected log stream */
		ostream & LogStream();
		/** error log stream */
		virtual ostream & ErrLog();
		/** return the streal where logging  goes to */
		virtual ostream & Log();

	public:
		/** the Ascii representaition of class name */
		const char * mClassName;
		/** all events higher than this are discarded */
		static int msLogLevel;
		/** a string stream variable to log */
		ostream *mToLog;
		static const string mEmpty;
		static int GetCount(){ return msCounterObj;}

	private:
		/** object counter */
		static int msCounterObj;
};
}; // namespace nVerliHub
#endif
