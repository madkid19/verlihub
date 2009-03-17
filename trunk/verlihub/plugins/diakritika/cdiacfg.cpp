/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <cserverdc.h>
#include "cdiacfg.h"

cDiaCfg::cDiaCfg(cServerDC *s) : mS(s)
{
	Add("replace_from",find_set,string(""));
	Add("replace_to", replace_set, string(""));
}

int cDiaCfg::Load()
{
	mS->mSetupList.LoadFileTo(this,"pi_diakritika");
	return 0;
}

int cDiaCfg::Save()
{
	mS->mSetupList.SaveFileTo(this,"pi_diakritika");
	return 0;
}

