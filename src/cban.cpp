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

#define PADDING 25

namespace nVerliHub {
	using namespace nEnums;
	using namespace nUtils;
	using namespace nSocket;
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
	mRangeMin = Ban.mRangeMin;
	mRangeMax = Ban.mRangeMax;
	mDateStart = Ban.mDateStart;
	mDateEnd = Ban.mDateEnd;
	mNickOp = Ban.mNickOp;
	mReason = Ban.mReason;
	mType = Ban.mType;
}
cUnBan::~cUnBan(){}

ostream & operator << (ostream &os, cBan &ban)
{
	switch(ban.mDisplayType) {
		case 0:
			ban.DisplayComplete(os);
			break;
		case 1:
			ban.DisplayUser(os);
			break;
		case 2:
			ban.DisplayKick(os);
			break;
		default:
			os << _("Unknown ban") << endl;
	}
	return os;
}

void cBan::DisplayUser(ostream &os)
{
	os << "\r\n";
	if (mNick.size()) os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Nickname") << mNick.c_str() << "\r\n";
	if (mIP.size() && (mIP[0] != '_')) os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("IP") << mIP.c_str() << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Reason") << mReason.c_str() << "\r\n";
	// append extra ban message
	if (!mS->mC.ban_extra_message.empty()) os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Extra message") << mS->mC.ban_extra_message << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Time");

	if (mDateEnd) {
		cTime HowLong(mDateEnd - cTime().Sec());
		os << HowLong.AsPeriod().AsString().c_str() << " " << _("remaining");
	} else
		os << _("Permanently");

	os << "\r\n";
	string initialRange, endRange;

	if (mRangeMin) {
		cBanList::Num2Ip(mRangeMin, initialRange);
		cBanList::Num2Ip(mRangeMax, endRange);
		os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("IP range") << initialRange.c_str() << "-" << endRange.c_str() << "\r\n";
	}

	if (mShare) os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Share") << mShare << " (" << convertByte(mShare, false).c_str() << ")" << "\r\n";
}

void cUnBan::DisplayUser(ostream &os)
{
	this->cBan::DisplayUser(os);
	os << autosprintf(_("Removed: %s by %s because %s"), cTime(mDateUnban,0).AsDate().AsString().c_str(), mUnNickOp.c_str(), mUnReason.c_str()) << "\r\n";
}

void cBan::DisplayComplete(ostream &os)
{
	DisplayUser(os);
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("OP") << mNickOp.c_str() << "\r\n";
	os << " [*] " << setw(PADDING) << setiosflags(ios::left) << _("Ban type") << this->GetBanType() << "\r\n";
}

const char *cBan::GetBanType()
{
	static const char *banTypes[] = {_("Nick + IP"), _("IP"), _("Nick"), _("IP range"), _("Host level 1"), _("Host level 2"), _("Host level 3"), _("Share size"), _("Nick prefix"), _("Reverse host")};
	return banTypes[mType];
}

void cUnBan::DisplayComplete(ostream &os)
{
	this->cBan::DisplayComplete(os);
	os << autosprintf(_("Removed: %s by %s because %s"), cTime(mDateUnban,0).AsDate().AsString().c_str(), mUnNickOp.c_str(), mUnReason.c_str()) << "\r\n";
}

void cBan::DisplayKick(ostream &os)
{
	os << setw(20) << setiosflags(ios::left);
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

void cBan::DisplayInline(ostream &os)
{
	os << " ";
	os << setw(30) << setiosflags(ios::left);
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
			os << convertByte(mShare,false);
		break;
		case eBF_PREFIX:
		default:
			os << mNick; // TODO: fix me
		break;
	}
	os << setw(25) << setiosflags(ios::left) << mNickOp;
	DisplayKick(os);
	os << GetBanType();
}
	}; // namespace nTables
}; // Namespace nVerliHub
