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
#ifndef CSIPS_H
#define CSIPS_H

#include <string>
#include "src/cconndc.h"
#include "src/tlistplugin.h"

using namespace std;
using namespace ::nEnums;
using namespace ::nPlugin;

namespace { class cServerDC;};
using namespace ;

class cpiISP;
class cISPs;

class cISP
{
public:
	cISP();
	virtual ~cISP();

	// -- stored data
	unsigned long mIPMin;
	unsigned long mIPMax;
	string mCC;
	string mName;
	string mAddDescPrefix;
	string mNickPattern;
	string mPatternMessage;
	string mConnPattern;
	string mConnMessage;
	// min share in GB
	long mMinShare[4];
	long mMaxShare[4];

	// --- memory data
	cPCRE *mpNickRegex;
	cPCRE *mpConnRegex;
	bool mOK;

	//-- methods
	bool CheckNick(const string &Nick, const string &cc);
	bool CheckConn(const string &ConnType);
	int CheckShare(int cls, __int64 share, __int64 min_unit, __int64 max_unit);
	
	cpiISP *mPI;
	virtual void OnLoad();
	friend ostream& operator << (ostream &, const cISP &isp);
};


class cISPCfg : public cConfigBase
{
public:
	cISPCfg(cServerDC *);
	int max_check_nick_class;
	int max_check_conn_class;
	int max_check_isp_class;
	int max_insert_desc_class;
	long unit_min_share_bytes;
	long unit_max_share_bytes;
	string msg_share_more;
	string msg_share_less;
	string msg_no_isp;
	bool allow_all_connections;
	bool case_sensitive_nick_pattern;

	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};

typedef class tOrdList4Plugin<cISP, cpiISP> tISPListBase;

class cISPs : public tISPListBase
{
public:
	cISPs(cVHPlugin *pi);	
	virtual void AddFields();
	cISP *FindISPByCC(const string &cc);
	cISP *FindISP(const string &ip,const string &cc);
	virtual bool CompareDataKey(const cISP &D1, const cISP &D2);
	virtual int OrderTwoItems(const cISP &D1, const cISP &D2);
	virtual cISP *AddData(cISP const &isp);
	virtual void DelData(cISP &isp);
	virtual void OnLoadData(cISP &Data);
protected:
	// CC hash ?? either quickly locate the isp by given CC - complicates the CC parsing
	// or just maintain a list of CC isps - simplier, slower (but probably not much, easier to use)
	// I deciced for the second way - 
	typedef vector<cISP*> tISPCCList;
	tISPCCList mCCList;
};

#endif//CSIPS_H
