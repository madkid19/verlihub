/***************************************************************************
 *   Copyright (C) 2003 by dan at verliba dot cz Muller                                      *
 *   dan at verliba dot cz@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cconndc.h"
#include "cserverdc.h"
#include "cbanlist.h"
#include <stdio.h>

namespace nDirectConnect {
namespace nTables {

cBanList::cBanList(cServerDC *s)
 : cConfMySQL(s->mMySQL), mS(s),mModel(s)
{
	mMySQLTable.mName = "banlist";
	AddCol("ip", "varchar(15)", "", true, mModel.mIP);
	AddPrimaryKey("ip");
	AddCol("nick", "varchar(64)", "", true, mModel.mNick);
	AddPrimaryKey("nick");
	AddCol("ban_type", "tinyint(4)", "0", true, mModel.mType);
	AddCol("host", "text", "", true, mModel.mHost);
	AddCol("range_fr", "bigint(32)", "", true, mModel.mRangeMin);
	AddCol("range_to", "bigint(32)", "", true, mModel.mRangeMax);
	AddCol("date_start", "int(11)", "0", true, mModel.mDateStart);
	AddCol("date_limit", "int(11)", "", true, mModel.mDateEnd);
	AddCol("nick_op", "varchar(30)", "", true, mModel.mNickOp);
	AddCol("reason", "text", "", true, mModel.mReason);
	AddCol("share_size", "varchar(15)", "", true, mModel.mShare);
	AddCol("email", "varchar(128)", "", true, mModel.mMail);
	mMySQLTable.mExtra = "UNIQUE (ip,nick), ";
	mMySQLTable.mExtra+= "INDEX nick_index (nick), ";
	mMySQLTable.mExtra+= "INDEX date_index (date_limit), ";
	mMySQLTable.mExtra+= "INDEX range_index (range_fr)";
	SetBaseTo(&mModel);
}

cBanList::~cBanList(){
	RemoveOldShortTempBans(0);
}

cUnBanList::cUnBanList(cServerDC* s) : cBanList(s), mModelUn(s)
{
	mMySQLTable.mName = "unbanlist";
	SetBaseTo(&mModelUn);
	AddCol("date_unban", "int(11)", "", true, mModelUn.mDateUnban);
	AddPrimaryKey("date_unban");
	AddCol("unban_op", "varchar(30)", "", true, mModelUn.mUnNickOp);
	AddCol("unban_reason", "text", "", true, mModelUn.mUnReason);
	mMySQLTable.mExtra = "UNIQUE (ip, nick, date_unban)";
}

cUnBanList::~cUnBanList(){}

};};

void nDirectConnect::nTables::cBanList::Cleanup()
{
	time_t Now = cTime().Sec();
	mQuery.OStream() << "DELETE FROM " << mMySQLTable.mName <<
		" WHERE date_limit IS NOT NULL AND date_limit < " <<
		Now - 3600*24*7;
	mQuery.Query();
	mQuery.Clear();
}

void nDirectConnect::nTables::cUnBanList::Cleanup()
{
	time_t Now = cTime().Sec();
	mQuery.OStream() << "DELETE FROM " <<
		mMySQLTable.mName << " WHERE date_unban < " << Now - 3600*24*30;
	mQuery.Query();
	mQuery.Clear();
}

/*!
    \fn nDirectConnect::nTables::cBanList::UpdateBan(cBan &ban)
 */
int nDirectConnect::nTables::cBanList::UpdateBan(cBan &ban)
{
	cQuery query(mMySQL);
	SetBaseTo(&ban);

	UpdateFields(query.OStream());
	WherePKey( query.OStream() );
	query.Query();
   return 0;
}


/*!
    \fn nDirectConnect::nTables::cBanList::LoadBanByKey(cBan &ban)
    \param ban the destination and a structure containing the primary key
    \param return wheather loaded
 */
bool nDirectConnect::nTables::cBanList::LoadBanByKey(cBan &ban)
{
	SetBaseTo( &ban );
	return LoadPK();
}

/*!
	\brief Generate - fill in a ban structure ready to be added
	\fn nDirectConnect::nTables::cBanList::NewBan(cBan &ban, cConnDC *conn, const string &nick_op, const string &reason, unsigned length, unsigned type)
	\param ban Destination of the new ban
	\param conn the currently being banned connection
	\param nick_op operator performing the ban
	\param reason ban reason
	\param length number of seconds to be banned, 0 - means permban
	\param type use the enum from cBan::tBanFlags, only accepted are values defined in the enum
	\sa cBan::tBanFlags
 */
