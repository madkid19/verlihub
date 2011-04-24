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
#ifndef CREGUSERINFO_H
#define CREGUSERINFO_H
#include <string>
#include <iostream>

using namespace std;

namespace nVerliHub {

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

		/** friends */
		friend ostream & operator << (ostream &, cRegUserInfo &i);
		friend istream & operator >> (istream &, cRegUserInfo &i);
		friend class cUser;
		friend class cRegList;

		/**
		* Get the class of the user.
		* @return The class
		*/
		int getClass(){return mClass;};

		/**
		* Get the nickname of the user.
		* @return The nickname
		*/
		string & GetNick();

		/**
		* Verify the password of the user.
		* @param password The password to check
		* @return True if password matches or false on failure
		*/
		bool PWVerify(const string &pass);

		/**
		* Set user passwrod.
		* @param password The new password
		* @param crypt_method The crypt method to use
		* @return Zero on success or -1 on failure
		*/
		void SetPass(string password, int crypt_method);

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
	}; // nTables
}; // nVerliHub

#endif
