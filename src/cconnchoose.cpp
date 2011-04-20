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

#include "cconnchoose.h"

namespace nVerliHub {
	using namespace nEnums;
	namespace nSocket {

	cConnChoose::iterator cConnChoose::sBegin;
	cConnChoose::iterator cConnChoose::sEnd;

cConnChoose::cConnChoose()
{
	mLastSock = 0;
}


cConnChoose::~cConnChoose(){}


#if defined USE_OLD_CONNLIST || defined _WIN32
bool cConnChoose::AddConn(cConnBase *conn)
{
	if (!conn) return false;
	tSocket sock = (tSocket)(*conn);
	if (mConnList.ContainsHash(sock)) return false;
	if (sock > mLastSock) mLastSock = sock;
	bool ret = mConnList.AddWithHash(conn, sock);
	return ret;
}

bool cConnChoose::DelConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	OptOut(conn, eCC_ALL);
 	OptOut(conn, eCC_CLOSE);
	return mConnList.RemoveByHash(sock);
}

bool cConnChoose::HasConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	return mConnList.ContainsHash(sock);
}

inline cConnBase * cConnChoose::operator[] (tSocket sock)
{
	return mConnList.GetByHash(sock);
}

#else

bool cConnChoose::AddConn(cConnBase *conn)
{
	if ( conn == NULL ) return false;
	tSocket sock = (tSocket)(*conn);
	// resize
	if ( (tSocket)mConnList.size() <= sock ) mConnList.resize(sock+sock/4, NULL);
	// don't add twice
 	if ( mConnList[sock] != NULL ) return false;

	if (sock > mLastSock) mLastSock = sock;

	mConnList[sock] = conn;
	return true;
}

bool cConnChoose::DelConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	if ( (tSocket)mConnList.size() <= sock ) return false;
	OptOut(conn, eCC_ALL);
 	OptOut(conn, eCC_CLOSE);
	mConnList[sock] = NULL;
	return true;
}

bool cConnChoose::HasConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	if ( (tSocket)mConnList.size() <= sock ) return false;
	return mConnList[sock] != NULL;
}

inline cConnBase * cConnChoose::operator[] (tSocket sock)
{
	if(tSocket(mConnList.size()) > sock)
		return mConnList[sock];
	else
		return NULL;
}
#endif

inline void cConnChoose::OptIn (cConnBase* conn, nEnums::tChEvent mask)
{
	if(!conn)
		return;
	this->OptIn(tSocket(*conn), mask);
}

inline void cConnChoose::OptOut(cConnBase* conn, nEnums::tChEvent mask)
{
	if(!conn)
		return;
	this->OptOut(tSocket(*conn), mask);
}

inline int cConnChoose::OptGet(cConnBase *conn)
{
	if(!conn)
		return 0;
	return this->OptGet(tSocket(*conn));
}

inline int cConnChoose::RevGet(cConnBase *conn)
{
	if(!conn)
		return 0;
	return this->RevGet(tSocket(*conn));
}

inline bool cConnChoose::RevTest(cConnBase *conn)
{
	if(!conn)
		return false;
	return this->RevTest(tSocket(*conn));
}

inline tSocket cConnChoose::operator[] (cConnBase *conn)
{
	if(!conn)
		return INVALID_SOCKET;
	return (tSocket)(*conn);
}
	}; // namespace nSocket
}; // namespace nVerliHub
