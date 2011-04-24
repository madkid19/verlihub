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
#ifndef NDIRECTCONNECTCBAN_H
#define NDIRECTCONNECTCBAN_H
#include "cobj.h"

namespace nVerliHub {

	namespace nEnums {
		// Type of the ban
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
	};
	namespace nSocket {
		class cServerDC;
	};

namespace nTables {
/**
 * This class represents a ban that is stored into banlist table.
 * It provides basic methods to display the ban.
 *
 * @author Daniel Muller
 */
class cBan : public cObj
{
    public:

	/**
	* Class constructor.
	* @param server Pointer to the server.
	*/
	cBan(class nSocket::cServerDC *);

	/**
	* Class destructor.
	*/
	~cBan();

	/**
	* Display full information about the ban.
	* @param os The output stream.
	*/
	virtual void DisplayComplete(ostream &os);

	/**
	* Display kick information.
	* @param os The output stream.
	*/
	virtual void DisplayKick(ostream &os);

	/**
	* Display ban information into a single line.
	* @param os The output stream.
	*/
	void DisplayInline(ostream &os);

	/**
	* Display ban information about the user.
	* @param os The output stream.
	*/
	virtual void DisplayUser(ostream &);

	/**
	* Return a string describing the type of the ban.
	* @return The type of the ban.
	*/
	const char *GetBanType();

	/**
	* Set the type of the ban.
	* @param type Ban type.
	*/
	void SetType(unsigned type)
	{
		for(mType = 0; mType < 11; mType++)
			if(type == (unsigned)(1 << mType))
				break;
	}

	/**
	* Write ban information to the output stream.
	*/
	friend ostream & operator << (ostream &, cBan &);

	// Banned IP address
	string mIP;
	// Banned nick
	string mNick;
	// Banned host name
	string mHost;
	// Banned share size
	__int64 mShare;
	// Banned mail
	string mMail;
	// Lowest IP in banned IP range
	unsigned long mRangeMin;
	// Highest IP in banned IP range
	unsigned long mRangeMax;
	// Time of the ban in Unix time format
	long mDateStart;
	// End of the ban in Unix time format
	long mDateEnd;

	// Ban type
	unsigned mType;
	// Operator who banned an user
	string mNickOp;
	// Ban reason
	string mReason;
	// How ban should be displayed to output stream
	int mDisplayType;
	// Pointer to the server
	nSocket::cServerDC *mS;
};

/**
 * This class represents an unban that is stored into unbanlist table.
 * It provides basic methods to display the unban.
 *
 * @author Daniel Muller
 */
class cUnBan : public cBan
{
    public:
	/**
	* Class constructor.
	* @param ban A ban
	* @param server Pointer to the server.
	*/
	cUnBan(cBan &, nSocket::cServerDC *);

	/**
	* Class constructor.
	* @param server Pointer to the server.
	*/
	cUnBan(nSocket::cServerDC *);

	/**
	* Class destructor.
	*/
	~cUnBan();

	/**
	* Display full information about the ban.
	* @param os The output stream.
	*/
	virtual void DisplayComplete(ostream &os);

	/**
	* Display unban information about the user.
	* @param os The output stream.
	*/
	virtual void DisplayUser(ostream &);

	// Time of the unban in Unix time format
	long mDateUnban;
	// Operator who unbanned an user
	string mUnNickOp;
	// Unban reason
	string mUnReason;
};

};
};

#endif
