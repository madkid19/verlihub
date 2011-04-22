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
#ifndef NCONFIGTMYSQLMEMORYHASH_H
#define NCONFIGTMYSQLMEMORYHASH_H
#include "tmysqlmemorylist.h"
#include "thasharray.h"

namespace nVerliHub {
	using namespace nUtils;
	namespace nConfig {

/**
a list mirroring the mysql data, loaded in ram, and hashed for searching

@author Daniel Muller
*/
template<class DataType, class OwnerType>
class tMySQLMemoryHash : public tMySQLMemoryList<DataType,OwnerType>
{
protected:
	typedef tHashArray<DataType*> tDataHashArray;
	typedef typename tDataHashArray::tHashType tDataHashType;

	virtual tDataHashType GetHash(DataType & data) = 0;

	tDataHashArray mDataHash;

public:
	tMySQLMemoryHash(nMySQL::cMySQL& mysql, OwnerType* owner, string tablename):
		tMySQLMemoryList<DataType, OwnerType> (mysql, owner, tablename)
	{}

	virtual ~tMySQLMemoryHash() {}

	virtual DataType* AppendData(DataType const& data)
	{
		DataType* pData = tMySQLMemoryList<DataType,OwnerType>::AppendData(data);
		tDataHashType Hash = this->GetHash(*pData);
		mDataHash.AddWithHash(pData, Hash);
		return pData;
	}

	virtual void DelData(DataType& data)
	{
		tDataHashType Hash = this->GetHash(data);
		tMySQLMemoryList<DataType,OwnerType>::DelData(data);
		mDataHash.RemoveByHash(Hash);
	}

	virtual void Empty()
	{
		tMySQLMemoryList<DataType,OwnerType>::Empty();
		mDataHash.Clear();
	}

};

	}; // namespace nConfig
}; // namespace nVerliHub

#endif
