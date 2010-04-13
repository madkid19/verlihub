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
#include "cconnchoose.h"

namespace nServer {

cConnChoose::iterator cConnChoose::sBegin;
cConnChoose::iterator cConnChoose::sEnd;

cConnChoose::cConnChoose()
{
	mLastSock = 0;
}


cConnChoose::~cConnChoose(){}
};

#if defined USE_OLD_CONNLIST || defined _WIN32
bool nServer::cConnChoose::AddConn(cConnBase *conn)
{
	if (!conn) return false;
	tSocket sock = (tSocket)(*conn);
	if (mConnList.ContainsHash(sock)) return false;
	if (sock > mLastSock) mLastSock = sock;
	bool ret = mConnList.AddWithHash(conn, sock);
	return ret;
}

bool nServer::cConnChoose::DelConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	OptOut(conn, eCC_ALL);
 	OptOut(conn, eCC_CLOSE);
	return mConnList.RemoveByHash(sock);
}

bool nServer::cConnChoose::HasConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	return mConnList.ContainsHash(sock);
}
#else
bool nServer::cConnChoose::AddConn(cConnBase *conn)
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

bool nServer::cConnChoose::DelConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	if ( (tSocket)mConnList.size() <= sock ) return false;
	OptOut(conn, eCC_ALL);
 	OptOut(conn, eCC_CLOSE);
	mConnList[sock] = NULL;
	return true;
}

bool nServer::cConnChoose::HasConn(cConnBase *conn)
{
	tSocket sock = (tSocket)(*conn);
	if ( (tSocket)mConnList.size() <= sock ) return false;
	return mConnList[sock] != NULL;
}
#endif
