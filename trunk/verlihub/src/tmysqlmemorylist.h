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
#ifndef TMYSQLMEMORYLIST_H
#define TMYSQLMEMORYLIST_H
#include <vector>
#include "cconfmysql.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "stringutils.h"
#include <dirsettings.h>

using std::vector;
using nConfig::cConfMySQL;
using namespace nStringUtils;

namespace nConfig {

/**
@author Daniel Muller
*/
template<class DataType, class OwnerType> class tMySQLMemoryList : public cConfMySQL
{
public:

	tMySQLMemoryList(cMySQL &mysql, OwnerType *owner, string tablename) :
		cConfMySQL(mysql), mOwner(owner)
	{
		mMySQLTable.mName = tablename;
	}

	virtual ~tMySQLMemoryList(){ this->Empty();}

	virtual void OnStart()
	{
		AddFields();
		SetBaseTo(&mModel);
		CreateTable();
		InstallDefaultData();
		ReloadAll();
	}

	typedef vector<DataType *> tMyDataType;
	typedef typename tMyDataType::iterator iterator;

	virtual void OnLoadData(DataType &Data)
	{
		Data.OnLoad();
	}
	
	/**
	 * \brief add all field you want in this function
	 * */
	virtual void AddFields()=0;

	/**
	 * \brief empty what is in the memory storage
	 * */
	virtual void Empty() {
		for (typename tMyDataType::iterator it = mData.begin(); it != mData.end(); ++it) {
			if (*it != NULL) {
				delete *it;
				*it = NULL;
			}
		}
		mData.clear();
	}

	/**
	 * \return the number of elements in the list loaded in memory
	 * */
	virtual int Size(){ return mData.size();}

	/**
	 * \brief Loads all data from the database into memory
	 * \return the number of elements loaded
	 * */
	virtual int ReloadAll()
	{
		cQuery Query(mQuery); // make a second query for safety reasons
		Empty();
		Query.Clear();
		SelectFields(Query.OStream());

		if(this->mWhereString.size())
			Query.OStream() << " WHERE " << this->mWhereString;
		if(this->mOrderString.size()) 
			Query.OStream() << " ORDER BY " << this->mOrderString;
		int n=0;
		db_iterator it;

		DataType CurData, *AddedData;
		SetBaseTo(&CurData);

		for(it = db_begin(Query); it != db_end(); ++it)
		{
			AddedData = this->AppendData(CurData);
			OnLoadData(*AddedData);
			n++;
		}
		Query.Clear();
		return n;
	}

	virtual DataType *AppendData(DataType const &data)
	{
		DataType *copy = new DataType(data);
		mData.push_back(copy);
		return copy;
	}
	/**
	 * \brief Add data into memory and database too, if you add twice same, it may be duplicated
	 * */
	virtual DataType *AddData(DataType const &data)
	{
		DataType *copy = this->AppendData(data);
		SetBaseTo(copy);
		SavePK();
		return copy;
	}

	virtual int UpdateData(DataType &data)
	{
		SetBaseTo(&data);
		return UpdatePK();
	}

	/**
	 * \brief Removes data from database, given probably by the PK
	 *
	 * */
	virtual void DelData(DataType &data)
	{
		SetBaseTo(&data);
		DeletePK();
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			DataType* CurrentData =  *it;
			if((CurrentData != NULL) && CompareDataKey(data, *CurrentData))
			{
				delete CurrentData;
				*it = NULL;
				mData.erase(it);
				break;
			}
		}
	}

	virtual bool CompareDataKey(const DataType &D1, const DataType &D2){ return false; }

	virtual DataType *FindData(DataType &ByKey)
	{
		iterator it;
		for (it = begin(); it != end(); ++it)
		{
			if( CompareDataKey(ByKey, *(*it))) return *it;
		}
		return NULL;
	}

	
	void SetSelectOrder(const string &Order)
	{
		this->mOrderString = Order;
	}

	void SetSelectWhere(const string &Order)
	{
		this->mWhereString = Order;
	}
	
	/**
	 * \brief save data on n-th position
	 * */
	void SaveData(size_t n)
	{
		if (n < mData.size())
		{
			SetBaseTo(mData[n]);
			Save();
		}
	}

	/**
	 * \brief load default data script from a file
	 * */
	bool InstallDefaultData()
	{
		mQuery.Clear();
		string buf, filename;
		#ifdef _WIN32
		filename =  ".\\sql\\default_" + mMySQLTable.mName + ".sql";
		#else
		filename =  DATADIR  "/sql/default_" + mMySQLTable.mName + ".sql";
		#endif
		bool _Result = false;
		if(LoadFileInString(filename, buf))
		{
			mQuery.OStream() << buf;
			_Result = mQuery.Query();
			mQuery.Clear();
		}
		return _Result;
	}
	
	/**
	 * \brief Get the i-th element from the list
	 * */
	DataType * operator[](int i)
	{
		if( i < 0 || i >= Size() ) return NULL;
		return mData[i];
	}

	iterator begin() { return mData.begin();}
	iterator end() { return mData.end();}

private:
	tMyDataType mData;

protected:
	string mOrderString;
	string mWhereString;

	/**
	 * The prototype of given data
	 * */
	DataType mModel;

	/**
	 * The owner object of this one
	 * */
	OwnerType *mOwner;
};

};

#endif
