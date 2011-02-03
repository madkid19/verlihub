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
#include <iostream>
#include <string>
using namespace std;
#include "cdctag.h"
#include "cconntypes.h"
#include <string>
#include <iostream>
#include "cdcconf.h"
#include "i18n.h"

using std::string;
using namespace std;
using namespace nDirectConnect::nTables;
namespace nDirectConnect
{


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
		int slotToOpen = (mTotHubs / mServer->mC.tag_max_hs_ratio);
		if(slotToOpen > 0)
			os << " " << autosprintf(_("Open %d slots for %d hubs."), (int) (mTotHubs / mServer->mC.tag_max_hs_ratio), mTotHubs);
		code = eTC_MAX_HS_RATIO;
		return false;
	}
	
	if ( mLimit >= 0 ) {
		//Well, DCGUI bug!
		//if (tag->mClientType == eCT_DCGUI) limit *= slot;
		if( (conn_type->mTagMinLimit) > mLimit ) {
			os << autosprintf(_("Too low upload limit for your connection type (%s), max ratio is %f"), conn_type->mIdentifier.c_str(), conn_type->mTagMinLimit);
			code = eTC_MIN_LIMIT;
			return false;
		}
		if( (conn_type->mTagMinLSRatio *mSlots) > mLimit ) {
			os << autosprintf(_("Too upload limit for your connection type (%s), min upload limit is %f per every slot"), conn_type->mIdentifier.c_str(), conn_type->mTagMinLSRatio);
			code = eTC_MIN_LS_RATIO;
			return false;
		}
	}
	//TODO: Disable version checking if ver is -1
	if (client && client->mMinVersion > -1 && mClientVersion < client->mMinVersion ) {
		os << autosprintf(_("Your client version is too old, please upgrade it. Allowed minimum version number for %s client is: %f"), client->mName.c_str(), client->mMinVersion) << endl;
		code = eTC_MIN_VERSION;
		return false;
	}

	if (client && client->mMaxVersion > -1 && mClientVersion > client->mMaxVersion) {
		os << autosprintf(_("Your client version is too recent. Allowed maximum version number for %s client is %f"), client->mName.c_str(), client->mMaxVersion) << endl;
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

};

