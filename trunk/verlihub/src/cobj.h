/***************************************************************************
                          cobj.h  -  description
                             -------------------
    begin                : Sat May 10 2003
    copyright            : (C) 2003 by Daniel Muller
    email                : dan at verliba dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
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
	virtual int StrLog(std::ostream & ostr, int level);
	/** Log or not an event?? */
	int Log(int level);
	/** error Log or not an event */
	int ErrLog(int level);

	/** No descriptions */
	void SetClassName(const char *name){mClassName=name;}
	/** return selected log stream */
	std::ostream & LogStream();
	/** error log stream */
	virtual std::ostream & ErrLog();
	/** return the streal where logging  goes to */
	virtual std::ostream & Log();

public: // Public attributes
	/** the Ascii representaition of class name */
	const char * mClassName;
	/** all events higher than this are discarded */
	static int msLogLevel;
	/** a string stream variable to log */
	std::ostream *mToLog;
	static const string mEmpty;
	static int GetCount(){ return msCounterObj;}
	
private: //private attributes
	/** object counter */
	static int msCounterObj;
};

#endif
