/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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
#include "cserverdc.h"
#include "cban.h"
#include "ctime.h"
#include "cbanlist.h"

using namespace nUtils;
namespace nDirectConnect {
namespace nTables {

cBan::cBan(cServerDC *s) : cObj("cBan"), mS(s)
{
	mShare = 0;
	mDateStart = 0;
	mDateEnd = 0;
	mType = 0;
	mRangeMin = 0;
	mRangeMax = 0;
}
cBan::~cBan(){}

cUnBan::cUnBan(cServerDC *s):cBan(s){}

cUnBan::cUnBan(cBan &Ban, cServerDC *s): cBan(s)
{

	mIP=Ban.mIP;
	mNick=Ban.mNick;
	mHost=Ban.mHost;
	mShare=Ban.mShare;
	mMail=Ban.mMail;
	mRangeMin=Ban.mRangeMin;
	mRangeMax=Ban.mRangeMax;
	mDateStart=Ban.mDateStart;
	mDateEnd=Ban.mDateEnd;
	mNickOp=Ban.mNickOp;
	mReason=Ban.mReason;
	mType = Ban.mType;
}
cUnBan::~cUnBan(){}

};
};

ostream & operator << (ostream &os, nDirectConnect::nTables::cBan &ban)
{
	switch(ban.mDisplayType)
	{
		case 0: ban.DisplayComplete(os); break;
		case 1: ban.DisplayUser(os); break;
		case 2: ban.DisplayKick(os); break;
		default: os << "Unknown ban display" << "\r\n";
	}
	return os;
}

/*!
    \fn nDirectConnect::nTables::cBan::DisplayUser(ostream &)
 */
void nDirectConnect::nTables::cBan::DisplayUser(ostream &os)
{
	os << mS->mL.ban_reason << mReason << "\r\n";
	if(mDateEnd)
	{
		cTime HowLong(mDateEnd - cTime().Sec());
		os << mS->mL.ban_remaining << HowLong.AsPeriod() << "\r\n";
	}
	else os << mS->mL.ban_permanently << "\r\n";
	if(mIP.size() && mIP[0] != '_') os << mS->mL.ip << ": " << mIP << "\r\n" ;
	if(mNick.size()) os << mS->mL.nick << ": " << mNick << "\r\n";
	string tmp;
	if(mRangeMin)
	{
		cBanList::Num2Ip(mRangeMin, tmp);
		os << mS->mL.ip_range << tmp << "-";
		cBanList::Num2Ip(mRangeMax, tmp);
		os << tmp << "\r\n";
	}
	if(mShare) os << "Share: " << mShare << "\r\n";
}

void nDirectConnect::nTables::cUnBan::DisplayUser(ostream &os)
{
	this->cBan::DisplayUser(os);
	os << mS->mL.ban_removed << cTime(mDateUnban,0).AsDate() << mS->mL.ban_by << mUnNickOp << mS->mL.because << mUnReason << "\r\n";
}

/*!
    \fn nDirectConnect::nTables::cBan::DisplayComplete(ostream &os)
 */
void nDirectConnect::nTables::cBan::DisplayComplete(ostream &os)
{
	DisplayUser(os);
	os << mS->mL.op << ": " << mNickOp << "\r\n";
	os << mS->mL.ban_type << ": " << mS->mL.ban_types[mType];
}

void nDirectConnect::nTables::cUnBan::DisplayComplete(ostream &os)
{
	this->cBan::DisplayComplete(os);
	os << mS->mL.ban_removed << cTime(mDateUnban,0).AsDate() <<  mS->mL.ban_by << mUnNickOp << mS->mL.because  << mUnReason << "\r\n";
}

/*!
    \fn nDirectConnect::nTables::cBan::DisplayKick(ostream &os)
    A display method for the after kick output to operator
 */
void nDirectConnect::nTables::cBan::DisplayKick(ostream &os)
{
	if (mDateEnd)
	{
		cTime HowLong(mDateEnd-cTime().Sec(),0);
		if (HowLong.Sec() < 0)
		{
			os << mS->mL.ban_expired << HowLong.AsPeriod();
		}
		else
		{
			os << mS->mL.ban_for << HowLong.AsPeriod();
		}
	}
	else
	{
		os << mS->mL.ban_permanently;
	}
}


/*!
    \fn nDirectConnect::nTables::cBan::DisplayInline(ostream &os)
 */
void nDirectConnect::nTables::cBan::DisplayInline(ostream &os)
{
	static const char *sep = " \t ";
	os << mNick << sep << mIP << sep << mNickOp << sep;
	DisplayKick(os);
}
