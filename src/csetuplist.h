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
