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

#ifndef CCONFIGBASE_H
#define CCONFIGBASE_H

#include <vector>
#include "tchashlistmap.h"
#include "cconfigitembase.h"
#include "cobj.h"

using std::vector;
using namespace nUtils;

namespace nConfig
{


#define NewItemMethod(TYPE,Suffix) virtual cConfigItemBase##Suffix * NewItem(TYPE &var){ return new cConfigItemBase##Suffix(var);};

class cBasicItemCreator
{
public:
	cBasicItemCreator(){};
	NewItemMethod(bool, Bool);
	NewItemMethod(char, Char);
	NewItemMethod(int, Int);
	NewItemMethod(unsigned int, UInt);
	NewItemMethod(long, Long);
	NewItemMethod(__int64, Int64);
	NewItemMethod(unsigned long, ULong);
	NewItemMethod(char *, PChar);
	NewItemMethod(string, String);
	NewItemMethod(double, Double);
	virtual void DeleteItem(cConfigItemBase *item) { delete item; }
};


class cConfigBaseBase;

/**configuration base class
  *@author Daniel Muller
  */
class cConfigBaseBase : public cObj
{
public:
	typedef unsigned tItemHashType;
	cConfigBaseBase();
	virtual ~cConfigBaseBase();
	/** save config, to be able to load it after */
	virtual int Save() = 0;
	/** The config load function - whetre from a file, or from database, or whatever
		return >= 0 on success, otherwise error code
	*/
	virtual int Load() = 0;

	cBasicItemCreator *mItemCreator;
	
	/********** typedefs */
	/** the itemlist type */
	// nicked acess
	//typedef tStringHashMap<cConfigItemBase*> tItemHash;
	// numerated access
	typedef vector<size_t> tItemVec;
	typedef tItemVec::iterator tIVIt;

	typedef tcHashListMap<cConfigItemBase*,unsigned > tItemHash;
	typedef tItemHash::iterator tIHIt;

	// set the base pointer to relative adresses
	void SetBaseTo(void * new_base);

	void *mBasePtr;
	tItemHash mhItems;
	tItemVec  mvItems;
	static hHashStr<tItemHashType> msHasher;

	/**
		Item iterator
	*/
	struct iterator
	{
		iterator(){}
		iterator (class cConfigBaseBase *C,const tIVIt &it):mC(C),mIT(it){}
		cConfigItemBase * operator* () { return mC->mhItems.GetByHash(*mIT);}
		iterator &operator ++(){ mIT++; return *this;}
		iterator(iterator &it){operator=(it);}
		bool operator != (iterator &it){ return mIT != it.mIT;}
		cConfigBaseBase *mC;
		tIVIt mIT;
		iterator &Set(class cConfigBaseBase *C,const tIVIt &it){mC=C;mIT=it; return *this;}
		iterator &operator=(iterator & it){ mIT = it.mIT; mC=it.mC; return *this;}

	};

	iterator mBegin;
	iterator mEnd;
	iterator &begin(){return mBegin.Set(this,mvItems.begin());}
	iterator &end()  {return mEnd.Set(this,mvItems.end());  }
	/** add existing item pointed by the argument at the end of mvItems , and bind a nick to it*/
	cConfigItemBase * Add(const string &, cConfigItemBase *);

	/** bind a nick to a given vaiable */
	void BindNick(int , const string &);


	/** access operators */
	cConfigItemBase * operator[](int);
	cConfigItemBase * operator[](const string &);
};


#define DeclareAddMethods(TYPE) \
cConfigItemBase * Add(const string &name, TYPE &var); \
cConfigItemBase * Add(const string &name, TYPE &var, TYPE const & def);

/**
This is a base class for every configuration like structure.
You can bind real variables with their names. You can get or affect their values. Convert from/to a std::string
read/write into/from a stream
very useful
@author Daniel Muller
*/
class cConfigBase : public cConfigBaseBase
{
public:
	cConfigBase(){};
	virtual ~cConfigBase(){};

	DeclareAddMethods(bool);
	DeclareAddMethods(char);
	DeclareAddMethods(int);
	DeclareAddMethods(unsigned);
	DeclareAddMethods(long);
	DeclareAddMethods(unsigned long);
	DeclareAddMethods(__int64);
	DeclareAddMethods(string);
	DeclareAddMethods(char *);
	DeclareAddMethods(double);
};

};
#endif
