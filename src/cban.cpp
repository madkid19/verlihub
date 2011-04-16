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
#include "cserverdc.h"
#include "cban.h"
#include "ctime.h"
#include "cbanlist.h"
#include "stringutils.h"
#include "i18n.h"

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

	mIP = Ban.mIP;
	mNick = Ban.mNick;
	mHost = Ban.mHost;
	mShare = Ban.mShare;
	mMail = Ban.mMail;
	mRangeMin = Ban.mRangeMin;
	mRangeMax = Ban.mRangeMax;
	mDateStart = Ban.mDateStart;
	mDateEnd = Ban.mDateEnd;
	mNickOp = Ban.mNickOp;
	mReason = Ban.mReason;
	mType = Ban.mType;
}
cUnBan::~cUnBan(){}

};
};

ostream & operator << (ostream &os, nDirectConnect::nTables::cBan &ban)
{
	switch(ban.mDisplayType) {
		case 0: ban.DisplayComplete(os); break;
		case 1: ban.DisplayUser(os); break;
		case 2: ban.DisplayKick(os); break;
		default: os << _("Unknown ban") << "\r\n";
	}
	return os;
}

void nDirectConnect::nTables::cBan::DisplayUser(ostream &os)
{
	if(mNick.size())
		os << autosprintf(_("Nick: %s"), mNick.c_str()) << "\r\n";
	if(mIP.size() && mIP[0] != '_')
		os << autosprintf(_("IP: %s"), mIP.c_str()) << "\r\n";
	os << autosprintf(_("Reason: %s"), mReason.c_str());
	// Append extra ban message
	if(!mS->mC.ban_extra_message.empty())
		os << " " << mS->mC.ban_extra_message.empty();
	os << "\r\n";
	if(mDateEnd) {
		cTime HowLong(mDateEnd - cTime().Sec());
		os << autosprintf(_("Remaining: %s"), HowLong.AsPeriod().AsString().c_str()) << "\r\n";
	} else
		os << _("Permanently.") << "\r\n";

	string initialRange, endRange;
	if(mRangeMin) {
		cBanList::Num2Ip(mRangeMin, initialRange);
		cBanList::Num2Ip(mRangeMax, endRange);
		os <<  autosprintf(_("IP range: %s-%s"), initialRange.c_str(), endRange.c_str()) << "\r\n";
	}
	if(mShare)
		os << autosprintf(_("Share: %s"), nStringUtils::convertByte(mShare, false).c_str()) << "\r\n";
}

void nDirectConnect::nTables::cUnBan::DisplayUser(ostream &os)
{
	this->cBan::DisplayUser(os);
	os << autosprintf(_("Removed: %s by %s because %s"), cTime(mDateUnban,0).AsDate().AsString().c_str(), mUnNickOp.c_str(), mUnReason.c_str()) << "\r\n";
}

void nDirectConnect::nTables::cBan::DisplayComplete(ostream &os)
{
	DisplayUser(os);
	os << autosprintf(_("OP: %s"), mNickOp.c_str()) << "\r\n";
	os << autosprintf(_("Ban type: %s"), this->GetBanType());
}

const char *nDirectConnect::nTables::cBan::GetBanType()
{
	static const char *banTypes[] = {_("Nick+IP"), _("IP"), _("Nick"),_("IP Range"), _("Host Level 1"),_("Host Level 2"), _("Host Level 3"), _("Share Size"), _("E-mail"), _("Nick Prefix"), _("Reverse Host")};
	return banTypes[mType];
}

void nDirectConnect::nTables::cUnBan::DisplayComplete(ostream &os)
{
	this->cBan::DisplayComplete(os);
	os << autosprintf(_("Removed: %s by %s because %s"), cTime(mDateUnban,0).AsDate().AsString().c_str(), mUnNickOp.c_str(), mUnReason.c_str()) << "\r\n";
}

void nDirectConnect::nTables::cBan::DisplayKick(ostream &os)
{
	if(mDateEnd) {
		cTime HowLong(mDateEnd-cTime().Sec(),0);
		if(HowLong.Sec() < 0) {
			os << autosprintf(_("expired on: %s"), HowLong.AsDate().AsString().c_str());
		} else {
			os << autosprintf(_("for: %s"), HowLong.AsPeriod().AsString().c_str());
		}
	} else {
		os << _("permanently");
	}
}

void nDirectConnect::nTables::cBan::DisplayInline(ostream &os)
{
	static const char *sep = " \t ";
	switch(1 << mType) {
		case eBF_NICK:
			os << mNick;
		break;
		case eBF_IP:
		case eBF_RANGE:
			os << mIP;
		break;
		case eBF_HOST1:
		case eBF_HOST2:
		case eBF_HOST3:
			os << mHost;
		break;
		case eBF_SHARE:
			os << nStringUtils::convertByte(mShare,false);
		break;
		case eBF_PREFIX:
		default:
			os << mNick; // TODO: fix me
		break;
	}
	os << sep << mNickOp << sep;
	DisplayKick(os);
	os << sep << GetBanType();
}
