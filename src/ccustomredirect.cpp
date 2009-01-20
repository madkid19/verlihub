/***************************************************************************
*   Copyright (C) 2009 by Simoncelli Davide                               *
*   netcelli@verlihub-project.org                                         *
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

#include "cserverdc.h"
#include "ccustomredirect.h"
#include "cconndc.h"
#include "curr_date_time.h"
#include "stringutils.h"
#include <stdlib.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

using namespace nStringUtils;
namespace nDirectConnect {
	namespace nTables {

		cRedirect::cRedirect()
		{
			mEnable = 1;
			mFlag = 0;
		}
		
		cRedirect::~cRedirect(){}

		void cRedirect::OnLoad()
		{}

		ostream &operator << (ostream &os, cRedirect &tr)
		{
			string buff;
			os << "\r" << tr.mAddress << " for ";
			int flag = tr.mFlag;
			if(flag & eKick) buff += "ban and kick,";
			if(flag & eUserLimit) buff += "hub full,";
			if(flag & eShareLimit) buff += "share limit,";
			if(flag & eTag) buff += "invalid tag,";
			if(flag & eWrongPasswd) buff += "wrong password,";
			if(buff.empty()) buff = "default";
			else buff.erase(buff.end() -1);
			os << buff << " (";
			if(tr.mEnable) os << "Enable";
			else os << "Disable";
			os << ")";
			return os;
		}

	};
};
 
