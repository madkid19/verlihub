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
#ifndef NPLUGINTLISTPLUGIN_H
#define NPLUGINTLISTPLUGIN_H
#include "cvhplugin.h"
#include "tmysqlmemorylist.h"
#include "tmysqlmemoryhash.h"
#include "tmysqlmemoryordlist.h"
#include "cserverdc.h"

using namespace nUtils;

namespace 
{
namespace nPlugin
{

template <class DATA_TYPE, class PLUGIN_TYPE>
class tList4Plugin : public tMySQLMemoryList<DATA_TYPE, PLUGIN_TYPE>
{
public:
	tList4Plugin(cVHPlugin *pi, const string & TableName):
		tMySQLMemoryList<DATA_TYPE,PLUGIN_TYPE>(pi->mServer->mMySQL, (PLUGIN_TYPE*)pi, TableName)
	{}
	virtual ~tList4Plugin() {}
};

template <class DATA_TYPE, class PLUGIN_TYPE>
class tHash4Plugin : public tMySQLMemoryHash<DATA_TYPE, PLUGIN_TYPE>
{
public:
	tHash4Plugin(cVHPlugin *pi, const string & TableName):
		tMySQLMemoryHash<DATA_TYPE,PLUGIN_TYPE>(pi->mServer->mMySQL, (PLUGIN_TYPE*)pi, TableName)
	{}
	virtual ~tHash4Plugin() {}
};


template <class DATA_TYPE, class PLUGIN_TYPE>
class tOrdList4Plugin : public tMySQLMemoryOrdList<DATA_TYPE, PLUGIN_TYPE>
{
public:
	tOrdList4Plugin(cVHPlugin *pi, const string & TableName, const string &DbOrder):
		tMySQLMemoryOrdList<DATA_TYPE,PLUGIN_TYPE>(pi->mServer->mMySQL, (PLUGIN_TYPE*)pi, TableName, DbOrder)
	{}
	virtual ~tOrdList4Plugin() {}
};

template <class LIST_TYPE, class CONSOLE_TYPE>
class tpiListPlugin : public cVHPlugin
{
public:
	tpiListPlugin() : mConsole(this), mList(NULL) {}
	virtual ~tpiListPlugin() {
		if (mList != NULL) delete mList;
		mList = NULL;
	}
	
	virtual bool RegisterAll() {
		RegisterCallBack("VH_OnUserCommand");
		return true;
	}
	
	virtual bool OnUserCommand(cConnDC *conn, string *str) {
		return !(mConsole.DoCommand(*str, conn));
	}

	virtual void OnLoad(cServerDC *server){
		cVHPlugin::OnLoad(server);
	
		mList = new LIST_TYPE(this);
		mList->OnStart();
	}
	CONSOLE_TYPE mConsole;
	LIST_TYPE* mList;
};
};
};

#endif
