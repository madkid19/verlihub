/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cquery.h"

namespace nMySQL {

cQuery::cQuery(cMySQL & mysql)
	: cObj("nMySQL::cQuery"), mMySQL(mysql), mResult(NULL)
{}

cQuery::cQuery(cQuery & query)
	: cObj("nMySQL::cQuery"), mMySQL(query.mMySQL), mResult(NULL)
{}

cQuery::~cQuery()
{
}

void cQuery::Clear(void)
{
	if(mResult)
	{
		mysql_free_result(mResult);
		mResult = NULL;
	}
	mOS.str(mEmpty);
}

int cQuery::Query()
{
	string qstr(mOS.str());
	if(Log(3)) LogStream() << "Execute query ~" << qstr << "~" << endl;
	if(mysql_query(mMySQL.mDBHandle, qstr.c_str()))
	{
		if(ErrLog(1)) LogStream() << "Error in query ~" << qstr << "~" << endl;
		mMySQL.Error(2,string("Query error:"));
		return -1;
	}
	return 1;//mysql_affected_rows(mMySQL.mDBHandle);
}

int cQuery::StoreResult()
{
	mResult = mysql_store_result(mMySQL.mDBHandle);
	if (mResult != NULL)
		return mysql_num_rows(mResult);
	else return 0;
}

MYSQL_ROW cQuery::Row()
{
	if(!mResult) return NULL;
	return mysql_fetch_row(mResult);
}

int cQuery::Cols()
{
	return mysql_num_fields(mResult);
}

void cQuery::DataSeek(unsigned long long r)
{
	mysql_data_seek(mResult, r);
}

bool cQuery::GetResult()
{
	return mResult ? true : false;
}

};