void nDirectConnect::nTables::cBanList::NewBan(cBan &ban, cConnDC *conn, const string &nick_op, const string &reason, unsigned length, unsigned type)
{
	if(conn != NULL)
	{
		ban.mIP   = conn->AddrIP();
		ban.mHost = conn->AddrHost();
		ban.mDateStart = cTime().Sec();
		ban.mDateEnd = ban.mDateStart + length;
		ban.mReason = reason;
		ban.mNickOp = nick_op;
		ban.SetType(type);
		if( conn->mpUser )
		{
			ban.mNick  = conn->mpUser->mNick;
			ban.mShare = conn->mpUser->mShare;
		}
		else
		{
			ban.mNick = "nonick_" + ban.mIP;
		}
	}
}

/*!
	\brief Add ban to the database
	\fn nDirectConnect::nTables::cBanList::AddBan(cBan &ban)
	\param ban the Ban to be added, created by NewBan or similar
	\sa NewBan

	This one finnished up the filling of ban structure. Searches for the ban record in database.
	When found the time adition is performed. Otherwise the ban is  inserted as it is.
 */
void nDirectConnect::nTables::cBanList::AddBan(cBan &ban)
{
	//@todo nick2dbkey
	switch (1 << ban.mType)
	{
	case cBan::eBF_NICK: ban.mIP = "_nickban_"; break;
	case cBan::eBF_IP: ban.mNick = "_ipban_"; break;
	case cBan::eBF_RANGE: ban.mNick = "_rangeban_"; break;
	case cBan::eBF_HOST1: ban.mIP = "_host1ban_";
		if(!this->GetHostSubstring(ban.mHost,ban.mNick,1)) return; break;
	case cBan::eBF_HOST2: ban.mIP = "_host2ban_";
		if(!this->GetHostSubstring(ban.mHost,ban.mNick,2)) return; break;
	case cBan::eBF_HOST3: ban.mIP = "_host3ban_";
		if(!this->GetHostSubstring(ban.mHost,ban.mNick,3)) return; break;
	case cBan::eBF_HOSTR1: ban.mIP = "_hostr1ban_";
		if(!this->GetHostSubstring(ban.mHost,ban.mNick,-1)) return; break;
	case cBan::eBF_SHARE: ban.mNick = "_shareban_";
		ban.mIP = ban.mShare; break;
	case cBan::eBF_EMAIL: ban.mNick = "_emailban_";
		ban.mIP = ban.mMail; break;
	case cBan::eBF_PREFIX: ban.mIP = "_prefixban_"; break;
	default: break;
	}
   
	// copy PK
	cBan OldBan(mS);
	OldBan.mIP = ban.mIP;
	OldBan.mNick = ban.mNick;
	// Load by PK to mModel
	SetBaseTo( &OldBan );
	bool update = false;

	if(LoadPK())
	{
		update = true;
		mModel = OldBan;
		if(ban.mReason.size())
			mModel.mReason += "(" + mModel.mNickOp + ")\r\n" + ban.mReason;
		if(!ban.mDateEnd || (ban.mDateEnd > mModel.mDateEnd))
			mModel.mDateEnd = ban.mDateEnd;
		mModel.mNickOp = ban.mNickOp;

		if((1 << ban.mType) == cBan::eBF_RANGE)
		{
			mModel.mRangeMin = ban.mRangeMin;
			mModel.mRangeMax = ban.mRangeMax;
		}
	}
	else mModel = ban;

	SetBaseTo( &mModel );

	if( update ) UpdatePK();
	else SavePK(false);
}

/*!
    \fn nDirectConnect::nTables::cBanList::TestBan(cBan &ban, cConnDC *conn, const string &Nick, unsigned mask)
    \param ban destination where found ban record is loaded
    \param conn a connection for which the bans is testes
    \param Nick maybe banned nickname
    \param mask binary OR'ed sum of cBan flags to test ban for
 */
