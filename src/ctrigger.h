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
#ifndef NDIRECTCONNECTCTRIGGER_H
#define NDIRECTCONNECTCTRIGGER_H
#include <sstream>
#include <string>

using namespace std;

namespace  {

class cConnDC;
class cServerDC;

namespace nTables {

/**
Trigger class
User is able to define trigger string that show a message in mainchat or as private message

@author Daniel Muller
@author Simoncelli Davide
*/

class cTrigger
{
public:
	cTrigger();
	virtual ~cTrigger();
	int DoIt(istringstream & cmd_line, cConnDC *conn, cServerDC &server, bool timer=false);
	/**
	 The trigger
	*/
	string mCommand;
	/**
	 The nick that sends the trigger message
	*/
	string mSendAs;
	/**
	 Allowed flags and their meanings for a trigger
	*/
	enum
	{
		eTF_EXECUTE = 1 << 0, // Execute the content of the trigger message in as a shell command
		eTF_SENDPM = 1 << 1, // Send trigger message as private message
		eTF_MOTD = 1 << 2, // Trigger when an user logs in
		eTF_HELP = 1 << 3, // Trigger on +help or !help command
		eTF_DB = 1 << 4, // Trigger message is stored in DB (def column)
		eTF_VARS = 1 << 5, // Replace variables
		eTF_SENDTOALL = 1 << 6 // Send trigger message to all when triggered
	};
	
	/**
	 Timeout for trigger if there is an actived timer for it
	*/
	long mSeconds;
	long mLastTrigger;
	/**
	 The trigger flags that describe when and how to trigger it
	*/
	int mFlags;
	/**
	 The trigger message that can be a file name or a text
	*/
	string mDefinition;
	/**
	 Trigger description
	*/
	string mDescription;
	/**
	 Min class that can trigger
	*/
	int mMinClass;
	/**
	 Max class that can trigger
	*/
	int mMaxClass;
	/**
	 Max lines for definition (0 = unlimited)
	*/
	int mMaxLines;
	/**
	 Max length for definition (0 = unlimited)
	*/
	int mMaxSize;
	/**
	 Min delay before triggering for users
	*/
	int mDelayUser;
	/**
	 Min overall delay in seconds
	*/
	int mDelayTotal;

	virtual void OnLoad();
	friend ostream &operator << (ostream &, cTrigger &);
};
};
};

#endif
