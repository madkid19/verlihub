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

#ifndef CMGSLIST_H
#define CMGSLIST_H

#include "src/cconfmysql.h"
#include "src/tcache.h"

using namespace nConfig;
namespace nDirectConnect { class cUser; class cServerDC; };
using namespace nDirectConnect;

namespace nMessanger
{

struct sMessage
{
	string mSender;
	string mSenderIP;
	string mReceiver;
	time_t mDateSent;
	time_t mDateExpires;
	string mSubject;
	string mBody;

	sMessage(){mDateSent =mDateExpires = 0;};
	// output stuff
	mutable enum { AS_SUBJECT, AS_BODY, AS_DELIVERY, AS_ONLINE } mPrintType;
	sMessage &AsSubj() { mPrintType = AS_SUBJECT; return *this; } 
	sMessage &AsOnline() { mPrintType = AS_ONLINE ; return *this; }
	sMessage &AsBody() { mPrintType = AS_BODY; return *this; }
	sMessage &AsDelivery() { mPrintType = AS_DELIVERY; return *this; }
	friend ostream & operator << (ostream &os, sMessage &Msg);
};

/**
@author Daniel Muller
*/
class cMsgList : public cConfMySQL
{
public:
	cMsgList(cServerDC *server);
	virtual ~cMsgList();
	virtual void CleanUp();
	virtual void AddFields();
	
	int CountMessages(const string &nick, bool IsSender);
	bool AddMessage( sMessage &msg );
	int PrintSubjects(ostream &os, const string &nick, bool IsSender);
	int DeliverMessagesForUser(cUser *dest);
	int DeliverMessagesSinceSync(unsigned sync);
	int DeliverModelToUser(cUser *dest);

	void DeliverOnline(cUser *dest, sMessage &msg);
	int SendAllTo(cUser *, bool IsSender);
	void UpdateCache();

	tCache<string> mCache;
	sMessage mModel;
	cServerDC *mServer;
};

};
#endif
