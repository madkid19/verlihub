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

#include "cconfigbase.h"

namespace nConfig
{
hHashStr<cConfigBaseBase::tItemHashType> cConfigBaseBase::msHasher;

cConfigBaseBase::cConfigBaseBase() : cObj("cConfigBase")
{
	mBasePtr= NULL;
	mItemCreator = new cBasicItemCreator;
}

cConfigBaseBase::~cConfigBaseBase()
{
	tItemHashType Hash;
	tItemVec::iterator it;
	cConfigItemBase *item;
	
	for(it = mvItems.begin(); it != mvItems.end(); ++it)
	{
		Hash = *it;
		item = mhItems.GetByHash(Hash);
		mhItems.RemoveByHash(Hash);
		this->mItemCreator->DeleteItem(item);
	}
	if (mItemCreator != NULL) delete mItemCreator;
	mItemCreator = NULL;
}

/** add existing item pointed by the argument at the end of mvItems , and bind a nick to it*/
cConfigItemBase * cConfigBaseBase::Add(const string &nick, cConfigItemBase *ci)
{

	tItemHashType Hash = msHasher(nick);
	if(!mhItems.AddWithHash(ci, Hash))
	{
		if(Log(1)) {
			cConfigItemBase *other = mhItems.GetByHash(Hash);
			LogStream() << "Error adding " << nick <<
				" because of " << (other?other->mName.c_str():"NULL") << "\r\n";
		}
	}
	mvItems.push_back(Hash);
	ci->mName = nick;
	return ci;
}

/** access operators */
cConfigItemBase * cConfigBaseBase::operator[](int i)
{
	return mhItems.GetByHash(mvItems[i]);
}

cConfigItemBase * cConfigBaseBase::operator[](const string &n)
{
	tItemHashType Hash = msHasher(n);
	return mhItems.GetByHash(Hash);
}

void cConfigBaseBase::SetBaseTo(void * new_base)
{
	if(mBasePtr)
	{
		for(tIVIt it = mvItems.begin(); it != mvItems.end(); it++ )
			mhItems.GetByHash(*it)->mAddr =
				(void*)(long(mhItems.GetByHash(*it)->mAddr) +
				(long(new_base)-long(mBasePtr)));
	}
	mBasePtr = new_base;
}


/** create and add an item of template type with nick too */
#define DefineAddMethodWithDefValue(TYPE) \
cConfigItemBase * cConfigBase::Add(const string &name, TYPE &var, TYPE const &def) \
{ \
	cConfigItemBase * ci = this->Add(name, var); \
	*ci=def; \
	return ci; \
}

#define DefineAddMethodWithoutDefValue(TYPE) \
cConfigItemBase * cConfigBase::Add(const string &name, TYPE &var) \
{ \
	cConfigItemBase * ci= this->mItemCreator->NewItem(var); \
	return this->cConfigBaseBase::Add(name, ci); \
}
#define DefineAddMethods(TYPE) \
DefineAddMethodWithoutDefValue(TYPE) \
DefineAddMethodWithDefValue(TYPE)

DefineAddMethods(bool);
DefineAddMethods(char);
DefineAddMethods(int);
DefineAddMethods(unsigned);
DefineAddMethods(long);
DefineAddMethods(unsigned long);
DefineAddMethods(__int64);
DefineAddMethods(string);
DefineAddMethods(char *);
DefineAddMethods(double);

};

