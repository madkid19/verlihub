/***************************************************************************
                          cconfigfile.h  -  description
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Daniel Muller
    email                : dan at verliba dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
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
