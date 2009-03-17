/***************************************************************************
                          cdctag.cpp  -  description
                             -------------------
    begin                : Sun Aug 24 2003
    copyright            : (C) 2003 by Daniel Muller
    email                : dan at verliba dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cdctag.h"
#include "cconntypes.h"
#include <string>
#include "cpcre.h"
#include <iostream>
#include "cdcconf.h"

using std::string;
using namespace std;
using nUtils::cPCRE;
using namespace nDirectConnect::nTables;
namespace nDirectConnect
{

cDCTagParser cDCTag::mParser;

cDCTagParser::cDCTagParser()
{
	/**
		the order of subpatterns
		0 - the whole tag
		1 - ++ or DCGUI
		2 - the version number
		3 - mode
		4 - the rest (call it body)

	*/
//<StrgDC++ V:1.00 RC3,M:A,H:0/3/2,S:2>
//<DC:PRO V:0.2.3.0,M:A,H:1/0/0,S:4>

	if(!mTagRE.Compile("((<iDC|zDC\\+\\+\\[|o)([\\d\\.]+)?(>|\\])?)?<(\\+\\+|DCGUI|oDC|Z\\+\\+|ApexDC\\+\\+|DC|DC\\:PRO|TK|StrgDC\\+\\+) ?V\\:([^,]+),([^>]*)>"))
		throw "Error in tag general REGEX";
	/**
		0 - whole Hubs part
		1 - norm user hubs
		2 - reg hubs
		3 - op hubs
	*/
	if(!mModeRE.Compile("M\\:(A|P|5)"))
		throw "Error in hubs general REGEX";
	if(!mHubsRE.Compile("H\\:(\\d+)(\\/\\d+)?(\\/\\d+)?"))
		throw "Error in hubs general REGEX";
	if(!mSlotsRE.Compile("S\\:(\\d+)"))
		throw "Error in slots general REGEX";
	if(!mLimitRE.Compile("(B\\:|L\\:|F\\:\\d+\\/)(\\d+(\\.\\d)?)"))
		throw "Error in limiter general REGEX";
}

cDCTag::cDCTag(const std::string &desc, cDCConf &config, cDCLang &lang) : mC(config), mL(lang)
{
	string str;

	enum { eTP_COMPLETE, eTP_PREFIX, eTP_PRE_PREFIX, eTP_PVERSION, eTP_POST_PREFIX, eTP_TYPE, eTP_VERSION, eTP_BODY};
	mClientType = eCT_NOTAG;
	mClientMode = eCM_NOTAG;
	mPositionInDesc = -1;

	if(mParser.mTagRE.Exec(desc) >= 0)
	{
		// copy tag's parts
		mPositionInDesc = mParser.mTagRE.StartOf(eTP_COMPLETE);
		mParser.mTagRE.Extract(eTP_COMPLETE, desc, mTag);
		mParser.mTagRE.Extract(eTP_BODY, desc, mTagBody);
		// determine client's type
		mParser.mTagRE.Extract(eTP_TYPE,desc,str);
                if( str == "++" ) mClientType = eCT_PLUSPLUS;
                else if (str == "DCGUI") mClientType = eCT_DCGUI;
                else if (str == "oDC") mClientType = eCT_ODC;
                else if (str == "DC") mClientType = eCT_DC;
                else if (str == "DC:PRO") mClientType = eCT_DCPRO;
                else if (str == "StrgDC++") mClientType = eCT_STRONGDC;
                else if (str == "ApexDC++") mClientType = eCT_APEXDC;
		else if (str == "Z++") mClientType = eCT_ZION;
                else if (str.size() ) mClientType = eCT_UNKNOWN;
                if (mParser.mTagRE.PartFound(eTP_PRE_PREFIX))
                {
			mParser.mTagRE.Extract(eTP_PRE_PREFIX, desc, str);
			if ((str == "<iDC") &&
			   (mParser.mTagRE.PartFound(eTP_PVERSION)) &&
			   (mParser.mTagRE.PartFound(eTP_POST_PREFIX))) mClientType = eCT_IDC;
			   
			else if ((str == "zDC++[") &&
			        (mParser.mTagRE.PartFound(eTP_PVERSION)) &&
				(mParser.mTagRE.PartFound(eTP_POST_PREFIX))) mClientType = eCT_ZDC;
				
			else if ((str == "o") && (mParser.mTagRE.PartFound(eTP_PVERSION))) mClientType = eCT_ODC;
		}
	}

	if(mParser.mModeRE.Exec(desc) >= 0)
	{
		mParser.mModeRE.Extract(1,desc,str);
		if( str == "A" ) mClientMode = eCM_ACTIVE;
		else if (str == "P") mClientMode = eCM_PASSIVE;
		else if (str == "5") mClientMode = eCM_SOCK5;
	}

	string version;
	mParser.mTagRE.Extract(eTP_VERSION,desc, version);
	if ((mParser.mTagRE.PartFound(eTP_PVERSION)) && (mParser.mTagRE.PartFound(eTP_PRE_PREFIX)))
	{
		if(mClientType == eCT_IDC || mClientType == eCT_ZDC)
		{
			//iDC and zDC must have POST_PREFIX part
			if(mParser.mTagRE.PartFound(eTP_POST_PREFIX))
			mParser.mTagRE.Extract(eTP_PVERSION, desc, version);
		}
		else if(mClientType == eCT_ODC) //oDC doesn't have the POST_PREFIX part
			mParser.mTagRE.Extract(eTP_PVERSION, desc, version);
	}
	istringstream is(version);
	is >> mClientVersion;
}

