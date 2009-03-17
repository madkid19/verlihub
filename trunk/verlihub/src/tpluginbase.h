/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NPLUGINTPLUGINBASE_H
#define NPLUGINTPLUGINBASE_H

#include <string>
#include "cobj.h"

using std::string;

namespace nPlugin {

/**
the plugin base class suitable for any application

@author Daniel Muller
*/
class tPluginBase : public cObj
{
public:
	tPluginBase();
	~tPluginBase();
	bool Open();
	bool Close();
	string Error();
	int StrLog(ostream & ostr, int level);
protected:
	string mFileName;
	string mName;
	void *mHandle;

};

};

#endif
