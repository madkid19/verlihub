/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "csetuplist.h"
#include "cdcproto.h"

using namespace ::nDirectConnect::nProtocol;
namespace nDirectConnect {

namespace nTables  {

cSetupList::cSetupList(cMySQL &mysql):cConfMySQL(mysql)
{
	mMySQLTable.mName = "SetupList";
	AddCol("file", "varchar(15)", "" , false, mModel.mFile);
	AddPrimaryKey("file");
	AddCol("var", "varchar(32)", "" , false, mModel.mVarName);
	AddPrimaryKey("var");
	AddCol("val", "text", "" , true, mModel.mVarValue);
	mMySQLTable.mExtra = "PRIMARY KEY (file, var)";
	SetBaseTo(&mModel);
}

cSetupList::~cSetupList()
{
}


};

};


/*!
    \fn nDirectConnect::nTables::cSetupList::LoadFileTo(cConfigBase &, const char*)
 */
void nDirectConnect::nTables::cSetupList::LoadFileTo(cConfigBaseBase *Config, const char*file)
{
	db_iterator it;
	cConfigItemBase *item = NULL;
	SelectFields(mQuery.OStream());
	mQuery.OStream() << " WHERE file='" << file << "'";

	for(it = db_begin(); it != db_end(); ++it)
	{
		item = (*Config)[mModel.mVarName];
		if (item) item->ConvertFrom(mModel.mVarValue);
	}
	mQuery.Clear();
}

void nDirectConnect::nTables::cSetupList::OutputFile(const char*file, ostream &os)
{
	const int width = 5;
	db_iterator it;
	SelectFields(mQuery.OStream());
	mQuery.OStream() << " WHERE file='" << file << "'";
	string val;

	for(it = db_begin(); it != db_end(); ++it)
	{
		cDCProto::EscapeChars(mModel.mVarValue, val);
		os << "\r[::]  " << setw(width) << setiosflags(ios::left) << mModel.mVarName << setiosflags(ios::right) <<"    =   " << val << "\r\n";
	}
	mQuery.Clear();
}
/*!
    \fn nDirectConnect::nTables::cSetupList::SaveFileTo(cConfigBase *, const char*)
 */
void nDirectConnect::nTables::cSetupList::SaveFileTo(cConfigBaseBase *Config, const char*file)
{
	cConfigBaseBase::iterator it;
	mModel.mFile = file;
	SetBaseTo(&mModel);
	for(it = Config->begin(); it != Config->end(); ++it)
	{
		mModel.mVarName = (*it)->mName;
		(*it)->ConvertTo(mModel.mVarValue);
		SavePK();
	}
}

/*!
    \fn nDirectConnect::nTables::cSetupList::SaveItem(cConfigItemBase *)
 */
bool nDirectConnect::nTables::cSetupList::SaveItem(const char *InFile, cConfigItemBase *ci)
{
	mModel.mFile = InFile;
	mModel.mVarName = ci->mName;
	ci->ConvertTo(mModel.mVarValue);
	DeletePK();
	SavePK(false);
   return true;
}

/*!
    \fn nDirectConnect::nTables::cSetupList::LoadItem(const char* FromFile, cConfigItemBase *)
 */
bool nDirectConnect::nTables::cSetupList::LoadItem(const char *FromFile, cConfigItemBase *ci)
{
	mModel.mFile = FromFile;
	mModel.mVarName = ci->mName;
	LoadPK();
	ci->ConvertFrom(mModel.mVarValue);
	return true;
}
