/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECTCTRIGGER_H
#define NDIRECTCONNECTCTRIGGER_H

#include <sstream>
#include <string>

using namespace std;

namespace nDirectConnect {

class cConnDC;
class cServerDC;

namespace nTables {

/**
a trigger command ...
user defined string that triggers given action

@author Daniel Muller
*/

class cTrigger
{
public:
	cTrigger();
	virtual ~cTrigger();
	int DoIt(istringstream & cmd_line, cConnDC *conn, cServerDC &server);
	// the trigger
	string mCommand;
	// the nick for the response
	string mSendAs;
	enum
	{
		eTF_EXECUTE = 1 << 0, //< execute the definition as shell command instead of sending it
		eTF_SENDPM = 1 << 1, //< send as private message
		eTF_MOTD = 1 << 2, //< trigger when user logs in
		eTF_HELP = 1 << 3, //< trigger on +help command or !help
		eTF_DB = 1 << 4, //< means the text to send is stored in def column
		eTF_VARS = 1 << 5, //< allow replacing in text vars
		eTF_SENDTOALL = 1 << 6 //< send this to everybody when triggered // @CHANGED by dReiska
	};
	// settings flags
	int mFlags;
	// the definition string (filename, program, etc..)
	string mDefinition;
	// description
	string mDescription;
	// minimum class that can trigger this
	int mMinClass;
	int mMaxClass;
	// max lines (0 = ilimited)
	int mMaxLines;
	// maximum length (0 ilimited)
	int mMaxSize;
	// min delay betwen triggering this for user [s]
	int mDelayUser;
	// min overall delay in seconds too
	int mDelayTotal;

	virtual void OnLoad();
	friend ostream &operator << (ostream &, cTrigger &);
};
};
};

#endif
