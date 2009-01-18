/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 
