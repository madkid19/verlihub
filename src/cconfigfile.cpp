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

#include "cconfigfile.h"
#include <fstream>
#include <sstream>

namespace nConfig
{

/*
 	to be used in a following way..
	* add as many fields as nedded, all binded to some physical locations and refering to the types
	* variable given by the name can be found and loaded using a >> operator (virtual template)
	* variable can be also outputed by the << operator
*/

cConfigFile::cConfigFile(const string &file, bool load): mFile(file)
{
	if(load) Load();
}

cConfigFile::~cConfigFile(){
}

/** The config load function - from a file */
int cConfigFile::Load()
{
	string name;
	string str;
	istringstream *ss;
	cConfigItemBase *ci;
	char ch;

	ifstream is(mFile.c_str());
	if(!is.is_open()) {
		if(ErrLog(1))LogStream() << "Can't open file '" << mFile << "' for reading." << endl;
		return 0;
	}

	while(!is.eof()) {
		ch = ' ';
		is >> name;
		if(name[name.size()-1] != '=') {
			is >> ch >> ws;
			if(ch == ' ')
				break;

		} else {
			ch='=';
			name.assign(name,0,name.size()-1);
		}

		getline(is,str);
		if(ch != '=') break;
		if((ci = operator[](name))) {
			ss = new istringstream(str);
			//ss->str(str);
			ss->seekg(0,istream::beg);
			(*ss) >> *ci;
			delete ss;
		}
		else
			if(ErrLog(3)) LogStream() << "Uknown variable '" << name << "' in file '" << mFile << "', ignoring it" << endl;
	}
	is.close();
	return 0;
}

/** save config, to be able to load it after */
int cConfigFile::Save(ostream &os)
{
	for(tIHIt it = mhItems.begin();it != mhItems.end(); it++)
		os << (*it)->mName << " = " << (*it) << "\r\n";
	return 0;
}

int cConfigFile::Save()
{
	ofstream of(mFile.c_str());
	Save(of);
	of.close();
	return 0;
}

};

