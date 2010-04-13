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