bool nDirectConnect::nTables::cBanList::TestBan(cBan &ban, cConnDC *conn, const string &Nick, unsigned mask)
{
	ostringstream query;
	if(conn != NULL)
	{
		string ip = conn->AddrIP();
		SelectFields(query);
		string host = conn->AddrHost();
		// IP and NICK and BOTH are done by this first one
		query << " WHERE (";
		if( mask & (cBan::eBF_NICKIP |cBan::eBF_IP) )
		{
			AddTestCondition(query , ip   , cBan::eBF_IP  );
			query << " OR ";
		}
		if( mask & (cBan::eBF_NICKIP |cBan::eBF_NICK) )//@todo nick2dbkey
			AddTestCondition( query , Nick , cBan::eBF_NICK);
		if( mask & cBan::eBF_RANGE ) AddTestCondition (query << " OR ", ip , cBan::eBF_RANGE);
		if( conn->mpUser != NULL)
		{
//			if( mask & cBan::eBF_SHARE )
//				AddTestCondition (query << " OR ", conn->mpUser->mShare, cBan::eBF_SHARE);
			if( mask & cBan::eBF_EMAIL )
				AddTestCondition (query << " OR ", conn->mpUser->mEmail, cBan::eBF_EMAIL);
		}
		if( mask & cBan::eBF_HOST1 ) AddTestCondition (query << " OR ", host, cBan::eBF_HOST1);
		if( mask & cBan::eBF_HOST2 ) AddTestCondition (query << " OR ", host, cBan::eBF_HOST2);
		if( mask & cBan::eBF_HOST3 ) AddTestCondition (query << " OR ", host, cBan::eBF_HOST3);
		if( mask & cBan::eBF_HOSTR1 ) AddTestCondition (query << " OR ", host, cBan::eBF_HOSTR1);
		if( mask & cBan::eBF_PREFIX ) AddTestCondition (query << " OR ", Nick, cBan::eBF_PREFIX);

		query << " ) AND ( (date_limit >= " << cTime().Sec() <<
			") OR date_limit IS NULL OR (date_limit = 0)) ORDER BY date_limit DESC LIMIT 1";

		if( -1 == StartQuery( query.str() )) return false;
		SetBaseTo( &ban );
		bool found = (Load() >= 0);
		EndQuery();
		return found;
	}
	else return false;
	/*
	*/
}



/*!
    \fn nDirectConnect::nTables::cBanList::DelBan(cBan &Ban)
 */
void nDirectConnect::nTables::cBanList::DelBan(cBan &Ban)
{
	SetBaseTo( &Ban );
	DeletePK();
}


/*!
    \fn nDirectConnect::nTables::cBanList::DeleteAllBansBy(const string &IP, const string &Nick, int Flags)
 */
int nDirectConnect::nTables::cBanList::DeleteAllBansBy(const string &IP, const string &Nick, int Flags)
{
	mQuery.OStream() << "DELETE FROM " << mMySQLTable.mName << " WHERE ";
	if(Flags & cBan::eBF_IP) mQuery.OStream() << " ip = '" << IP << "'";
	if(Flags & cBan::eBF_IP | cBan::eBF_NICK) mQuery.OStream() << " AND";
	if(Flags & cBan::eBF_NICK) mQuery.OStream() << " nick = '" << Nick << "'";

	return mQuery.Query();
}


/*!
    \fn nDirectConnect::nTables::cBanList::NewBan(cBan &Dest, const  cKick &Kick, time_t period, int Flags)
 */
void nDirectConnect::nTables::cBanList::NewBan(cBan &ban, const cKick &Kick, long period, int type)
{
	ban.mIP   = Kick.mIP;
	ban.mDateStart = cTime().Sec();
	if(period) ban.mDateEnd = ban.mDateStart + period;
	else ban.mDateEnd = 0;
	ban.mReason = Kick.mReason;
	ban.mNickOp = Kick.mOp;
	ban.mNick  = Kick.mNick;
	ban.SetType(type);
	ban.mHost = Kick.mHost;
	ban.mMail = Kick.mEmail;
	ban.mShare = Kick.mShare;
}

