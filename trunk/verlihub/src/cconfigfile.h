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
#ifndef CCONFIGFILE_H
#define CCONFIGFILE_H

#include "cconfigbase.h"
#include <string>


using namespace std;
namespace nConfig
{

/**configuration lodable from a file
  *@author Daniel Muller
  */

class cConfigFile : public cConfigBase //<sBasicItemCreator>
{
public:
	cConfigFile(const string &file, bool load=true);
	/** The config load function - from a file */
	int Load();
	~cConfigFile();
	/** save config, to be able to load it after */
	int Save();
	int Save(ostream &);
protected: // Protected attributes
	/** filename */
	string mFile;

public:
/*	class cConfigItemFile : public cConfigItemBase
	{
	public:
		cConfigItemFile(void *a=NULL, tItemType t=eIT_VOID, int s=0): cConfigItem(a,t,s){};
		cConfigItemFile(const cConfigItem &ci):cConfigItem(ci){}
		friend istream & operator >> (istream &, cConfigItem &);
		friend ostream & operator << (ostream &, cConfigItem &);
	};*/
};
};
#endif
