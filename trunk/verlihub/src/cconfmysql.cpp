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
#include "cconfmysql.h"
#include <algorithm>
#include <string.h>
#define DEFAULT_COLLATION "utf8_unicode_ci"
#define DEFAULT_CHARSET "utf8"

using namespace std;

namespace nConfig
{

cMySQLColumn::cMySQLColumn()
{}

cMySQLColumn::~cMySQLColumn()
{}

void cMySQLColumn::AppendDesc(ostream &os) const
{
	string isNull, hasDefault;
	mNull ? isNull = "" : isNull = " NOT NULL";
	//Who added charset here? Query fails
	os << mName << " " << mType /*<< " CHARACTER SET utf8 COLLATE utf8_unicode_ci "*/ << isNull;
	if (mDefault.size())
	{
		os << " DEFAULT '";
		cConfMySQL::WriteStringConstant(os, mDefault);
		os << "'";
	}
}

void cMySQLColumn::ReadFromRow(const MYSQL_ROW &row)
{
	mName = row[0]?row[0]:""; // just for sure
	mType = row[1]?row[1]:""; // just for sure
	mDefault = row[4]?row[4]:"";
	mNull = ((row[2] != NULL) && strlen(row[2]));
}

bool cMySQLColumn::operator!=(const cMySQLColumn &col) const
{
	return (this->mType != col.mType) || ((this->mDefault != col.mDefault) && this->mDefault.size());
}

cMySQLTable::cMySQLTable(cMySQL &mysql) : cObj("cMySQLTable"),  mQuery(mysql)
{}

cMySQLTable::~cMySQLTable()
{}

bool cMySQLTable::GetCollation()
{
            int i = 0, n;
            MYSQL_ROW row;            
            mQuery.OStream() << "SELECT TABLE_COLLATION FROM information_schema.TABLES WHERE TABLE_NAME='" << mName << "' AND TABLE_SCHEMA='" << mQuery.getMySQL().GetDBName() << "'";
            if(mQuery.Query() <= 0) {
	 mQuery.Clear();
	 return false;
            }
            
            n = mQuery.StoreResult();
            
            cMySQLColumn col;
            for( ; i < n; i++)
            {
	 row = mQuery.Row();
	 mCollation = row[0];
            }
            
            mQuery.Clear();
            return true;
}

bool cMySQLTable::GetDescription(const string &tableName)
{
	int i = 0, n;
	MYSQL_ROW row;

	mName = tableName;
	
	
	mQuery.OStream() << "SHOW COLUMNS FROM " << tableName;
	if(mQuery.Query() <= 0)
	{
		mQuery.Clear();
		return false;
	}

	n = mQuery.StoreResult();

	cMySQLColumn col;
	for( ; i < n; i++)
	{
		// 0=Field, 1=Type, 2=Null, 3=Key, 4=Default, 5=Extra
		row = mQuery.Row();
		col.ReadFromRow(row);
		mColumns.push_back(col);
	}

	mQuery.Clear();
	return true;
}

const cMySQLColumn * cMySQLTable::GetColumn(const string &colName) const
{
	vector<cMySQLColumn>::const_iterator it;
	for(it = mColumns.begin(); it != mColumns.end(); ++it)
		if (it->mName == colName) return &(*it);
	return NULL;
}

bool cMySQLTable::CreateTable()
{
	vector<cMySQLColumn>::iterator it;
	bool IsFirstCol = true;

	// Try to create first
	mQuery.OStream() << "CREATE TABLE IF NOT EXISTS " << mName << " (";
	for(it = mColumns.begin(); it != mColumns.end(); ++it)
	{
		mQuery.OStream() << (IsFirstCol?"":", ");
		it->AppendDesc(mQuery.OStream());
		IsFirstCol = false;
	}

	if(mExtra.size())
		mQuery.OStream() << ", " << mExtra;
	//TODO: Alter charset if db_charset changes
	mQuery.OStream() << ") CHARACTER SET " << DEFAULT_CHARSET << " COLLATE " << DEFAULT_COLLATION;
	mQuery.Query();
	mQuery.Clear();
	return true;
}

void cMySQLTable::TruncateTable()
{
	mQuery.OStream() << "TRUNCATE TABLE " << mName;
	mQuery.Query();
	mQuery.Clear();
}


bool cMySQLTable::AutoAlterTable(const cMySQLTable &original)
{
	string isNull, hasDefault;
	vector<cMySQLColumn>::iterator it;
	const cMySQLColumn *col;
	bool NeedAdd = false, NeedModif = false, result = false;

	for(it = mColumns.begin(); it != mColumns.end(); ++it)
	{
		NeedAdd = NeedModif = false;

		if ((col = original.GetColumn(it->mName)) == NULL)
			NeedAdd = true;
		else
			NeedModif = ((*it) != (*col));

		if (NeedAdd || NeedModif)
		{
			result = true;
			if(Log(1)) LogStream() << "Altering table " << mName << (NeedAdd?" add column ":" modify column")
				<< it->mName << " with type: " << it->mType << endl;
			mQuery.OStream() << "ALTER TABLE  " << mName << (NeedAdd?" ADD COLUMN ":" MODIFY COLUMN ");
			it->AppendDesc(mQuery.OStream());
			mQuery.Query();
			mQuery.Clear();
		}
	}
	GetCollation();
	if(mCollation != DEFAULT_COLLATION) {
		if(Log(1)) LogStream() << "Altering table " << mName << " setting collation to " << DEFAULT_COLLATION << endl;
		mQuery.OStream() << "ALTER TABLE  " << mName << " CHARACTER SET " << DEFAULT_CHARSET << " COLLATE " << DEFAULT_COLLATION;
		mQuery.Query();
		mQuery.Clear();
	}
	return result;
}

cConfMySQL::cConfMySQL(cMySQL &mysql) :
	mMySQL(mysql), mQuery(mMySQL), mMySQLTable(mMySQL)
{
	if (mItemCreator != NULL) delete mItemCreator;
	mItemCreator = new cMySQLItemCreator;
}

cConfMySQL::~cConfMySQL()
{
}

void cConfMySQL::CreateTable()
{
	cMySQLTable existing_desc(mMySQL);

	if (existing_desc.GetDescription(mMySQLTable.mName))
		mMySQLTable.AutoAlterTable(existing_desc);
	else
		mMySQLTable.CreateTable();
}

void cConfMySQL::TruncateTable()
{
	mMySQLTable.TruncateTable();
}

int cConfMySQL::Save()
{
	SavePK(false);
	return 0;
}

int cConfMySQL::Load()
{
	return Load(mQuery);
}

int cConfMySQL::Load(cQuery &Query)
{
	MYSQL_ROW row;
	if(!(row= Query.Row())) return -1;

	for_each(mhItems.begin(), mhItems.end(), ufLoad(row));
	return 0;
}

/** return -1 on error, otherwinse number of results */
int cConfMySQL::StartQuery()
{
	return StartQuery(mQuery);
}

int cConfMySQL::StartQuery(cQuery &Query)
{
	int ret = Query.Query();
	if(ret <= 0)
	{
		Query.Clear();
		return ret;
	}
	Query.StoreResult();
	mCols = Query.Cols();
	return ret;
}

/** return -1 on error, otherwinse number of results */
int cConfMySQL::StartQuery(string query)
{
	mQuery.OStream() << query;
	return StartQuery();
}

int cConfMySQL::EndQuery()
{
	return EndQuery(mQuery);
}

int cConfMySQL::EndQuery(cQuery &Query)
{
	Query.Clear();
	return 0;
}

};


