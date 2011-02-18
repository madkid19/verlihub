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

#include "ciplog.h"
#include <stdlib.h>
#include "src/ctime.h"
#include "src/cserverdc.h"
#include "src/cbanlist.h"

using namespace nDirectConnect;
using namespace nDirectConnect::nTables;
using namespace nUtils;


namespace nIPLog
{


cIPLog::cIPLog(cServerDC *server): cConfMySQL(server->mMySQL), mS(server)
{
	AddFields();
}


cIPLog::~cIPLog()
{}

void cIPLog::CleanUp()
{
	mQuery.Clear();
	// I should probably make something more automated to create the table, maybe later
//	mQuery.OStream() << "delete from iplog where("
//		"realtime < " << cTime().Sec() - 7 * 3600* 24 <<
//		")";
//	mQuery.Query();
//	mQuery.Clear();
}

void cIPLog::GetNickHistory(const string &ip, int limit, ostream &os)
{
	GetHistory(ip, true, limit,os);
}

void cIPLog::GetIPHistory(const string &ip, int limit, ostream &os)
{
	GetHistory(ip, false, limit,os);
}

void cIPLog::GetLastNick(const string &ip, int limit, ostream &os)
{
	GetLastLogin(ip, false, limit, os);
}

void cIPLog::GetLastIP(const string &nick, int limit, ostream &os)
{
	GetLastLogin(nick, true, limit, os);	
}

void cIPLog::MakeSearchQuery(const string &who, bool isNick, int action, int limit)
{
	SelectFields(mQuery.OStream());
	mQuery.OStream() << (isNick?"WHERE nick='":"WHERE ip=");
	if (isNick){
		WriteStringConstant(mQuery.OStream(),who);
		mQuery.OStream() << "'";
	} else
		mQuery.OStream() << cBanList::Ip2Num(who);
	if (action>= 0) mQuery.OStream() << " AND action =" << action;
	mQuery.OStream() << " ORDER BY date DESC LIMIT " << limit;
}

void cIPLog::GetHistory(const string &who, bool isNick, int limit, ostream &os)
{
	string ip;
	os << "Last " << limit << " events of " << (isNick?"Nick ":"IP ") << who << ":\r\n";

	MakeSearchQuery(who, isNick, -1, limit);
	SetBaseTo(&mModel);

	const char *Actions[]={"connect","login","logout","disconnect"};
	const char *Infos[]={"",
		"bad nick, or banned nick or ip or whatever",
		"used different nick in chat",
		"kicked",
		"redirected",
		"quits himself",
		"critical hub load",
		"timeout",
		"user did nothing for too long time",
		"user limit exceeded for this user",
		"min or max share limit",
		"no tags in description (or badly parsed)",
		"tags not validated (general), slots or hubs or limiter, or version etc..",
		"wrong password",
		"error in login sequence",
		"syntax error in some message"
	};
	db_iterator it;
	for(it = db_begin(); it != db_end(); ++it)
	{
		cBanList::Num2Ip(mModel.mIP, ip);
		if (mModel.mType < 4) os << Actions[mModel.mType];
		else os << mModel.mType;
		os << " : " << cTime(mModel.mDate,0).AsDate() << " - " << (isNick?ip:mModel.mNick) << " - ";
		if (mModel.mInfo < 16) os << Infos[mModel.mInfo];
		else os << mModel.mInfo;
		os << "\r\n";
	}

	mQuery.Clear();
}

void cIPLog::GetLastLogin(const string &who, bool isNick, int limit, ostream &os)
{
	string ip;
	os << (isNick?"Nick ":"IP ") << who << " has lately been here " << (isNick?"on IPs":"withNicks")<<"\r\n";

	MakeSearchQuery(who, isNick, 1, limit);
	SetBaseTo(&mModel);

	db_iterator it;
	for(it = db_begin(); it != db_end(); ++it)
	{
		cBanList::Num2Ip(mModel.mIP, ip);
		os << cTime(mModel.mDate,0).AsDate() << " - " << (isNick?ip:mModel.mNick) << "\r\n";
	}

	mQuery.Clear();
}

void cIPLog::AddFields()
{
	// this is useful for the parent class
	mMySQLTable.mName = "pi_iplog";
	AddCol("date","int(11)","",true, mModel.mDate );
	AddCol("action","smallint(6)","0",true, mModel.mType);
	AddCol("ip", "bigint(20)","",true, mModel.mIP);
	AddCol("nick", "varchar(32)","",true, mModel.mNick);
	AddCol("info","int(11)","",true, mModel.mInfo);
	mMySQLTable.mExtra="id bigint(20) NOT NULL AUTO_INCREMENT PRIMARY KEY,"
		"index ind_ip(ip), index ind_nick(nick)";
	SetBaseTo(&mModel);
}


/** log that user logged in */
bool cIPLog::Log(cConnDC *conn, int action, int info)
{
	sUserStruct entry;
	entry.mIP = cBanList::Ip2Num(conn->AddrIP());
	if (conn->mpUser != NULL) entry.mNick = conn->mpUser->mNick;
	else entry.mNick = "";

	
	entry.mDate = cTime().Sec();
	entry.mType = action;
	entry.mInfo = info;
	SetBaseTo(&entry);
	return SavePK();
}

};
