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
#ifndef NDIRECTCONNECTCFORBIDDEN_H
#define NDIRECTCONNECTCFORBIDDEN_H

#include <string>
#include "src/cpcre.h"
#include <vector>
#include "src/tlistplugin.h"
	

using namespace std;
using namespace nUtils;
using std::vector;
using nConfig::tMySQLMemoryList;

class cpiForbid;

namespace  {
	class cConnDC;
	class cServerDC;

	using namespace nPlugin;
namespace nTables {

/**
a trigger command ...
user defined string that triggers given action

@author Daniel Muller
*/

class cForbiddenWorker
{
public:
	cForbiddenWorker();
	~cForbiddenWorker();
	int DoIt(const string & cmd_line, cConnDC *conn, cServerDC *server, int mask);
	bool CheckMsg(const string &msg);
	bool PrepareRegex();
	// the forbidden word
	string mWord;
	// the checking mask
	int mCheckMask;
	// affected class
	int mAfClass;
	
	string mReason;
	
	enum {eCHECK_CHAT = 1, eCHECK_PM = 2, eNOTIFY_OPS = 4};
	
	virtual void OnLoad();
	friend ostream &operator << (ostream &, cForbiddenWorker &);
private:
	cPCRE *mpRegex;
};

typedef tList4Plugin<cForbiddenWorker, cpiForbid> tForbiddenBase;

/**
the vector of triggers, with load, reload, save functions..

@author Daniel Muller
*/
class cForbidden : public tForbiddenBase
{
public:
	cForbidden(cVHPlugin *pi); 
	virtual ~cForbidden(){};
	virtual void AddFields();
	virtual bool CompareDataKey(const cForbiddenWorker &D1, const cForbiddenWorker &D2);
	
	int ForbiddenParser(const string & str, cConnDC * conn, int mask);
	int CheckRepeat(const string & , int);
	int CheckUppercasePercent(const string & , int);
};

class cForbidCfg : public cConfigBase
{
public: 
	cForbidCfg(cServerDC *);
	int max_upcase_percent;
	int max_repeat_char;
	int max_class_dest;
	
	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};

};
};

#endif
