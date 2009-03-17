/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CCONFIGITEMBASE_H
#define CCONFIGITEMBASE_H

#include <string>
#include <iostream>

using namespace std;
#ifndef _WIN32
#define __int64 long long
#endif

namespace nConfig
{

typedef enum
{
	eIT_VOID, // unspecified
	eIT_BOOL,
	eIT_CHAR,
	eIT_INT,
	eIT_UINT, // unsigned int
	eIT_LONG,
	eIT_ULONG,
	eIT_PCHAR, // null terminated string
	eIT_TIMET,
	eIT_LLONG, // long long
	eIT_ULLONG,
	eIT_AR_BYTE, // array of bytes
	eIT_STRING,  // std::string
	eIT_DOUBLE
}tItemType;

/**
a base class for all kinds of config items, used by cConfigBase

@author Daniel Muller
*/
class cConfigItemBase
{
	friend class mConfigBase;
public:
	/** constructors */
	cConfigItemBase(void *addr=0, void *base=0) :
		mAddr(addr),
		mBase(base)
	{};

	virtual ~cConfigItemBase(){};

	/** setting value */
	template <class TYPE> cConfigItemBase &operator=(const TYPE &i){*(TYPE*)Address() = i; return *this;};
	/** cast operator */
	template <class TYPE> operator TYPE(){return *(TYPE*)mAddr;}

	/** input /output */
	virtual std::istream &ReadFromStream(std::istream &)=0;
	virtual std::ostream &WriteToStream (std::ostream &)=0;
	friend std::istream &operator >> (std::istream &is, cConfigItemBase &ci){return ci.ReadFromStream(is);}
	friend std::ostream &operator << (std::ostream &os, cConfigItemBase &ci){return ci.WriteToStream (os);}

	/** conversion of a string to a value */
	virtual void ConvertFrom(const std::string &) = 0;
	virtual void ConvertTo(std::string &) = 0;
	virtual bool IsEmpty() = 0;
	virtual tItemType GetTypeID() = 0;
	void *mAddr;
	string mName;
	char mBuf[32];
public:
	virtual void *Address(){return mAddr;}
	void *mBase;
	
};

/**
  * a Typed template for a config Item, provides all the methods for convetiong with string, affecting values, ans streaming
*/

#define DeclarecConfigItemBaseT(TYPE,TypeID,Suffix) \
class cConfigItemBase##Suffix : public cConfigItemBase { \
public: \
	cConfigItemBase##Suffix(TYPE &var): cConfigItemBase((void*)&var, 0){}; \
	cConfigItemBase##Suffix(TYPE &var, string const &Name): cConfigItemBase((void*)&var, 0){this->mName=Name;}; \
	virtual ~cConfigItemBase##Suffix(){}; \
	virtual TYPE & Data(){return *(TYPE*)mAddr;}; \
	virtual cConfigItemBase##Suffix &operator=(TYPE const &i){*(TYPE*)Address() = i; return *this;};\
	virtual tItemType GetTypeID(){ return TypeID;} \
	virtual bool IsEmpty();\
	virtual std::istream &ReadFromStream(std::istream& is); \
	virtual std::ostream &WriteToStream (std::ostream& os); \
	virtual void ConvertFrom(const std::string &str); \
	virtual void ConvertTo(std::string &str); \
};

DeclarecConfigItemBaseT(bool,eIT_BOOL,Bool);
DeclarecConfigItemBaseT(char,eIT_CHAR,Char);
DeclarecConfigItemBaseT(int,eIT_INT,Int);
DeclarecConfigItemBaseT(unsigned,eIT_UINT,UInt);
DeclarecConfigItemBaseT(long,eIT_LONG,Long);
DeclarecConfigItemBaseT(unsigned long,eIT_ULONG, ULong);
DeclarecConfigItemBaseT(double,eIT_DOUBLE,Double);
DeclarecConfigItemBaseT(char*,eIT_PCHAR,PChar);
DeclarecConfigItemBaseT(string,eIT_STRING,String);
DeclarecConfigItemBaseT(__int64,eIT_LLONG,Int64);

};


#endif
