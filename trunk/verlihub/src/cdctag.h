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
#ifndef CDCTAG_H
#define CDCTAG_H
#include <string>
#include "cdcclient.h"

using namespace std;
// forward declarations
//namespace nUtils { class cPCRE; };

namespace nDirectConnect
{
	namespace nEnums {
	      typedef enum{ eCM_NOTAG, eCM_ACTIVE, eCM_PASSIVE, eCM_SOCK5} tClientMode;
	      
		// Validate tag error code
	        enum {
			eTC_BANNED, // Banned client
			eTC_UNKNOWN, // Unkown client
			eTC_PARSE, // Tag parse error
			eTC_MAX_HUB, // Too many hubs
			eTC_MAX_SLOTS, // Too many slots
			eTC_MIN_SLOTS, //< Too few slots
			eTC_MAX_HS_RATIO, // Hubs per slot
			eTC_MIN_LIMIT, // Small upload limiter
			eTC_MIN_LS_RATIO, // Small limiter per slot
			eTC_MIN_VERSION, // Version too old
			eTC_MAX_VERSION, // Version too recent
			eTC_SOCK5, // Not allowed sock5
			eTC_PASSIVE, // Restrict passive connections
		};
	}
	namespace nTables {
		class cDCConf;
		class cDCLang;
		class cConnType;
	};
	using namespace nDirectConnect::nTables;
	using namespace nDirectConnect::nEnums;

/** a parser for the dc info tag
  *@author Daniel Muller
  */
/*class cDCTagParser
{
	public:
	// the constructor
	cDCTagParser();
	/** the global teg's regular expression *
	nUtils::cPCRE mTagRE;
	nUtils::cPCRE mModeRE;
	nUtils::cPCRE mHubsRE;
	nUtils::cPCRE mSlotsRE;
	nUtils::cPCRE mLimitRE;
};*/

/**dc info tag the <++ V:0.00,S:0,H:1> thing
  *@author Daniel Muller
  */

class cDCTag
{
public:
	cDCTag(cServerDC *mS);
	cDCTag(cServerDC *mS, cDCClient *c);
	
	~cDCTag();
	
	cServerDC *mServer;
	
	cDCClient *client;
	
	/** the parsed tag if available **/
	
	string mTag;
	
	/** The name of the client **/
	
	//string mClientName;
	
	/** The chunck in the tag that identify the client **/
	
	string mTagID;


	/** The client version */
	
	double mClientVersion;
	
	/** The number of the hubs connected to **/
	
	int mTotHubs;
	
	/** The number of open slots **/
	
	int mSlots;
	
	/** Limit **/
	
	int mLimit;

	/** The mode of the client (active, passive or socket) **/
	
	tClientMode mClientMode;
	
	/** The rest of tag (after the mode part to the end) **/
	
	string mTagBody;
	
	bool ValidateTag(ostream &os, cConnType *conn_type, int &code);
	
	friend ostream &operator << (ostream&os, cDCTag &tag);

};

};
#endif
