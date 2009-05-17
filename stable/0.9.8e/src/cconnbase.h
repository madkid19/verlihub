/***************************************************************************
                          connbase.h  -  description
                             -------------------
    begin                : 15/01/04 14:01
    copyright            : (C) 2004 by Daniel Muller
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

#ifndef NSERVERCCONNBASE_H
#define NSERVERCCONNBASE_H

namespace nServer {

#if !defined _WIN32
	typedef int tSocket;
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
#else
	#include <winsock2.h>
	typedef SOCKET tSocket;
#endif

/** a Base class for cConnChoose Usage - for connections
	provides pure virtual conversion to a tSocket (uint or SOCKET) type
 */
class cConnBase
{
	public: virtual operator tSocket() const =0;
};

};

#endif
