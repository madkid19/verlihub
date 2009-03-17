/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECT_NTABLES_CSETUPLIST_H
#define NDIRECTCONNECT_NTABLES_CSETUPLIST_H

#include "cconfmysql.h"
#include <string>

using namespace std;
using namespace nConfig;

namespace nDirectConnect {

namespace nTables  {

/**
table containing hub's setup variables that used to be in config file

@author Daniel Muller
*/

class cSetupList: public cConfMySQL
{
public:
	/**
		a structure representing setup data (setup-file, varname and var value)
	*/
	class cSetup
	{
	public:
		cSetup(){};
		cSetup(string const & file, string const &var,string const &val):mFile(file),mVarName(var),mVarValue(val){}
		string mFile;
		string mVarName;
		string mVarValue;
	};
	cSetupList(cMySQL &mysql);
	~cSetupList();
	void LoadFileTo(cConfigBaseBase *, const char*);
	void SaveFileTo(cConfigBaseBase *, const char*);
	void OutputFile(const char *, ostream &os);
	bool SaveItem(const char *InFile, cConfigItemBase *);
	bool LoadItem(const char *FromFile, cConfigItemBase *);
private:
	cSetup mModel;
};


};

};

#endif
