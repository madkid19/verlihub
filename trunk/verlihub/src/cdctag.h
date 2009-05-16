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

#include <sstream>
#include "cpcre.h"

using namespace std;
// forward declarations
namespace nUtils { class cPCRE; };

namespace nDirectConnect
{
	namespace nTables { 
		class cDCConf;
		class cDCLang;
		class cConnType;
	};
	using namespace nDirectConnect::nTables;

/** a parser for the dc info tag
  *@author Daniel Muller
  */
class cDCTagParser
{
	public:
	// the constructor
	cDCTagParser();
	/** the global teg's regular expression */
	nUtils::cPCRE mTagRE;
	nUtils::cPCRE mModeRE;
	nUtils::cPCRE mHubsRE;
	nUtils::cPCRE mSlotsRE;
	nUtils::cPCRE mLimitRE;
};

/**dc info tag the <++ V:0.00,S:0,H:1> thing
  *@author Daniel Muller
  */

class cDCTag
{
public:
	cDCTag(const std::string &desc, cDCConf &, cDCLang &);
	~cDCTag();
	bool ValidateTag(ostream &os, cConnType *, int&);
	bool ParsePos(const std::string &desc);

	// tag parser static - one for all
	static cDCTagParser mParser;
	cDCConf &mC;
	cDCLang &mL;

        typedef enum{ eCT_NOTAG, eCT_PLUSPLUS, eCT_DCGUI, eCT_ODC, eCT_DC, eCT_DCPRO, eCT_STRONGDC, eCT_IDC, eCT_ZDC, eCT_APEXDC, eCT_ZION, eCT_UNKNOWN } tClientType;
	typedef enum{ eCM_NOTAG, eCM_ACTIVE, eCM_PASSIVE, eCM_SOCK5} tClientMode;

	enum {
		eTC_PARSE, //< tag parse error
		eTC_MAX_HUB, //< too many hubs
		eTC_MAX_SLOTS, //< too many slots
		eTC_MIN_SLOTS, //< too few slots
		eTC_MAX_HS_RATIO, //< hubs per slot
		eTC_MIN_LIMIT, //< small upload limiter
		eTC_MIN_LS_RATIO, //< small limiter per slot
		eTC_MIN_VERSION, //< version too old
		eTC_MAX_VERSION, //< version too recent
		eTC_SOCK5, //< unallowed sock5 mode
	};
	// tag's starting position in desc
	int mPositionInDesc;

	/** the client's type */
	tClientType mClientType;

	/** client's version */
	double mClientVersion;

	/** the client's mode */
	tClientMode mClientMode;

	/** the tag string */
	std::string mTag;
	/** the rest of tag after the Mode until the end*/
	std::string mTagBody;
	
	friend ostream &operator << (ostream&os, cDCTag &tag);

};

};
#endif
