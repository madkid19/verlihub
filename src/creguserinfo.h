/***************************************************************************
                          creguserinfo.h  -  description
                             -------------------
    begin                : Mon Jun 16 2003
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

#ifndef CREGUSERINFO_H
#define CREGUSERINFO_H

#include <string>
#include <iostream>

using namespace std;

namespace nDirectConnect {

class cUser;

namespace nTables {

/**info on a registered user
  *@author Daniel Muller
  */

class cRegUserInfo
{
public:
	cRegUserInfo();
	~cRegUserInfo();
	/** verify password - return true if ok */
	bool PWVerify(const string &pass);

	/** friends */
	friend ostream & operator << (ostream &, cRegUserInfo &i);
	friend istream & operator >> (istream &, cRegUserInfo &i);
	friend class nDirectConnect::cUser;
	friend class cRegList;

	/** nickname */
	string & GetNick();
	/** return user class */
	int getClass(){return mClass;};
	void SetPass(string, int crypt_method);

	enum {eCRYPT_NONE, eCRYPT_ENCRYPT, eCRYPT_MD5};
public: // Public attributes
	/** nickname */
	string mNick;
	/** password/hash */
	string mPasswd;
	/** crypted passwd - if mPasswd is a raw pw or a hash */
	int mPWCrypt;
	/** */
	int mClass;
	int mClassProtect;
	int mClassHideKick;
	bool mHideKick;
	bool mHideKeys;
	bool mHideShare;
	long mRegDate;
	string mRegOp;
	unsigned mLoginCount;
	unsigned mErrorCount;
	/** time of last login */
	long mLoginLast;
	long mLogoutLast;
	/** time of last error */
	long mErrorLast;
	/** last login ip */
	string mLoginIP;
	/** last error ip */
	string mErrorIP;
	/** alternate IP address for the ConnectToMe messages ip */
	string mAlternateIP;
	/** can change password */
	bool mPwdChange;
	/** account enabled */
	bool mEnabled;
	string mNoteOp;
	string mNoteUsr;
	string mEmail;
};
};
};

#endif
