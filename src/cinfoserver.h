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
#ifndef NDIRECTCONNECTINFOSERVER_H
#define NDIRECTCONNECTINFOSERVER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "cconfigbase.h"
#include <ostream>

using std::ostream;

using nConfig::cConfigBase;

namespace nDirectConnect {

class cServerDC;

namespace nInfo {
/**
info about the server
@author Daniel Muller
*/

class cInfoServer : public cConfigBase
{
public:
	cInfoServer();
	~cInfoServer();
	void SystemInfo(ostream &os);
	void SetServer(cServerDC *Server);
	void Output(ostream &os, int Class);
	virtual int Save(){return 0;}
	virtual int Load(){return 0;}
private:
	cServerDC *mServer;
};
};
};

#endif
