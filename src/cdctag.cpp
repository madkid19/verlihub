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
#include <iostream>
#include <string>
#include "cdctag.h"
#include "cconntypes.h"
#include <string>
#include <iostream>
#include "cdcconf.h"
#include "i18n.h"

using namespace std;
namespace nVerliHub {
	using namespace nTables;
cDCTag::cDCTag(cServerDC *mS, cDCClient *c) : mServer(mS), client(c)
{
	mTotHubs = -1;
	mSlots = -1;
	mLimit = -1;
}

cDCTag::cDCTag(cServerDC *mS) : mServer(mS)
{
	mTotHubs = -1;
	mSlots = -1;
	mLimit = -1;
}

cDCTag::~cDCTag() { }

bool cDCTag::ValidateTag(ostream &os, cConnType *conn_type, int &code)
{

	if(client && client->mBan) {
		os << _("<<Your client is banned>>");
		code = eTC_BANNED;
		return false;
	}

	//not parsed tag, unknown tag
	if ((mClientMode == eCM_SOCK5) && !mServer->mC.tag_allow_sock5) {
		os << _("Connections through proxy server are not allowed in this hub.");
		code = eTC_SOCK5;
		return false;
	}

	if ((mClientMode == eCM_PASSIVE) && !mServer->mC.tag_allow_passive) {
		os << _("Passive connections are restricted. Consider changing to active.");
		code = eTC_PASSIVE;
		return false;
	}

	if((mTotHubs < 0) || (mSlots < 0)) {
		os << _("Error: your client tag is reporting less then 0 hubs or slots!");
		code = eTC_PARSE;
		return false;
	}

	string MsgToUser;

	if(!mServer->mC.tag_allow_unknown && !client) {

		os << _("Unknown clients are not allowed in this hub");
		code = eTC_UNKNOWN;
		return false;
	}

	if(mTotHubs > mServer->mC.tag_max_hubs) {
		os << autosprintf(_("Too many open hubs, max is %d"), mServer->mC.tag_max_hubs);
		code = eTC_MAX_HUB;
		return false;
	}

	if(mSlots < conn_type->mTagMinSlots) {
		os << autosprintf(_("Too little open slots for your connection type (%s), min is %d"), conn_type->mIdentifier.c_str(), conn_type->mTagMinSlots);
		code = eTC_MIN_SLOTS;
		return false;
	}

	if(mSlots > conn_type->mTagMaxSlots) {
		os << autosprintf(_("Too many open slots for your connection type (%s), max is %d"), conn_type->mIdentifier.c_str(), conn_type->mTagMaxSlots);
		code = eTC_MAX_SLOTS;
		return false;
	}

	if( (mServer->mC.tag_max_hs_ratio * mSlots) < mTotHubs ) {
		os << autosprintf(_("Your hubs/slots ratio %.2f is too high (max is %.2f)."), (double) mTotHubs/mSlots, mServer->mC.tag_max_hs_ratio);
		int slotToOpen = (int) (mTotHubs / mServer->mC.tag_max_hs_ratio);
		if(slotToOpen > 0)
			os << " " << autosprintf(_("Open %d slots for %d hubs."), (int) (mTotHubs / mServer->mC.tag_max_hs_ratio), mTotHubs);
		code = eTC_MAX_HS_RATIO;
		return false;
	}

	if (mLimit >= 0) {
		//Well, DCGUI bug!
		//if (tag->mClientType == eCT_DCGUI) limit *= slot;
		if( (conn_type->mTagMinLimit) > mLimit ) {
			os << autosprintf(_("Too low upload limit for your connection type (%s), minimum upload rate is %.2f"), conn_type->mIdentifier.c_str(), conn_type->mTagMinLimit);
			code = eTC_MIN_LIMIT;
			return false;
		}
		if( (conn_type->mTagMinLSRatio *mSlots) > mLimit ) {
			os << autosprintf(_("Too low upload limit per slot for your connection type (%s), minimum upload limit is %.2f per slot"), conn_type->mIdentifier.c_str(), conn_type->mTagMinLSRatio);
			code = eTC_MIN_LS_RATIO;
			return false;
		}
	}

	// Use tag_min_version and tag_max_version for unknown client or use the version number in the matching rule
	double minVersion = mServer->mC.tag_min_version, maxVersion = mServer->mC.tag_max_version;

	if(client) {
		minVersion = client->mMinVersion;
		maxVersion = client->mMaxVersion;
	}
	if(minVersion != -1 && mClientVersion < minVersion) {
		os << _("Your client version is too old, please upgrade it.") << " ";
		if(client)
			os << autosprintf(_("Allowed minimum version number for %s client is: %.2f"), client->mName.c_str(), minVersion) << endl;
		else
			os << autosprintf(_("Allowed minimum version number for your client is: %.2f"), minVersion) << endl;
		code = eTC_MIN_VERSION;
		return false;
	}
	if(maxVersion != -1 && mClientVersion < maxVersion) {
		os << _("Your client version is too recent.") << " ";
		if(client)
				os << autosprintf(_("Allowed maximum version number for %s client is %.2f"), client->mName.c_str(), maxVersion) << endl;
		else
			os << autosprintf(_("Allowed maximum version number for your client is %.2f"), maxVersion) << endl;
		code = eTC_MAX_VERSION;
		return false;
	}

	return true;
}

ostream &operator << (ostream &os, cDCTag &tag)
{
	os << "Client " << (tag.client ? tag.client->mName : "Unknown") << " (v." << tag.mClientVersion << ")" << endl;
	os << "[::] Mode: ";
	if(tag.mClientMode == eCM_ACTIVE)
		os << "Active";
	else if(tag.mClientMode == eCM_PASSIVE)
		os << "Passive";
	else if(tag.mClientMode == eCM_SOCK5)
		os << "Socks";
	os << endl;
	os << "[::] Open hubs: ";
	if(tag.mTotHubs >=0)
		os << tag.mTotHubs;
	else
		os <<"Not available";
	os << endl;
	os << "[::] Open slots: ";
	if(tag.mSlots >=0)
		os << tag.mSlots;
	else
		os << "Not available";
	os << endl;
	return os;
}

}; // namespace nVerliHub
