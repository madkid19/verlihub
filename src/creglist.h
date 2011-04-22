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
#ifndef CREGLIST_H
#define CREGLIST_H
#include <string>
#include "cconfmysql.h"
#include "creguserinfo.h"
#include "tcache.h"

using namespace std;

namespace nVerliHub {
	using namespace nMySQL;
	namespace nSocket {
		class cConnDC;
		class cServerDC;
	};

	namespace nTables {

	class cRegUserInfo;

/**a reg users list
  *@author Daniel Muller
  */

class cRegList : public nConfig::cConfMySQL
{
public:
	cRegList(nMySQL::cMySQL &mysql, nSocket::cServerDC *);
	virtual ~cRegList();
	/** find nick in reglist
		if not foud return 0
		else return 1 and fill in the reuserinfo parameter */
	bool FindRegInfo(cRegUserInfo &, const string &nick);
	bool ShowUsers(cConnDC *op, ostream &os, int page, int offset, string nick);
	/** add registered user */
	bool AddRegUser(const string &nick, nSocket::cConnDC *op, int cl, const char *password = NULL);
	/** No descriptions */
	bool ChangePwd(const string &nick, const string &pwd,int);
	/** No descriptions */
	bool SetVar(const string &nick, string &field, string &value);
	/** log that user logged in */
	bool Login(nSocket::cConnDC *conn, const string &nick);
	bool Logout(const string &nick);
	/** log that user logged in with wrong passwd*/
	bool LoginError(nSocket::cConnDC *conn, const string &nick);
	bool DelReg(const string &nick);
	void ReloadCache(){mCache.Clear(); mCache.LoadAll();}
	void UpdateCache(){mCache.Update();}
	nConfig::tCache<string> mCache;
protected: // Protected attributes
	/** reference to a server */
	nSocket::cServerDC *mS;
	cRegUserInfo mModel;
};
};
};

#endif