/*!
    \fn nConfig::cConfMySQL::AddPrimaryKey(const char*)
 */
void nConfig::cConfMySQL::AddPrimaryKey(const char*key)
{
	string Key(key);
	tItemHashType Hash = msHasher(Key);
	cConfigItemBase *item = mhItems.GetByHash(Hash);
	if( item != NULL) mPrimaryKey.AddWithHash(item,Hash);
}


/*!
    \fn nConfig::cConfMySQL::WherePKey(ostream &os)
 */
void nConfig::cConfMySQL::WherePKey(ostream &os)
{
	os << " WHERE (";
	AllPKFields(os , true, true, false, string(" AND "));
	os << " )";
}


/*!
    \fn nConfig::cConfMySQL::AllFields(ostream &, bool DoF=true, bool DoV=false, string joint)
 */
void nConfig::cConfMySQL::AllFields(ostream &os, bool DoF, bool DoV, bool IsAff, string joint)
{
	 for_each(mhItems.begin(), mhItems.end(), ufEqual(os, joint , DoF, DoV, IsAff));
}

/*!
    \fn nConfig::cConfMySQL::AllPKFields(ostream &, bool DoF=true, bool DoV=false, string joint)
 */
void nConfig::cConfMySQL::AllPKFields(ostream &os, bool DoF, bool DoV, bool IsAff, string joint)
{
	 for_each(mPrimaryKey.begin(), mPrimaryKey.end(), ufEqual(os, joint , DoF, DoV, IsAff));
}

