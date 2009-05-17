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

#ifndef NUTILSTHASHARRAY_H
#define NUTILSTHASHARRAY_H

#include <string.h>
#include "cobj.h"

namespace nUtils {

/**
resizable hash array with non-persistent hashes

@author Daniel Muller
*/
template <class DataType> class tUniqueHashArray : public cObj
{
public:
	tUniqueHashArray(unsigned init_size=1024);
	virtual ~tUniqueHashArray();
	/** calculate hash fot given string, return the hash */
	unsigned ShortHashString(const string &);
	unsigned ShortHashPointer(const void *);
	/** returns zero on success, else returns data that has given hash */
	virtual DataType Insert(DataType const Data, unsigned hash);
	/** return the data that has previously been there */
	virtual DataType Update(DataType const Data, unsigned hash);
	/** returns zero on success, else returns data that has given hash */
	virtual DataType Get(unsigned hash);
	/** remove data from hash table */
	virtual DataType Remove(unsigned hash);
	/** return the number of elements in the array */
	virtual unsigned Size();
	/** return the reserved capacity */
	virtual unsigned Capacity();
	
	/// iterator thourg non NULL data items in the container
	struct iterator
	{
		iterator() : i(0),end(0),mData(NULL){}
		iterator(DataType *Data,unsigned _i, unsigned _end) : i(_i),end(_end),mData(Data){}
		iterator & operator = (const iterator &it){ mData = it.mData; i = it.i; end=it.end; return *this;}
		iterator(const iterator &it){ (*this) = it;}
		bool operator == (const iterator &it){ return i == it.i;}
		bool operator != (const iterator &it){ return i != it.i;}
		iterator & operator ++(){ while((++i != end) && (mData[i] == (DataType)NULL)){}; return *this;}
		DataType operator *(){ return mData[i]; }
		bool IsEnd(){ return i >= end; }
		unsigned i;
		unsigned end;
		DataType *mData;
	};

	iterator begin()
	{
		iterator Begin(mData, 0,mCapacity);
		if (mData[0] == (DataType)NULL) ++Begin;
		return Begin;
	}

	iterator end()
	{
		return iterator(mData, mCapacity , mCapacity);
	}

	unsigned   Key2ShortHash(const string &Key){ return ShortHashString(Key); }

	/*
	bool     AddWithHash(DataType Data, const unsigned &Hash)
	{
		bool ret= ((DataType)NULL == Insert(Data, Hash));
		if (ret) OnAdd(Data);
		return ret;
	}
	bool     RemoveByHash(const unsigned &Hash)
	{
		DataType old = Remove(Hash);
		if(old != (DataType)NULL)
		{
			OnRemove(old);
			return true;
		}
		else return false;
	}
	*/
	bool     ContainsHash(const unsigned &Hash){ return (DataType)NULL != Get(Hash);}
	DataType GetByHash(const unsigned &Hash){return Get(Hash);}
	unsigned size(){ return mSize; }

	//virtual void OnAdd(DataType){};
	//virtual void OnRemove(DataType){};

	DataType *mData;
	unsigned mSize;
	unsigned mCapacity;
};

/**
	A quick hash array. A container wher you can bind data to a hash number,
	which can be produced out of a string or you can provide it yourself.
	@author Daniel Muller
*/
template <class DataType> class tHashArray : public cObj
{
public:
	typedef unsigned long tHashType;
	class iterator;
private:
	
	/***/
	struct sItem
	{
		friend struct iterator;
		DataType mData;
		tHashType mHash;
	private:
		sItem *mNext;
	public:

		sItem(DataType Data= (DataType)NULL, tHashType Hash = 0, sItem *Next = NULL) :
			mData(Data), mHash(Hash), mNext(Next) {}

		~sItem()
		{
			if (mNext != NULL)
			{
				delete mNext;
				mNext =  NULL;
			}
		}

		unsigned size()
		{
			unsigned i = 1;
			sItem *it = mNext;
			while (it != NULL ) {
				it = it->mNext;
				i ++;
			}
			return i;
		}
		
		DataType Find(tHashType Hash)
		{
			if (mHash == Hash) return mData;
			sItem *it = mNext;
			while ((it != NULL ) && (it->mHash != Hash)) it = it->mNext;
			if (it != NULL) return it->mData;
			else return (DataType)NULL;
		}

		DataType Set(tHashType Hash, const DataType &value)
		{
			DataType _result = NULL;
			if (mHash == Hash)
			{
				_result = mData;
				mData = value;
				return _result;
			}
			sItem *it = mNext;
			while ((it != NULL ) && (it->mHash != Hash)) it = it->mNext;
			if (it != NULL)
			{
				_result = it->mData;
				it->mData = value;
				return _result;
			}
			else return _result;
		}

		DataType AddData(DataType Data, tHashType Hash)
		{
			if (mHash == Hash) return mData;
			sItem *it = mNext;
			sItem *prev = this;

			while ((it != NULL ) && (it->mHash != Hash))
			{
				prev = it;
				it = it->mNext;
			}

			if (it != NULL)
			{
				return it->mData;
			}
			prev->mNext = new sItem(Data , Hash);
			return (DataType)NULL;
		}

		DataType DeleteHash(tHashType Hash, sItem *&Start)
		{
			DataType Data = (DataType)NULL;
			if (mHash == Hash)
			{
				Start = mNext;
				mNext = NULL;
				Data = mData;
				return Data;
			}
			sItem *it = mNext;
			sItem *prev = this;

			while ((it != NULL ) && (it->mHash != Hash))
			{
				prev = it;
				it = it->mNext;
			}
			if (it != NULL)
			{
				Data = it->mData;
				prev->mNext = it->mNext;
				it->mNext = NULL;
				delete it;
			}
			return Data;
		}
 	};
public:
	tHashArray(unsigned init_size=1024) { mData = new tData(init_size); mSize = 0; mIsResizing = false;}

	~tHashArray()
	{
		Clear();
		delete mData;
		mData = NULL;
	}

	void Clear()
	{
		sItem *Item = NULL;
 		for (unsigned it = 0; it < mData->Capacity(); it ++)
 		{
 			Item = mData->Get(it);
 			if ( Item != NULL) { delete Item; }
 			mData->Update(NULL, it);
 		}
	}

	static tHashType HashString(const string &);
	static tHashType HashStringLower(const string &);
	tHashType Key2Hash(const string &Key){ return HashString(Key); }

	bool     AddWithHash(DataType Data, const tHashType &Hash)
	{
		if (Data == (DataType)NULL) return false;
		unsigned HashShort = Hash % mData->mCapacity ;
		sItem * Items, *Item = NULL;
		Items = mData->Get(HashShort);
		if (Items == NULL)
		{
			Item = new sItem(Data, Hash);
			mData->Insert(Item, HashShort);
			if(! mIsResizing)
			{
				OnAdd(Data);
				mSize++;
			}
			return true;
		}

		if ( (DataType)NULL == Items->AddData(Data, Hash))
		{
			if(! mIsResizing)
			{
				OnAdd(Data);
				mSize ++;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool     RemoveByHash(const tHashType &Hash)
	{
		unsigned HashShort = Hash % mData->mCapacity ;
		sItem * Items, *Item = NULL;
		Items = mData->Get(HashShort);
		if (Items == NULL){ return false; }

		Item = Items;
		DataType Data = Items->DeleteHash(Hash, Item);
		if(Item != Items)
		{
			mData->Update(Item, HashShort);
			delete Items;
			Items = NULL;
		}

		if ( (DataType)NULL != Data )
		{
			OnRemove(Data);
			mSize --;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool     ContainsHash(const tHashType &Hash)
	{
		unsigned HashShort = Hash % mData->mCapacity ;
		sItem * Items = mData->Get(HashShort);
		if (Items == NULL) return false;
		return ((DataType)NULL != Items->Find(Hash));
	}

	DataType GetByHash(const tHashType &Hash)
	{
		unsigned HashShort = Hash % mData->mCapacity ;
		sItem * Items = mData->Get(HashShort);
		if (Items == NULL) return (DataType)NULL;
		return Items->Find(Hash);
	}

	bool SetByHash(const tHashType &Hash, const DataType &value)
	{
		unsigned HashShort = Hash % mData->mCapacity ;
		sItem * Items = mData->Get(HashShort);
		if (Items == NULL) return false;
		return Items->Set(Hash, value);
	}

	unsigned size(){ return mSize; }

	virtual void OnAdd(DataType){};
	virtual void OnRemove(DataType){};

	typedef tUniqueHashArray<sItem *> tData;

	/**
		Iterator through all NON-NULL elements of the container
	*/
	class iterator
	{
	public:
		iterator():mItem(NULL){}
		iterator(typename tData::iterator it) : i(it){ if(!i.IsEnd()) mItem = *i; else mItem = NULL;}
		iterator & operator = (const iterator &it){ mItem = it.mItem; i = it.i; return *this;}
		iterator(const iterator &it){ (*this) = it;}

		bool operator == (const iterator &it){ return mItem == it.mItem;}
		bool operator != (const iterator &it){ return mItem != it.mItem;}

		iterator & operator ++()
		{
			if (mItem->mNext != NULL)
			{
				mItem = mItem->mNext;
			} else
			{
				++i;
				if (!i.IsEnd()) mItem = (*i);
				else mItem = NULL;
			}
			return *this;
		}

		bool IsEnd(){ return mItem == NULL;}

		DataType operator *(){ return mItem->mData; }

		typename tData::iterator i;
		sItem * mItem;
	};

	iterator begin(){ return iterator(mData->begin());}
	iterator end() { return iterator(); }

	void AutoResize()
	{
		if (
				( this->mSize > this->mData->mCapacity * 2  ) ||
				( (this->mSize * 2 + 1)< this->mData->mCapacity )
			)
		{
			if (Log(3)) LogStream() << "Autoresizing capacity: " <<
				this->mData->mCapacity << " size: " << this->mSize <<
				" >> " << (this->mSize + this->mSize/2 + 1) << endl;
			this->Resize(this->mSize + this->mSize/2 + 1);
		}
	}

	int Resize(int NewSize)
	{
		tData *NewData = new tData(NewSize);
		tData *OldData = this->mData;
		// copy and recalculate all old data
		iterator it;
		it = this->begin();
		mIsResizing = true;
		this->mData = NewData;
		while (!it.IsEnd())
		{
			this->AddWithHash(it.mItem->mData, it.mItem->mHash);
			++it;
		}
		// delete old data array
		delete OldData;
		mIsResizing = false;
		return 0;
	}

	void DumpProfile(ostream &os)
	{
			os << "Size = " << mSize << " Capacity = " << mData->mCapacity << endl;
			unsigned cumulative = 0;
			for(unsigned i=0;i < mData->mCapacity; ++i) {
					if (mData->mData[i] != NULL) {
						cumulative = 0;
						os << "i = " << i << " nulls = " << cumulative << " count = " << 
								((sItem*)mData->mData[i])->size() << endl;
					} else {
						cumulative ++;
					}
			}				
	}
protected:
	tData * mData;
	unsigned mSize;
	bool mIsResizing;
};


template <class DataType> tUniqueHashArray<DataType>::tUniqueHashArray(unsigned size) : mCapacity(size)
{
	mSize = 0;
	mData = new DataType[mCapacity];
	memset(mData, 0, sizeof(DataType)*mCapacity);
}

template <class DataType> tUniqueHashArray<DataType>::~tUniqueHashArray()
{
	if (mData) delete [] mData;
	mData = NULL;
}

template <class DataType> DataType tUniqueHashArray<DataType>::Insert(DataType const Data, unsigned hash)
{
	if (hash > mCapacity) hash %= mCapacity;
	DataType OldData = mData[hash];
	if (!OldData)
	{
		mData[hash] = Data;
		if( Data ) mSize ++;
	}
	return OldData;
}

/** return the data that has previously been there */
template <class DataType> DataType tUniqueHashArray<DataType>::Update(DataType const Data, unsigned hash)
{
	if (hash > mCapacity) hash %= mCapacity;
	DataType OldData = mData[hash];
	mData[hash] = Data;
	if (!OldData && Data) mSize ++;
	else if (OldData && !Data) mSize --;
	return OldData;
}

/** returns zero on success, else returns data that has given hash */
template <class DataType> DataType tUniqueHashArray<DataType>::Get(unsigned hash)
{
	if (hash > mCapacity) hash %= mCapacity;
	return mData[hash];
}

/** remove data from hash table */
template <class DataType> DataType tUniqueHashArray<DataType>::Remove(unsigned hash)
{
	if (hash > mCapacity) hash %= mCapacity;
	DataType OldData = mData[hash];
	mData[hash] = 0;
	if (OldData) mSize --;
	return OldData;
}

template <class DataType> unsigned tUniqueHashArray<DataType>::ShortHashString(const string &str)
{
	unsigned __h = 0;
	const char *__s = str.c_str();
	for ( ; *__s; ++__s)
	__h = 33*__h + *__s;

	return __h % mCapacity;
}

template <class DataType> typename tHashArray<DataType>::tHashType tHashArray<DataType>::HashString(const string &str)
{
	tHashType __h = 0;
	const char *__s = str.c_str();
	for ( ; *__s; ++__s)
	__h = 33*__h + *__s;

	return __h;
}

template <class DataType> typename tHashArray<DataType>::tHashType tHashArray<DataType>::HashStringLower(const string &str)
{
	tHashType __h = 0;
	const char *__s = str.c_str();
	for ( ; *__s; ++__s)
	__h = 33*__h + ::tolower(*__s);

	return __h;
}

template <class DataType> unsigned tUniqueHashArray<DataType>::ShortHashPointer(const void *)
{
	return 0;
}

/** return the number of elements in the array */
template <class DataType> unsigned tUniqueHashArray<DataType>::Size()
{
	return mSize;
}

/** return the reserved capacity */
template <class DataType> unsigned tUniqueHashArray<DataType>::Capacity()
{
	return mCapacity;
}

};
#endif
