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
#include "src/cserverdc.h"
#include "src/cconndc.h"

namespace nVerliHub {
	namespace nSocket {
		class cConnDC;
		class cServerDC;
	};
	namespace nEnums {
		enum {
			eLT_CONNECT =  0,
			eLT_LOGIN = 1,
			eLT_LOGOUT = 2,
			eLT_DISCONNECT = 3,
			eLT_DEFAULT = 1 << eLT_CONNECT |  1 << eLT_LOGOUT,
			eLT_ALL = 255
		};

	};
	namespace nIPLogPlugin {

struct sUserStruct {
	sUserStruct()
	{
		mDate=0;
		mIP = 0;
		mType=0;
		mInfo=0;
	}
	long mDate;
	unsigned long mIP;
	int mType;
	int mInfo;
	string mNick;
};

/**
@author Daniel Muller
*/
class cIPLog : public nConfig::cConfMySQL
{
public:
	nSocket::cServerDC * mS;
	cIPLog(nSocket::cServerDC *server);
        //bool SetVar(const string &nick, string &field, string &value);
	/** log that user logged in */
        bool Log(nSocket::cConnDC *conn, int action, int info);
	void AddFields();
	virtual void CleanUp();
	virtual ~cIPLog();

	void GetIPHistory(const string &ip, int limit, ostream &os);
	void GetNickHistory(const string &nick, int limit, ostream &os);
	void GetHistory(const string &who, bool isNick, int limit, ostream &os);

	void GetLastIP(const string &nick, int limit, ostream &os);
	void GetLastNick(const string &ip, int limit, ostream &os);
	void GetLastLogin(const string &who, bool isNick, int limit, ostream &os);

	void MakeSearchQuery(const string &who, bool IsNick, int action, int limit);
	struct sUserStruct mModel;
};
	}; // namespace nIPLogPlugin
}; // namespace nVerliHub
#endif
