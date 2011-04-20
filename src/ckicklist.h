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
#ifndef NDIRECTCONNECT_NTABLESCKICKLIST_H
#define NDIRECTCONNECT_NTABLESCKICKLIST_H
#include "cconfmysql.h"
#include "ckick.h"

namespace nVerliHub {
	using nConfig::cConfMySQL;
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

	}; // namespace nTables
}; // namespace nVerliHub

#endif
