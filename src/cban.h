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
#ifndef NDIRECTCONNECTCBAN_H
#define NDIRECTCONNECTCBAN_H
#include "cobj.h"

namespace nDirectConnect {

class cServerDC;
/**
  *  Classes and structure representing and treating MYSQL tables used by verlihub
  *
  */
namespace nTables {

/**
a ban record structure

@author Daniel Muller
*/
class cBan : public cObj
{
public:
	cBan(class cServerDC *);
	~cBan();
	virtual void DisplayUser(ostream &);
	virtual void DisplayComplete(ostream &os);
	virtual void DisplayKick(ostream &os);

	// banned ip address
	string mIP;
	// banned nick
	string mNick;
	// banned host name
	string mHost;
	// banned share size
	__int64 mShare;
	// banned mail
	string mMail;
	// banned ip range
	unsigned long mRangeMin;
	unsigned long mRangeMax;
	// unix timestamp of the start, end
	long mDateStart;
	long mDateEnd;

	enum {
		eBF_NICKIP = 1 << 0,
		eBF_IP  = 1 << 1,
		eBF_NICK  = 1 << 2,
		eBF_RANGE = 1 << 3,
		eBF_HOST1 = 1 << 4,
		eBF_HOST2 = 1 << 5,
		eBF_HOST3 = 1 << 6,
		eBF_SHARE = 1 << 7,
		eBF_EMAIL = 1 << 8,
		eBF_PREFIX = 1 << 9,
		eBF_HOSTR1 = 1 << 10,
	};
	unsigned mType;

	// op banner
	string mNickOp;
	// the ban reason
	string mReason;

	friend ostream & operator << (ostream &, cBan &);
	void DisplayInline(ostream &os);

	/*!
		\fn nDirectConnect::nTables::cBan::SetType(unsigned )
	*/
	void SetType(unsigned type)
	{
		for(mType = 0; mType < 11; mType++) if ( type == (unsigned)(1 << mType)) break;
	}

	int mDisplayType;

	cServerDC *mS;
};

/**
  Unban is a formaer ban, extended by some details.
  This is a class correspondig to the MySQL table
*/
class cUnBan : public cBan
{
public:
	cUnBan(cBan &, cServerDC *);
	cUnBan(cServerDC *);
	~cUnBan();

	virtual void DisplayUser(ostream &);
	virtual void DisplayComplete(ostream &os);

	long mDateUnban;

	// op unbanner
	string mUnNickOp;
	// the unban reason
	string mUnReason;

};

};
};

#endif
