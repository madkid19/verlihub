/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cdbconf.h"

namespace nDirectConnect {

cDBConf::cDBConf(const string &file):cConfigFile(file,false)
{
	msLogLevel = 1;
	Add("db_host",db_host,string("localhost"));
	Add("db_user",db_user,string("verlihub"));
	Add("db_pass",db_pass,string(""));
	Add("db_data",db_data,string("verlihub"));
	Add("config_name",config_name,string("config"));
	Add("lang_name",lang_name,string("lang_en"));
	Add("allow_exec",allow_exec, false);
	Add("allow_exec_mod",allow_exec_mod, true);
	Add("db_charset",db_charset,string("utf8"));
	Load();
}


cDBConf::~cDBConf()
{
}


};
