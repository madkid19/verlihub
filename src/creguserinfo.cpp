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

using namespace std;
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
//for the crypt
#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <iomanip>
#include <openssl/md5.h>
#include "creguserinfo.h"
#include "ctime.h"
#include "i18n.h"

#define PADDING 25

namespace nVerliHub {
	using namespace nUtils;
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
	unsigned char buf[MD5_DIGEST_LENGTH+1];
	bool Result = false;
	switch (mPWCrypt)
	{
		case eCRYPT_ENCRYPT:
			crypted_p = crypt(pass.c_str(),mPasswd.c_str());
			Result = crypted_p == mPasswd;
		break;
		case eCRYPT_MD5:
			MD5((const unsigned char*)pass.data(), pass.length(), buf);
			buf[MD5_DIGEST_LENGTH] = 0;
			Result = mPasswd == string((const char*)buf);
		break;
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
	ui.mPWCrypt = i;
	return is;
}

ostream & operator << (ostream &os, cRegUserInfo &ui)
{
	static const char *ClassName[] = {"Guest", "Registered", "VIP", "Operator", "Cheef", "Administrator", "6-err", "7-err", "8-err", "9-err", "Master"};
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Class") << ClassName[ui.mClass] << " [" << ui.mClass << "]\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Last login") << cTime(ui.mLoginLast,0).AsDate() << " " << _("from") << " " << ui.mLoginIP << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Registered since");

	if (ui.mRegDate)
		os << cTime(ui.mRegDate, 0).AsDate() << " " << autosprintf(_("by %s"),ui.mRegOp.c_str());
	else
		os <<  _("No information");

	os << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Last error");

	if (ui.mErrorLast)
		os << cTime(ui.mErrorLast).AsDate() << " " << _("from") << " " << ui.mErrorIP;
	else
		os <<  _("No information");

	os << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Login errors/total") << ui.mErrorCount << "/" << ui.mLoginCount << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Password set") << ((ui.mPasswd.size() != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Account enabled") << ((ui.mEnabled != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Protected") << ((ui.mClassProtect != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Hidden kicks") << ((ui.mHideKick != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Hidden key") << ((ui.mHideKeys != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Hidden share") << ((ui.mHideShare != 0) ? _("Yes") : _("No")) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Authorization IP") << (ui.mAuthIP.empty() ? "--" : ui.mAuthIP) << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Alternate IP") << (ui.mAlternateIP.empty() ? "--" : ui.mAlternateIP);
	return os;
}

string & cRegUserInfo::GetNick(){
	return mNick;
}

/*!
    \fn ::nTables::cRegUserInfo::SetPass(const string &)
 */
void cRegUserInfo::SetPass(string str, int crypt_method)
{
	string salt;
	mPwdChange = !str.size();

	if(str.size()) {
		static const char *saltchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmlopqrstuvwxyz0123456789./";
		static const int saltcharsnum = strlen(saltchars);

		unsigned char charsalt[2] = {((char*)&str)[0],((char*)&str)[1]};
		unsigned char buf[MD5_DIGEST_LENGTH+1];

		switch (crypt_method) {
			case eCRYPT_ENCRYPT:
				charsalt[0] = saltchars[charsalt[0] % saltcharsnum];
				charsalt[1] = saltchars[charsalt[1] % saltcharsnum];
				salt.assign((char *)charsalt,2);

				mPasswd = crypt(str.c_str(),salt.c_str());
				mPWCrypt = eCRYPT_ENCRYPT;
			break;
			case eCRYPT_MD5:

				MD5((const unsigned char *)str.c_str(), str.size(), buf);
				buf[MD5_DIGEST_LENGTH]=0;
				mPasswd = string((char*)buf);
				mPWCrypt = eCRYPT_MD5;

				break;
			case eCRYPT_NONE:
				mPasswd = str;
				mPWCrypt = eCRYPT_NONE;
				break;
		}
	} else
		mPasswd = str;
}

	}; // namespace nTables
}; // namespace nVerliHub
