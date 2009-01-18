/***************************************************************************
                          creglist.h  -  description
                             -------------------
    begin                : Mon Jul 7 2003
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

#ifndef CREGLIST_H
#define CREGLIST_H


#include <string>
#include "cconfmysql.h"
#include "creguserinfo.h"
#include "tcache.h"

using namespace std;
using namespace nMySQL;
using namespace nConfig;

namespace nDirectConnect {

// forward declarations
class cConnDC;
class cServerDC;

namespace nTables {

// forward declarations
//class cRegUserInfo;

/**a reg users list
  *@author Daniel Muller
  */

class cRegList : public cConfMySQL
{
public:
	cRegList(cMySQL &mysql,cServerDC *);
	virtual ~cRegList();
	/** find nick in reglist
		if not foud return 0
		else return 1 and fill in the reuserinfo parameter */
	bool FindRegInfo(cRegUserInfo &, const string &nick);
	/** add registered user */
	bool AddRegUser(const string &nick, cConnDC *op, int cl, const char *password = NULL);
	/** No descriptions */
	bool ChangePwd(const string &nick, const string &pwd,int);
	/** No descriptions */
	bool SetVar(const string &nick, string &field, string &value);
	/** log that user logged in */
	bool Login(cConnDC *conn, const string &nick);
	bool Logout(const string &nick);
	/** log that user logged in with wrong passwd*/
	bool LoginError(cConnDC *conn, const string &nick);
	bool DelReg(const string &nick);
	void ReloadCache(){mCache.Clear(); mCache.LoadAll();}
	void UpdateCache(){mCache.Update();}
	tCache<string> mCache;
protected: // Protected attributes
	/** reference to a server */
	cServerDC *mS;
	cRegUserInfo mModel;
};
};
};

#endif
