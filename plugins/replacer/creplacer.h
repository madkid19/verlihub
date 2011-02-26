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

#ifndef NDIRECTCONNECTCREPLACER_H
#define NDIRECTCONNECTCREPLACER_H

#include "creplacerworker.h"
#include <vector>
#include "cconfmysql.h"

using std::vector;
using nConfig::cConfMySQL;

namespace nDirectConnect {
namespace nTables {

/**
the vector of triggers, with load, reload, save functions..
@author Daniel Muller
changes by Pralcio
*/

class cReplacer : public cConfMySQL
{
public:
	cReplacer(cServerDC *server);
	~cReplacer();
	void Empty();
	int LoadAll();
	void CreateTable(void);
	int Size(){ return mData.size();}
	void PrepareNew();
	void DeleteLast();

	int AddReplacer(cReplacerWorker &);
	void DelReplacer(cReplacerWorker &);

	string ReplacerParser(const string & str, cConnDC * conn);

	cReplacerWorker * operator[](int );
private:
	typedef vector<cReplacerWorker *> tDataType;
	tDataType mData;
	// a model of a replacer worker
	cReplacerWorker mModel;

	cServerDC *mS;
};

class cReplaceCfg : public cConfigBase
{
public:
	cReplaceCfg(cServerDC *);

	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};

};
};

#endif
