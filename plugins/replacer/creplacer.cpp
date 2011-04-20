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

#include "creplacer.h"
#include "src/cconfigitembase.h"
#include "src/cserverdc.h"

namespace  {
namespace nTables {

cReplacer::cReplacer( cServerDC *server )
 : cConfMySQL(server->mMySQL) , mS(server)
{
	SetClassName("nDC::cReplacer");
	mMySQLTable.mName = "pi_replacer";
	Add("word",mModel.mWord);
	AddPrimaryKey("word");
	Add("rep_word",mModel.mRepWord);
	Add("afclass",mModel.mAfClass);
	SetBaseTo(&mModel);
}

cReplacer::~cReplacer()
{
}

void cReplacer::CreateTable(void)
{
	cQuery query(mMySQL);
	query.OStream() <<
		"CREATE TABLE IF NOT EXISTS " << mMySQLTable.mName << " ("
		"word varchar(30) not null primary key,"
		"rep_word varchar(30) not null,"
		"afclass tinyint default 4"
		")";
	query.Query();
}

void cReplacer::Empty()
{
	mData.clear();
}

int cReplacer::LoadAll()
{
	Empty();
	SelectFields(mQuery.OStream());
	int n=0;
	db_iterator it;

	PrepareNew();
	for(it = db_begin(); it != db_end(); ++it) {
		n++;
		if(Log(3))
			LogStream() << "Loading :" << mData.back()->mWord << endl;
		if(Log(3))
			LogStream() << "Loading :" << mData.back()->mRepWord << endl;
		if(!mData.back()->PrepareRegex()) {
			if(Log(3))
				LogStream() << "Error in regex :" << mData.back()->mWord << endl;
		} else
			PrepareNew();
	}
	mQuery.Clear();
	DeleteLast();
	return n;
}

void cReplacer::DeleteLast()
{
	if(!mData.size())
		return;
	SetBaseTo(&mModel);
	delete mData.back();
	mData.pop_back();
}

void cReplacer::PrepareNew()
{
	cReplacerWorker *tr = new cReplacerWorker;
	SetBaseTo(tr);
	mData.push_back(tr);
}

cReplacerWorker * cReplacer::operator[](int i)
{
	if(i < 0 || i >= Size())
		return NULL;
	return mData[i];
}

int cReplacer::AddReplacer(cReplacerWorker &fw)
{
	SetBaseTo(&fw);
	return SavePK();
}

void cReplacer::DelReplacer(cReplacerWorker &fw)
{
	SetBaseTo(&fw);
	DeletePK();
}

string cReplacer::ReplacerParser(const string & str, cConnDC * conn)
{
	string lcstr(str);
	string::size_type idx;
	string t_word;
	string r_word;
	string temp(str);
	bool find_loop;

	transform(lcstr.begin(), lcstr.end(), lcstr.begin(), ::tolower);

	for(tDataType::iterator it = mData.begin(); it != mData.end(); ++it) {
		if((*it)->CheckMsg(lcstr)) {
			if((*it)->mAfClass >= conn->mpUser->mClass) {
				t_word = (*it)->mWord;
				r_word = (*it)->mRepWord;
				find_loop = true;
				while(find_loop) {
					idx = temp.find(t_word.data());
					if(idx != string::npos)
						temp.replace(idx,t_word.length(),r_word.data(),r_word.length());
					else
						find_loop = false;
				}
			}
		}
	}
	return temp;
}

cReplaceCfg::cReplaceCfg(cServerDC *s) : mS(s)
{
}

int cReplaceCfg::Load()
{
	mS->mSetupList.LoadFileTo(this,"pi_replacer");
	return 0;
}

int cReplaceCfg::Save()
{
	mS->mSetupList.SaveFileTo(this,"pi_replacer");
	return 0;
}

};
};
