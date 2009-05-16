/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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
	cMySQL& getMySQL() { return mMySQL;}
private:
	cMySQL & mMySQL;
	MYSQL_RES *mResult;
	//string mQuery;
	ostringstream mOS;
};

};

#endif
