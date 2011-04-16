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

	for(it = db_begin(); it != db_end(); ++it) {
		item = (*Config)[mModel.mVarName];
		if (item) item->ConvertFrom(mModel.mVarValue);
	}
	mQuery.Clear();
}

void nDirectConnect::nTables::cSetupList::OutputFile(const string &file, ostream &os)
{
	const int width = 5;
	db_iterator it;
	SelectFields(mQuery.OStream());
	if(file == "plugins")
		mQuery.OStream() << " WHERE file LIKE 'pi_%'";
	else
		mQuery.OStream() << " WHERE file='" << file << "'";

	mQuery.OStream() << " ORDER BY `var` ASC";
	string val;

	for(it = db_begin(); it != db_end(); ++it) {
		cDCProto::EscapeChars(mModel.mVarValue, val);
		string varName = mModel.mVarName;
		if(file == "plugins")
			varName = mModel.mFile + "." + varName;

		os << " " << setw(35) << setiosflags(ios::left) << varName << val << "\n";
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
	for(it = Config->begin(); it != Config->end(); ++it) {
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
