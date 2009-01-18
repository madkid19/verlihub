/***************************************************************************
                          coperator.h  -  description
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

#ifndef COPERATOR_H
#define COPERATOR_H

#include "cuser.h"

namespace nDirectConnect
{

/**operator has some extra data to store, like kicks, ips, bans etc..
  *@author Daniel Muller
  */

class cOperator : public cUser
{
public: 
	cOperator();
	~cOperator();
	
//	typedef
//	mKickList
};

};
#endif
