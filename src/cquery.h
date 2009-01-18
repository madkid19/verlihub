/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NMYSQLCQUERY_H
#define NMYSQLCQUERY_H

#include "cmysql.h"
#include "cobj.h"
#include <sstream>

using namespace std;
namespace nMySQL {

/**
mysql query, contains result etc..

@author Daniel Muller
*/
class cQuery : public cObj
{
public:
	cQuery(cMySQL & mysql);
	cQuery(cQuery & query);
	~cQuery();
	// clear the query and result, etc..
	void Clear();
	// return the stream to build the query
	ostringstream & OStream(){ return mOS;}
	// perform the query, return -1 on error
	int Query();
	// store result for iterating through it
	int StoreResult();
	// fetch next row from result
	MYSQL_ROW Row();
	// return the number of colums in the query result
	int Cols();
	// seek data row by the given number
	void DataSeek(unsigned long long);
	// is there an existing result?
	bool GetResult();
private:
	cMySQL & mMySQL;
	MYSQL_RES *mResult;
	//string mQuery;
	ostringstream mOS;
};

};

#endif
