/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
*   devs at verlihub-project dot org                                      *
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

#ifndef CMGSLIST_H
#define CMGSLIST_H
#include "src/cserverdc.h"
#include "src/cconfmysql.h"

namespace nVerliHub {
	namespace nStatsPlugin {
/**
@author Daniel Muller
*/
class cStats : public nConfig::cConfMySQL
{
public:
	time_t mTime;
	double mUploadTotalBps;
	double mUploadInZonesBps[USER_ZONES+1];
	long mShareTotalGB;
	long mUptimeSec;
	double mFreqSearchA;
	double mFreqSearchP;
	double mFreqUserLogin;
	double mFreqUserLogout;
	double mFreqMyINFOSent;
	double mFreqMyINFOReceived;

	nSocket::cServerDC * mS;
	cStats(nSocket::cServerDC *server);
	void AddFields();
	virtual void CleanUp();
	virtual ~cStats();
};
	}; // namespace nStatsPlugin
}; // namespace nVerliHub
#endif
