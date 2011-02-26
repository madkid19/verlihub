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
#ifndef CMYSQL_H
#define CMYSQL_H
#if ! defined _WIN32
//#include <mysql/mysql.h>
#else
typedef unsigned int SOCKET;
#endif
#include <mysql.h>
#include "cobj.h"

/**
   Mysql utilities
   Contains classes that encapsulate mysql structures,  and ease their usage
*/
namespace nMySQL {

/**
a class encapsulating operations with mysql conenction

@author Daniel Muller
*/
class cMySQL : public cObj
{
friend class cQuery;
public:
	cMySQL();
	cMySQL(string&host,string&user,string&pass,string&data);
	~cMySQL();
	void Init();
	bool Connect(string &host, string &user, string &passwd, string &db);
	string GetDBName()
	{
		return mDBName;
	}
	public: void Error(int level, string text);

private:
	string mDBName;
	MYSQL *mDBHandle;
	
};
};
#endif