/*!
    \fn nConfig::cConfMySQL::SelectFields(ostream &)
 */
void nConfig::cConfMySQL::SelectFields(ostream &os)
{
	os << "SELECT ";
	AllFields(os, true, false, false,string(", "));
	os << " FROM " << mMySQLTable.mName << " ";
}


/*!
    \fn nConfig::cConfMySQL::UpdateFields(ostream &os)
 */
void nConfig::cConfMySQL::UpdateFields(ostream &os)
{
	os << "UPDATE " << mMySQLTable.mName  << " SET ";
	AllFields(mQuery.OStream(), true, true, true, string(", "));
}


/*!
    \fn nConfig::cConfMySQL::LoadPK()
 */
bool nConfig::cConfMySQL::LoadPK()
{
	ostringstream query;

	SelectFields( query );
	WherePKey( query );

	if( StartQuery(query.str()) == -1) return false;
	bool found = (Load() >= 0);
	EndQuery();
	return found;
}


/*!
    \fn nConfig::cConfMySQL::SavePK(bool dup)
 */
bool nConfig::cConfMySQL::SavePK(bool dup)
{
	mQuery.OStream() << "INSERT IGNORE INTO " << mMySQLTable.mName << " (";
	AllFields(mQuery.OStream(), true, false, false, string(", "));
	mQuery.OStream() << ") VALUES (";
	AllFields(mQuery.OStream(), false, true, true, string(", "));
	mQuery.OStream() << ")";
	if(dup)
	{
		mQuery.OStream() << " ON DUPLICATE SET ";
		AllFields(mQuery.OStream(), true, true, true, string(", "));
	}
	bool ret = mQuery.Query();
	mQuery.Clear();
	return ret;
}


/*!
    \fn nConfig::cConfMySQL::DeletePK()
 */
void nConfig::cConfMySQL::DeletePK()
{
	mQuery.Clear();
	mQuery.OStream() << "DELETE FROM " << mMySQLTable.mName << " ";
	WherePKey( mQuery.OStream() );
	mQuery.Query();
	mQuery.Clear();
}

nConfig::cConfMySQL::db_iterator &nConfig::cConfMySQL::db_begin()
{
	return db_begin(mQuery);
}

nConfig::cConfMySQL::db_iterator &nConfig::cConfMySQL::db_begin(cQuery &Query)
{
	if((-1 == this->StartQuery(Query)) || (Load(Query) < 0))
		mDBBegin = NULL;
	else
		mDBBegin = db_iterator(this,&Query);
	return mDBBegin;
}