// Remove ban for given ip nick or whatever.. and also corresponding bans.
int nDirectConnect::nTables::cBanList::Unban(ostream &os, const string &What, const string &reason, const string &NickOp, int TypeOfWhat, bool DoIt)
{
	SelectFields(mQuery.OStream());
	if(! AddTestCondition(mQuery.OStream() << " WHERE ", What, TypeOfWhat))
	{
		mQuery.Clear();
		return 0;
	}
	db_iterator it;
	cUnBan *unban = NULL;
	int i = 0;
	SetBaseTo(&mModel);

	for( it = db_begin(); it != db_end(); ++it )
	{
		mModel.DisplayComplete(os);
		if( DoIt )
		{
			unban = new cUnBan(mModel, mS);
			unban->mUnReason = reason;
			unban->mUnNickOp = NickOp;
         unban->mDateUnban = cTime().Sec();
			mUnBanList->SetBaseTo(unban);
			mUnBanList->SavePK();
			delete unban;
		}
		i++;
	}
	mQuery.Clear();
	if( DoIt )
	{
		mQuery.OStream() << "DELETE FROM " << this->mMySQLTable.mName << " WHERE ";
		AddTestCondition(mQuery.OStream() , What, TypeOfWhat);
		mQuery.Query();
		mQuery.Clear();
	}
	return i;
}

/*
 * Extract the host (of given level) substring from a given string
 */
bool nDirectConnect::nTables::cBanList::GetHostSubstring(const string &src, string &dest, int level)
{
	string tmp(".");
	size_t pos;
	if (level > 0)
	{
		tmp += src;
		pos = tmp.npos;
		for (int i = 0; i < level; i++)
		{
			if (!pos) return false;
			pos = tmp.rfind('.',pos-1);
		}
		dest.assign(tmp, pos, tmp.size()-pos);
	}
	if (level < 0)
	{
		tmp = src;
		pos = 0;
		for (int i = 0; i < -level; i++)
		{
			if (pos == tmp.npos) return false;
			pos = tmp.find('.',pos+1);
		}
		dest.assign(tmp, 0, pos);
	}

	return true;
}



/*!
    \fn nDirectConnect::nTables::cBanList::AddTestCondition(ostream &os, const string &What, int Type)
 */
bool nDirectConnect::nTables::cBanList::AddTestCondition(ostream &os, const string &What, int Type)
{
	string host;
	unsigned long num;
	switch(Type)
	{
		case cBan::eBF_NICK : os << "( nick = '"; cConfMySQL::WriteStringConstant(os, What); os << "')"; break;
		case cBan::eBF_IP : os << "(ip='"; cConfMySQL::WriteStringConstant(os, What); os << "')"; break;
		//case (int)cBan::eBF_NICK  : os << "(ip='_nickban_' AND nick='" << What << "')"; break;
		//case (int)cBan::eBF_IP    : os << "(nick='_ipban_' AND ip='" << What << "')"; break;
		case cBan::eBF_RANGE :
			num = Ip2Num(What);
			os << "(nick='_rangeban_' AND " << num << " BETWEEN range_fr AND range_to )"; break;
		case cBan::eBF_SHARE :
			os << "(nick='_shareban_' AND ip = '" << What << "')"; break;
		case cBan::eBF_EMAIL :
			os << "(nick='_emailban_' AND ip = '" << What << "')"; break;
		case cBan::eBF_HOST1 :
			if( ! this->GetHostSubstring(What, host, 1)) { os << " 0 "; return false; }
			os << "(ip='_host1ban_' AND '" << host << "' = nick)"; break;
		case cBan::eBF_HOST2 :
			if( ! this->GetHostSubstring(What, host, 2)) { os << " 0 "; return false; }
			os << "(ip='_host2ban_' AND '" << host << "' = nick)"; break;
		case cBan::eBF_HOST3 :
			if( ! this->GetHostSubstring(What, host, 3)) { os << " 0 "; return false; };
			os << "(ip='_host3ban_' AND '" << host << "' = nick)"; break;
		case cBan::eBF_HOSTR1 :
			if( ! this->GetHostSubstring(What, host, -1)) { os << " 0 "; return false; };
			os << "(ip='_hostr1ban_' AND '" << host << "' = nick)"; break;
		case cBan::eBF_PREFIX :
			os << "(ip='_prefixban_' AND nick=LEFT('";cConfMySQL::WriteStringConstant(os, What); os << "',LENGTH(nick)))"; break;
		default: return false;
	}
	return true;
}


/*!
    \fn nDirectConnect::nTables::cBanList::List(ostream &os, int)
 */
