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
using namespace std;
// removed _XOPEN8SOURCE, it does not seem to be necessary and it does nto work on some freebsd machines
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
//for the crypt
#define _XOPEN_SOURCE 
#include <unistd.h>
#include <string.h>

#if HAVE_LIBSSL && HAVE_OPENSSL_MD5_H
#include <openssl/md5.h>
#endif
#include "creguserinfo.h"
#include "ctime.h"

using namespace ::nUtils;

namespace nDirectConnect {
namespace nTables {

cRegUserInfo::cRegUserInfo():
	mPWCrypt(eCRYPT_NONE),
	mClass(0),
	mClassProtect(0),
	mClassHideKick(0),
	mHideKick(false),
	mHideKeys(false),
	mHideShare(false),
	mRegDate(0),
	mLoginCount(0),
	mErrorCount(0),
	mLoginLast(0),
	mLogoutLast(0),
	mErrorLast(0),
	mEnabled(1)
{}

cRegUserInfo::~cRegUserInfo(){}

bool cRegUserInfo::PWVerify(const string &pass)
{
	string crypted_p;
#if HAVE_LIBSSL && HAVE_OPENSSL_MD5_H
	unsigned char buf[MD5_DIGEST_LENGTH+1];
#endif
	bool Result = false;
	switch (mPWCrypt)
	{
#if HAVE_LIBCRYPT || HAVE_LIBCRYPTO
		case eCRYPT_ENCRYPT:
			crypted_p=crypt(pass.c_str(),mPasswd.c_str());
			Result = crypted_p == mPasswd;
			break;
#endif
#if HAVE_LIBSSL && HAVE_OPENSSL_MD5_H
		case eCRYPT_MD5:
			MD5((const unsigned char*)pass.data(), pass.length(), buf);
			buf[MD5_DIGEST_LENGTH] = 0;
			Result = mPasswd == string((const char*)buf);
			break;
#endif
		case eCRYPT_NONE:
			Result = pass == mPasswd;
			break;
	}
	return Result;
}

istream & operator >> (istream &is, cRegUserInfo &ui)
{
	int i;
	is >> ui.GetNick() >> i >> ui.mPasswd >> ui.mClass;
#if !HAVE_LIBCRYPT && !HAVE_LIBCRYPTO && !HAVE_LIBSSL
	ui.mPWCrypt = i;
#else
    ui.mPWCrypt = cRegUserInfo::eCRYPT_NONE;
#endif
	return is;
}

ostream & operator << (ostream &os, cRegUserInfo &ui)
{
	static const char *CryptNames[] = {"plain", "encrypted", "md5", "err", "err", "err"};
	os << "[::] Nick: " << ui.mNick << "\r\n";
	os << "[::] Class: " << ui.mClass << "\r\n";
	os << "[::] Password set:" << ((ui.mPasswd.size() != 0)?"Yes":"No") << "\r\n";
	os << "[::] Last login: " << cTime(ui.mLoginLast,0).AsDate() << "\r\n";
	os << "[::] Last IP: " << ui.mLoginIP << "\r\n";
	os << "[::] Last error: " << cTime(ui.mErrorLast).AsDate() << "\r\n"; 
	os << "[::] Error IP: " << ui.mErrorIP << "\r\n";
	os << "[::] Login count: " << ui.mLoginCount << "\r\n";
	os << "[::] Login errors: " << ui.mErrorCount << "\r\n";
	os << "[::] Protected: " << ((ui.mClassProtect != 0)?"Yes":"No") << "\r\n";
	os << "[::] Hidden kicks: " << ((ui.mHideKick != 0)?"Yes":"No") << "\r\n";
	os << "[::] Hidden keys: " << ((ui.mHideKeys != 0)?"Yes":"No") << "\r\n";
	os << "[::] Hidden share: " << ((ui.mHideShare != 0)?"Yes":"No") << "\r\n";
	os << "[::] Registered since: " << cTime(ui.mRegDate,0).AsDate() << "\r\n";
	os << "[::] Registered by: " << ui.mRegOp << "\r\n";
	os << "[::] Alternate IP: " << ui.mAlternateIP;
	return os;
}

/** nickname */
string & cRegUserInfo::GetNick(){
	return mNick;
}

};
};

/*!
    \fn nDirectConnect::nTables::cRegUserInfo::SetPass(const string &)
 */
void nDirectConnect::nTables::cRegUserInfo::SetPass(string str, int crypt_method)
{
	string salt;
	mPwdChange = !str.size();
	
	if(str.size())
	{
#if ! HAVE_LIBCRYPT && ! HAVE_LIBCRYPTO
		if (crypt_method == eCRYPT_ENCRYPT) crypt_method = eCRYPT_MD5;
#endif
#	if ! HAVE_LIBSSL || ! HAVE_OPENSSL_MD5_H
		if (crypt_method == eCRYPT_MD5) crypt_method = eCRYPT_ENCRYPT;
#endif
#if ! HAVE_LIBCRYPT && ! HAVE_LIBCRYPTO
		if (crypt_method == eCRYPT_ENCRYPT) crypt_method = eCRYPT_NONE;
#endif

		static const char *saltchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmlopqrstuvwxyz0123456789./";
		static const int saltcharsnum = strlen(saltchars);

		unsigned char charsalt[2] = {((char*)&str)[0],((char*)&str)[1]};

#if HAVE_LIBSSL && HAVE_OPENSSL_MD5_H
		unsigned char buf[MD5_DIGEST_LENGTH+1];
#endif

		switch (crypt_method)
		{
			case eCRYPT_ENCRYPT:
#if HAVE_LIBCRYPT || HAVE_LIBCRYPTO
				charsalt[0] = saltchars[charsalt[0] % saltcharsnum];
				charsalt[1] = saltchars[charsalt[1] % saltcharsnum];
				salt.assign((char *)charsalt,2);

				mPasswd = crypt(str.c_str(),salt.c_str());
				mPWCrypt = eCRYPT_ENCRYPT;
#endif
				break;
			case eCRYPT_MD5:
#if HAVE_LIBSSL && HAVE_OPENSSL_MD5_H
				MD5((const unsigned char *)str.c_str(), str.size(), buf);
				buf[MD5_DIGEST_LENGTH]=0;
				mPasswd = string((char*)buf);
				mPWCrypt = eCRYPT_MD5;
#endif
				break;
			case eCRYPT_NONE:
				mPasswd = str;
				mPWCrypt = eCRYPT_NONE;
				break;
		}
	}
	else mPasswd = str;
}
