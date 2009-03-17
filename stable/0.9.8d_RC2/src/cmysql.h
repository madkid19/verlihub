/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	public: void Error(int level, string text);

private:
	MYSQL *mDBHandle;
};
};
#endif