void nDirectConnect::nTables::cBanList::List(ostream &os, int count)
{
	mQuery.Clear();
	SelectFields(mQuery.OStream());
	mQuery.OStream() << " order by date_start desc limit " << count;
	db_iterator it;
	SetBaseTo(&mModel);
	os << "Last " << count << " bans added:" << "\r\n";

	for(it = db_begin() ; it != db_end(); ++it)
	{
		mModel.DisplayInline(os);
		os << "\r\n";
	}
	mQuery.Clear();
}


/*!
    \fn nDirectConnect::nTables::cBanList::Ip2Num(const string &ip)
 */
unsigned long nDirectConnect::nTables::cBanList::Ip2Num(const string &ip)
{
	int i;
	char c;
	istringstream is(ip);
	unsigned long mask = 0;
	is >> i >> c; mask += i & 0xFF; mask <<= 8;
	is >> i >> c; mask += i & 0xFF; mask <<= 8;
	is >> i >> c; mask += i & 0xFF; mask <<= 8;
	is >> i     ; mask += i & 0xFF;
	return mask;
}

void nDirectConnect::nTables::cBanList::Num2Ip(unsigned long mask, string &ip)
{
	ostringstream os;
	unsigned char *i = (unsigned char *)&mask;
	os << int(i[3]) << ".";
	os << int(i[2]) << ".";
	os << int(i[1]) << ".";
	os << int(i[0]);
	ip = os.str();
}

/// \return 0 if not found else time of the end
long nDirectConnect::nTables::cBanList::IsNickTempBanned(const string &nick)
{
	unsigned long hash= mTempNickBanlist.HashStringLower(nick);
	sTempBan *tban = this->mTempNickBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		return tban->mUntil;
	}
	return 0;
}

/// \return 0 if not found else time of the end
long nDirectConnect::nTables::cBanList::IsIPTempBanned(unsigned long ip)
{
	unsigned long hash=ip; 
	sTempBan *tban = this->mTempIPBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		return tban->mUntil;
	}
	return 0;
}

void nDirectConnect::nTables::cBanList::AddNickTempBan(const string &nick, long until, const string &reason)
{
	unsigned long hash = mTempNickBanlist.HashStringLower(nick);
	sTempBan *tban = this->mTempNickBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		tban->mUntil = time_t(until);
		tban->mReason = reason;
	}
	else
	{
		tban = new sTempBan(until, reason);
		this->mTempNickBanlist.AddWithHash(tban, hash );
	}
}

void nDirectConnect::nTables::cBanList::AddIPTempBan(unsigned long ip, long until, const string &reason)
{
	unsigned long hash = ip; 
	sTempBan *tban = this->mTempIPBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		tban->mUntil = time_t(until);
		tban->mReason = reason;
	}
	else
	{
		tban = new sTempBan(until, reason);
		this->mTempIPBanlist.AddWithHash(tban, hash );
	}
}

void nDirectConnect::nTables::cBanList::DelNickTempBan(const string &nick)
{
	unsigned long hash= mTempNickBanlist.HashStringLower(nick);
	sTempBan *tban = this->mTempNickBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		this->mTempNickBanlist.RemoveByHash(hash);
		delete tban;
	}
}

void nDirectConnect::nTables::cBanList::DelIPTempBan(unsigned long ip)
{
	unsigned long hash = ip;
	sTempBan *tban = this->mTempIPBanlist.GetByHash(hash);
	if (tban != NULL)
	{
		this->mTempIPBanlist.RemoveByHash(hash);
		delete tban;
	}
}

/**
  * \return the number of removed items
  */
int nDirectConnect::nTables::cBanList::RemoveOldShortTempBans(long before)
{
	int n = 0;
	tTempNickBans::iterator it;
	unsigned long Hash;
	long Until;
	sTempBan *tban;
	
	for(it = mTempNickBanlist.begin(); it != mTempNickBanlist.end();)
	{
		Hash = it.mItem->mHash;
		tban = *it;
		Until = tban->mUntil;
		
		++it;
		if(!before || (Until< before))
		{
			this->mTempNickBanlist.RemoveByHash(Hash);
			delete tban;
			n++;
		}
	}
	for(it = mTempIPBanlist.begin(); it != mTempIPBanlist.end();)
	{
		Hash = it.mItem->mHash;
		tban = *it;
		Until = tban->mUntil;
		
		++it;
		if(!before || (Until< before))
		{
			this->mTempIPBanlist.RemoveByHash(Hash);
			delete tban;
			n++;
		}
	}
	return n;
}
