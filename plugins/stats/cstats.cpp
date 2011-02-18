/***************************************************************************
 *   Copyright (C) 2004 by Daniel Muller                                   *
 *   dan at verliba dot cz                                                 *
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
#include <config.h>
#include "cstats.h"
#include <stdlib.h>
#include "src/ctime.h"
#include "src/cserverdc.h"

using namespace nDirectConnect;
using namespace nUtils;
namespace nStats
{

cStats::cStats(cServerDC *server): cConfMySQL(server->mMySQL), mS(server)
{
	AddFields();
}


cStats::~cStats()
{}

void cStats::CleanUp()
{
	mQuery.Clear();
	// I should probably make something more automated to create the table, maybe later
	mQuery.OStream() << "delete from " << mMySQLTable.mName << " where("
		"realtime < " << cTime().Sec() - 7 * 3600* 24 <<
		")";
	mQuery.Query();
	mQuery.Clear();
}

void cStats::AddFields()
{
	// this is useful for the parent class
	mMySQLTable.mName = "pi_stats";
	int i;
	ostringstream field_name;
	AddCol("realtime","int(11)","",false, mTime);
	AddPrimaryKey("realtime");
	AddCol("uptime", "int(11)","",true,mUptimeSec);
	AddCol("users_total","int(11)","0",true, mS->mUserCountTot);
	for (i=0; i <= USER_ZONES; i++)
	{
		field_name.str("");
		field_name << "users_zone" << i;
		AddCol(field_name.str().data(),"int(11)","0",true, mS->mUserCount[i]);
	}
	AddCol("upload_total","double","0",true, mUploadTotalBps);
	for (i=0; i <= USER_ZONES; i++)
	{
		field_name.str("");
		field_name << "upload_zone" << i;
		AddCol (field_name.str().data(),"double","0",true, mUploadInZonesBps[i]);
	}
	AddCol("share_total_gb","int(11)","0",true, mShareTotalGB);
	AddCol("freq_search_active","double","0",true, mFreqSearchA);
	AddCol("freq_search_passive","double","0",true, mFreqSearchP);
	AddCol("freq_user_login","double","0",true, mFreqUserLogin);
	AddCol("freq_user_logout","double","0",true, mFreqUserLogout);
	mMySQLTable.mExtra ="PRIMARY KEY (realtime)";

	SetBaseTo(this);
}


};