cDCTag::~cDCTag(){
}

bool cDCTag::ParsePos(const std::string &desc)
{
	mPositionInDesc = -1;

	if(mParser.mTagRE.Exec(desc) >= 0)
		mPositionInDesc = mParser.mTagRE.StartOf(0);
	return mPositionInDesc > -1;
}

/*!
    \fn cDCTag::ValidateTag(ostream &os)
 */
bool cDCTag::ValidateTag(ostream &os, cConnType *conn_type, int &code)
{
	istringstream *is;
	string str, MsgToUser;
	int hub=-1,slot=-1, tmp;
	char c;
	double limit=-1.0;

	if(mPositionInDesc == -1)
	{
		return true;
	}
	
	if ((mClientMode == eCM_SOCK5) && !mC.tag_allow_sock5 )
	{
		os << mL.tag_no_sock;
		code = eTC_SOCK5;
		return false;
	}

	if(mParser.mHubsRE.Exec( mTagBody ) >= 2)
	{
		mParser.mHubsRE.Extract(1,mTagBody,str);
		is = new istringstream(str);
		*is >> hub;
		delete is;
		if (mParser.mHubsRE.PartFound(2) && (mC.tag_sum_hubs >= 2) )
		{
			tmp = 0;
			mParser.mHubsRE.Extract(2,mTagBody,str);
			is = new istringstream(str);
			*is >> c >> tmp;
			delete is;
			hub += tmp;
		}
		if (mParser.mHubsRE.PartFound(3) && (mC.tag_sum_hubs >= 3))
		{
			tmp = 0;
			mParser.mHubsRE.Extract(3,mTagBody,str);
			is = new istringstream(str);
			*is >> c >> tmp;
			delete is;
			hub += tmp;
		}
	}

	if(mParser.mSlotsRE.Exec( mTagBody ) >= 2)
	{
		mParser.mSlotsRE.Extract(1,mTagBody,str);
		is = new istringstream(str);
		*is >> slot;
		delete is;
	}

	if(mParser.mLimitRE.Exec( mTagBody ) >= 2)
	{
		mParser.mLimitRE.Extract(2,mTagBody,str);
		is = new istringstream(str);
		*is >> limit;
		delete is;
	}

	if((hub < 0) || (slot < 0))
	{
		os << mL.wrong_dc_tag;
		code = eTC_PARSE;
		return false;
	}
	if(hub > mC.tag_max_hubs)
	{
		ReplaceVarInString(mL.tag_max_hubs, "tag_max_hubs", MsgToUser,mC.tag_max_hubs);
		os << MsgToUser;
		code = eTC_MAX_HUB;
		return false;
	}
	if(slot > conn_type->mTagMaxSlots)
	{
		ReplaceVarInString(mL.tag_max_slots, "conn_type", MsgToUser, conn_type->mIdentifier);
		ReplaceVarInString(MsgToUser, "tag_max_slots", MsgToUser,  conn_type->mTagMaxSlots);
		os << MsgToUser;
		code = eTC_MAX_SLOTS;
		return false;
	}
	if(slot < conn_type->mTagMinSlots)
	{
		ReplaceVarInString(mL.tag_min_slots, "conn_type", MsgToUser, conn_type->mIdentifier);
		ReplaceVarInString(MsgToUser, "tag_min_slots", MsgToUser, conn_type->mTagMinSlots);
		os << MsgToUser;
		code = eTC_MIN_SLOTS;
		return false;
	}
	if( (mC.tag_max_hs_ratio * slot) < hub )
	{
		ReplaceVarInString(mL.tag_max_hs_ratio, "tag_max_hs_ratio", MsgToUser, mC.tag_max_hs_ratio);
		ReplaceVarInString(MsgToUser, "hubs", MsgToUser, hub); 
		ReplaceVarInString(MsgToUser, "max_hubs", MsgToUser, slot * mC.tag_max_hs_ratio); 
		if (mC.tag_max_hs_ratio != 0)
			ReplaceVarInString(MsgToUser, "min_slots", MsgToUser, hub / mC.tag_max_hs_ratio); 
		if(mC.tag_max_hs_ratio != 0)
			ReplaceVarInString(MsgToUser, "tag_min_sh_ratio", MsgToUser, 1/mC.tag_max_hs_ratio);
		os << MsgToUser;
		code = eTC_MAX_HS_RATIO;
		return false;
	}
	if ( limit >= 0 )
	{
		if (mClientType == eCT_DCGUI) limit *= slot;
		if( (conn_type->mTagMinLimit) > limit )
		{
			ReplaceVarInString(mL.tag_min_limit, "conn_type", MsgToUser, conn_type->mIdentifier);
			ReplaceVarInString(MsgToUser, "tag_min_limit", MsgToUser, conn_type->mTagMinLimit);
			os << MsgToUser;
			code = eTC_MIN_LIMIT;
			return false;
		}
		if( (conn_type->mTagMinLSRatio *slot) > limit )
		{
			ReplaceVarInString(mL.tag_min_ls_ratio, "conn_type", MsgToUser,	conn_type->mIdentifier);
			ReplaceVarInString(MsgToUser, "tag_min_ls_ratio", MsgToUser, conn_type->mTagMinLSRatio);
			os << MsgToUser;
			code = eTC_MIN_LS_RATIO;
			return false;
		}
	}

	if ((mClientType > eCT_NOTAG) && (mClientType < eCT_UNKNOWN))
	{
	        string Client;
                switch(mClientType)
                {
                        case cDCTag::eCT_PLUSPLUS: Client = "DC++"; break;
                        case cDCTag::eCT_DCGUI: Client = "DCGUI"; break;
                        case cDCTag::eCT_ODC: Client = "oDC"; break;
                        case cDCTag::eCT_DC: Client = "DC"; break;
                        case cDCTag::eCT_DCPRO: Client = "DCPRO"; break;
                        case cDCTag::eCT_STRONGDC: Client = "StrongDC++"; break;
                        case cDCTag::eCT_IDC: Client = "iDC"; break; 
                        case cDCTag::eCT_ZDC: Client = "zDC++"; break;
                        case cDCTag::eCT_APEXDC: Client = "ApexDC++"; break;
			case cDCTag::eCT_ZION: Client = "Z++"; break;
                        default: Client = "Unknown"; break;
                }

		if (mC.tag_min_version[mClientType] > (mClientVersion+0.00001))
		{
			ReplaceVarInString(mL.msg_upgrade, "msg_upgrade", MsgToUser, mC.msg_upgrade);
			ReplaceVarInString(MsgToUser, "client_type", MsgToUser, Client);
			ReplaceVarInString(MsgToUser, "tag_min_version", MsgToUser, mC.tag_min_version[mClientType]);
			os << MsgToUser << endl;
			code = eTC_MIN_VERSION;
			return false;
		}

		if (mC.tag_max_version[mClientType] < (mClientVersion-0.00001))
		{
			ReplaceVarInString(mL.msg_downgrade, "msg_downgrade", MsgToUser, mC.msg_downgrade);
			ReplaceVarInString(MsgToUser, "client_type", MsgToUser, Client);
			ReplaceVarInString(MsgToUser, "tag_max_version", MsgToUser, mC.tag_max_version[mClientType]);
			os << MsgToUser << endl;
			code = eTC_MAX_VERSION;
			return false;
		}
	}
 
	return true;
}

ostream &operator << (ostream &os, cDCTag &tag)
{
	os << tag.mTag;
	return os;
}

};

