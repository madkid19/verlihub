/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CDIACFG_H
#define CDIACFG_H

#include <string>
#include <cconfigbase.h>

using namespace std;
using namespace nConfig;

namespace nDirectConnect {
class cServerDC;
};

using namespace nDirectConnect;

class cDiaCfg : public cConfigBase
{
public: 
	cDiaCfg(cServerDC *);
	string replace_set;
	string find_set;
	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};

#endif
