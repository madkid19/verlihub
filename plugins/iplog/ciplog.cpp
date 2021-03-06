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

#include "ciplog.h"
#include <stdlib.h>
#include "src/ctime.h"
#include "src/cserverdc.h"
#include "src/cbanlist.h"
#include "src/i18n.h"
#include "stringutils.h"

namespace nVerliHub {
	using namespace nEnums;
	using namespace nTables;
	using namespace nUtils;
	namespace nIPLogPlugin {


cIPLog::cIPLog(cServerDC *server): cConfMySQL(server->mMySQL), mS(server)
{
	AddFields();
}


cIPLog::~cIPLog()
{}

void cIPLog::CleanUp()
{
	mQuery.Clear();
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
	mQuery.OStream() << (isNick ? "WHERE nick='" : "WHERE ip=");
	if(isNick) {
		WriteStringConstant(mQuery.OStream(),who);
		mQuery.OStream() << "'";
	} else
		mQuery.OStream() << cBanList::Ip2Num(who);
	if(action>= 0)
		mQuery.OStream() << " AND action =" << action;
	mQuery.OStream() << " ORDER BY date DESC LIMIT " << limit;
}

void cIPLog::GetHistory(const string &who, bool isNick, int limit, ostream &os)
{
	string ip;
	if(isNick)
		os << autosprintf(_("Last %d events of nick %s:"), limit, who.c_str()) << "\r\n";
	else
		os << autosprintf(_("Last %d events of IP %s:"), limit, who.c_str()) << "\r\n";

	MakeSearchQuery(who, isNick, -1, limit);
	SetBaseTo(&mModel);

	const char *Actions[]={_("connect"),_("login"),_("logout"),_("disconnect")};
	const char *Infos[]={
	    "--",
		_("bad nick or nick temporarily banned"),
		_("used different nick in chat"),
		_("kicked"),
		_("redirected"),
		_("exit from the hub"),
		_("critical hub load"),
		_("timeout"),
		_("user did nothing for too long time"),
		_("hub full"),
		_("share limit"),
		_("no tag or not valid"),
		_("tag breaks hub rules"),
		_("wrong password"),
		_("error in login sequence"),
		_("syntax error in some messages"),
		_("invalid key")
	};

	os << "\n ";
	os << setw(20) << setiosflags(ios::left) << toUpper(_("Date"));
	os << setw(20) << setiosflags(ios::left) << toUpper(_("Action"));
	os << setw(15) << setiosflags(ios::left) << (isNick ? "IP" : toUpper(_("Nickname")));
	os << toUpper(_("Info")) << "\n";
	os << " " << string(20+20+15+25,'=') << endl;

	db_iterator it;
	for(it = db_begin(); it != db_end(); ++it) {
		cBanList::Num2Ip(mModel.mIP, ip);
		os << " " <<  setw(20) << setiosflags(ios::left) << cTime(mModel.mDate,0).AsDate();
		os << setw(20) << setiosflags(ios::left);
		if(mModel.mType < 4)
			os << Actions[mModel.mType];
		else
			os << mModel.mType;
		os << setw(15) << setiosflags(ios::left) << (isNick ? ip : mModel.mNick.substr(0,14));
		if(mModel.mInfo < 16) {
			if(strlen(Infos[mModel.mInfo]) > 0)
				os << Infos[mModel.mInfo];
		} else
			os << mModel.mInfo;
		os << endl;
	}

	mQuery.Clear();
}

void cIPLog::GetLastLogin(const string &who, bool isNick, int limit, ostream &os)
{
	string ip;
	if(isNick)
		os << autosprintf(_("Nick %s has lately been in the hub with the following IP"), who.c_str()) << "\n";
	else
		os << autosprintf(_("IP %s has lately been in the hub with following nicknames"), who.c_str()) << "\n";

	MakeSearchQuery(who, isNick, 1, limit);
	SetBaseTo(&mModel);

	os << "\n ";
	os << setw(25) << setiosflags(ios::left) << toUpper(_("Date"));
	os << (isNick ? "IP" : toUpper(_("Nickname"))) << "\n";
	os << " " << string(25+20,'=') << endl;

	db_iterator it;
	for(it = db_begin(); it != db_end(); ++it) {
		cBanList::Num2Ip(mModel.mIP, ip);
		os << " " << setw(25) << setiosflags(ios::left) << cTime(mModel.mDate,0).AsDate();
		os << (isNick ? ip : mModel.mNick) << endl;
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
	AddCol("nick", "varchar(64)","",true, mModel.mNick);
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
	if(conn->mpUser != NULL)
		entry.mNick = conn->mpUser->mNick;
	else
		entry.mNick = "";

	entry.mDate = cTime().Sec();
	entry.mType = action;
	entry.mInfo = info;
	SetBaseTo(&entry);
	return SavePK();
}
	}; // namespace nIPLogPlugin
}; // namespace
