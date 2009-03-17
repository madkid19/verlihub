/***************************************************************************
                          ckick.cpp  -  description
                             -------------------
    begin                : Fri Jul 4 2003
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

#include "ckick.h"

namespace nDirectConnect {
namespace nTables {

cKick::cKick()
{
	mShare = 0;
	mTime = 0;
	mIsDrop = false;
}
cKick::~cKick(){
}

};
};
