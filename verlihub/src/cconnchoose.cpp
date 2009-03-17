/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
