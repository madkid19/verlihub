/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECT_NTABLESCKICKLIST_H
#define NDIRECTCONNECT_NTABLESCKICKLIST_H

#include "cconfmysql.h"
#include "ckick.h"

using nConfig::cConfMySQL;

namespace nDirectConnect {

class cConnDC;

namespace nTables {

/**
@author Daniel Muller
*/
class cKickList : public cConfMySQL
{
public:
	cKickList(cMySQL &mysql);
	~cKickList();
	bool AddKick(cConnDC *,const string& OPNick, const string *, cKick &);
	bool FindKick(cKick &Dest,const string& Nick, const string &OpNick, unsigned Age, bool WithReason, bool WithDrop, bool IsNick=true);
	void Cleanup();
protected:
	cKick mModel;
};

};

};

#endif
