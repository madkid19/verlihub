/***************************************************************************
 *   Copyright (C) 2004 by Pralcio                                         *
 *   based on "Forbid" code made by                                        *
 *   Daniel Muller                                                         *
 *   dan at verliba dot cz                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	int DeleteLast();

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
