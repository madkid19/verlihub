/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECTINFOSERVER_H
#define NDIRECTCONNECTINFOSERVER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "cconfigbase.h"
#if defined _WIN32 || HAVE_OSTREAM
#include <ostream>
#else
namespace std{
#include <ostream.h>
};
#endif


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
	string converByte(__int64 byte, bool UnitType);
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