nConfig::cConfMySQL::db_iterator &nConfig::cConfMySQL::db_iterator::operator++()
{
	if ((mConf != NULL) && (mQuery != NULL))
	{
		if (mConf->Load(*mQuery) < 0)
		{
			mConf->EndQuery(*mQuery);
			mConf = NULL;
			mQuery = NULL;
		}
	}
	return *this;
}

std::ostream &nConfig::cConfigItemMySQLPChar::WriteToStream (std::ostream& os)
{
	if( !IsEmpty() )
	{
		os << '"';
		cConfMySQL::WriteStringConstant(os, this->Data());
		os << '"';
	} else os << " NULL ";
	return os;
}

std::ostream &nConfig::cConfigItemMySQLString::WriteToStream (std::ostream& os)
{
	if( !IsEmpty() )
	{
		os << '"';
		cConfMySQL::WriteStringConstant(os, this->Data());
		os << '"';
	} else os << " NULL ";
	return os;
}

//@todo readfrom stream and char*

/*!
    \fn nConfig::cConfMySQL::UpdatePKVar(const char* var_name, string &new_val)
 */
bool nConfig::cConfMySQL::UpdatePKVar(const char* var_name, string &new_val)
{
	cConfigItemBase * item = NULL;
	string var(var_name);
	item = operator[](var);
	if(!item) return false;
	LoadPK();
	item->ConvertFrom(new_val);
	return UpdatePKVar(item);
}


/*!
    \fn nConfig::cConfMySQL::UpdatePKVar(cConfigItemBase *item)
 */
bool nConfig::cConfMySQL::UpdatePKVar(cConfigItemBase *item)
{
	mQuery.OStream() << "UPDATE " << mMySQLTable.mName << " SET ";
	ufEqual(mQuery.OStream(), string(", "), true, true, true)(item);
	WherePKey(mQuery.OStream());
	bool ret = mQuery.Query();
	mQuery.Clear();
	return ret;
}


/*!
    \fn nConfig::cConfMySQL::UpdatePK()
 */
bool nConfig::cConfMySQL::UpdatePK()
{
	return UpdatePK(mQuery);
}

bool nConfig::cConfMySQL::UpdatePK(cQuery &Query)
{
	UpdateFields(Query.OStream());
	WherePKey(Query.OStream());
	bool ret = Query.Query();
	Query.Clear();
	return ret;
}


/*!
    \fn nConfig::cConfMySQL::UpdatePKVar(const char *field)
 */
bool nConfig::cConfMySQL::UpdatePKVar(const char *field)
{
	cConfigItemBase *item = operator[](field);
	if(!item) return false;
	return UpdatePKVar(item);
}

void nConfig::cConfMySQL::WriteStringConstant(ostream &os, const string &str)
{
	string tmp;

	// replace all \ by "\\"
	// replace all " by \"
	size_t pos = 0, lastpos = 0;
	char c;

	while (
		str.npos != lastpos &&
		str.npos != ( pos = str.find_first_of("\\\"'`",lastpos)))
	{
		tmp.append(str, lastpos , pos - lastpos );
		tmp.append("\\");
		c = str[pos];
		tmp.append(&c,1);
		lastpos = pos + 1;
	}

	if( str.npos != lastpos) tmp.append(str, lastpos , pos - lastpos );
	os << tmp;
}

void nConfig::cConfMySQL::ufEqual::operator()(cConfigItemBase* item)
{
	if(!start) mOS << mJoint; else start=false;
	if(mDoField) mOS << (item->mName);
	if(mDoValue) {
		bool IsNull = false;
		tItemType TypeId =  item->GetTypeID();
		IsNull = item->IsEmpty() && ((TypeId == eIT_TIMET) || (TypeId == eIT_LONG));

		if(mDoField)
		{
			if(IsNull && !mIsAffect) mOS << " IS ";
			else mOS << " = ";
		}

		if(IsNull) mOS << "NULL ";
		else item->WriteToStream(mOS);
	}
}


