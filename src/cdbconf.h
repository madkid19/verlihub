/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NCONFIGCDBCONF_H
#define NCONFIGCDBCONF_H

#include "cconfigfile.h"

using nConfig::cConfigFile;

namespace nDirectConnect {

/**
config of the database

@author Daniel Muller
*/
class cDBConf : public cConfigFile
{
public:
	cDBConf(const string &);

	~cDBConf();
	string db_host;
	string db_user;
	string db_pass;
	string db_data;
	string config_name;
	string lang_name;
	string db_charset;
	bool allow_exec;
	bool allow_exec_mod;
	
};

};

#endif
