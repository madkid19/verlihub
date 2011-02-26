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
#include "cdcclients.h"
#include "cconfigitembase.h"
#include "cserverdc.h"
#include "cpcre.h"
#include "i18n.h"

using nUtils::cPCRE;
namespace nDirectConnect {
  
	cDCTagParser cDCClients::mParser;

	cDCTagParser::cDCTagParser()
	{
		/**
			Sub-patterns
			0 - The whole tag
			1 - The whole prefix
			2 - Tag ID in prefix
			3 - Client version in prefix
			4 - Tag ID
			5 - Client version
			6 - The rest of the tag (called body)

		*/
	
		//General regex PREFIX VERSION<NAME V:version, M:mode, H:i/i/i, S:slot>
		//"(<?([A-Za-z\\+]*)[\\[ ]?([.0-9]*)\\]?>?)?<([\\+:A-Za-z]+) ?V\\:([^,]+),([^>]*)>"
		if(!mTagRE.Compile("(<([A-Za-z\\+]*) ?([.0-9]*)>)?<([\\+:A-Za-z]+) ?V\\:([^,]+),([^>]*)>"))
			throw "Error in tag general REGEX";
		/**
			Sub-patterns
			0 - The whole mode chunck
			1 - Client mode
			2 - reg hubs
			3 - op hubs
		*/
		if(!mModeRE.Compile("M\\:(A|P|5)"))
			throw "Error in hubs general REGEX";
		/**
			Sub-patterns
			0 - The whole hubs chunck
			1 - Guest hubs
			2 - Reg hubs
			3 - OP hubs
		*/
		if(!mHubsRE.Compile("H\\:(\\d+)(\\/\\d+)?(\\/\\d+)?"))
			throw "Error in hubs general REGEX";
		/**
			Sub-patterns
			0 - The whole chunck
			1 - Slots
		*/
		if(!mSlotsRE.Compile("S\\:(\\d+)"))
			throw "Error in slots general REGEX";
		/**
			Sub-patterns
			0 - The whole chunck
			1 - Info
			2 - Value
		*/
		if(!mLimitRE.Compile("(B\\:|L\\:|F\\:\\d+\\/)(\\d+(\\.\\d)?)"))
			throw "Error in limiter general REGEX";
	}
	namespace nTables {

	cDCClients::cDCClients( cServerDC *server ) :
		tMySQLMemoryList<cDCClient, cServerDC>(server->mMySQL, server, "dc_clients"), mServer(server)
	{
		SetClassName("nDC::cDCClients");
	}
	
	void cDCClients::AddFields()
	{
		AddCol("name", "varchar(125)", "", false, mModel.mName);
		AddPrimaryKey("name");
		AddCol("tag_id", "varchar(125)", "", false, mModel.mTagID);
		AddCol("min_version", "decimal(4,4)", "-1", false, mModel.mMinVersion);
		AddCol("max_version", "decimal(4,4)", "-1", false, mModel.mMaxVersion);
		AddCol("ban", "tinyint(1)", "0", false, mModel.mBan);
		AddCol("enable", "tinyint(1)", "1", false, mModel.mEnable);
		mMySQLTable.mExtra = "PRIMARY KEY(name)";
		SetBaseTo(&mModel);
	}
	
	cDCClient* cDCClients::FindTag(const string &tagID)
	{
		iterator it;
		cDCClient *client;
		for (it= begin(); it != end(); ++it) {
			client = *it;
			if(client->mEnable && client->mTagID == tagID)
				return client;
		}
		// Unknwon client
		return NULL;
	}

	bool cDCClients::CompareDataKey(const cDCClient &D1, const cDCClient &D2)
	{
		return (D1.mName == D2.mName);
	}
	
	bool cDCClients::ParsePos(const string &desc)
	{
		mPositionInDesc = -1;

		if(mParser.mTagRE.Exec(desc) >= 0)
			mPositionInDesc = mParser.mTagRE.StartOf(0);
		return mPositionInDesc > -1;
	}
	
	cDCTag* cDCClients::ParseTag(const string &desc)
	{
		string str;
		cDCTag *tag = new cDCTag(mServer);
		/**
			Sub-patterns
			0 - The whole tag
			1 - The whole prefix
			2 - Tag ID in prefix
			3 - Client version in prefix
			4 - Tag ID
			5 - Client version
			6 - The rest of the tag (called body)

		*/
	
		enum { eTP_COMPLETE, eTP_PREFIX, eTP_PTAGID, eTP_PVERSION, eTP_TAGID, eTP_VERSION, eTP_BODY};
		//TODO: Detect invalid tag
		//tag->mClientType = eCT_NOTAG;
		tag->mClientMode = eCM_NOTAG;
		mPositionInDesc = -1;
		string version("0");
		if(mParser.mTagRE.Exec(desc) >= 0) {
			// copy tag's parts
			mPositionInDesc = mParser.mTagRE.StartOf(eTP_COMPLETE);
			mParser.mTagRE.Extract(eTP_COMPLETE, desc, tag->mTag);
			mParser.mTagRE.Extract(eTP_BODY, desc, tag->mTagBody);
			// determine client's type
			mParser.mTagRE.Extract(eTP_TAGID,desc,tag->mTagID);
			mParser.mTagRE.Extract(eTP_VERSION,desc, version);
			if (mParser.mTagRE.PartFound(eTP_PREFIX))
			{
			  string tmp;
			  mParser.mTagRE.Extract(eTP_PREFIX, desc, tmp);
				if ((mParser.mTagRE.PartFound(eTP_PVERSION)) && (mParser.mTagRE.PartFound(eTP_PTAGID))) {
				  mParser.mTagRE.Extract(eTP_PVERSION, desc, tmp);
					mParser.mTagRE.Extract(eTP_PTAGID, desc, tag->mTagID);
					mParser.mTagRE.Extract(eTP_PVERSION, desc, tag->mTagID);
				}
			}
		}
		//TODO: Use FindData
		tag->client = FindTag(tag->mTagID);
		// Detect client mode
		if(mParser.mModeRE.Exec(desc) >= 0) {
			mParser.mModeRE.Extract(1,desc,str);
			if( str == "A" ) tag->mClientMode = eCM_ACTIVE;
			else if (str == "P") tag->mClientMode = eCM_PASSIVE;
			else if (str == "5") tag->mClientMode = eCM_SOCK5;
		}

		istringstream is(version);
		is >> tag->mClientVersion;
		is.clear();
		
		int hubs = -1,tmp;
		char c;
		// Number of hubs
		if(mParser.mHubsRE.Exec( tag->mTagBody ) >= 2) {
			// Hubs where the user is guest
			mParser.mHubsRE.Extract(1,tag->mTagBody,str);
			is.str(str);
			is >> hubs;
			is.clear();
			// Hubs where the user is regged
			if (mParser.mHubsRE.PartFound(2) && mServer->mC.tag_sum_hubs >=2) {
				tmp = 0;
				mParser.mHubsRE.Extract(2,tag->mTagBody,str);
				is.str(str);
				is >> c >> tmp;
				is.clear();
				hubs += tmp;
			}
			// Hubs where the user is operator
			if (mParser.mHubsRE.PartFound(3)  && mServer->mC.tag_sum_hubs >=3) {
				tmp = 0;
				mParser.mHubsRE.Extract(3,tag->mTagBody,str);
				is.str(str);
				is >> c >> tmp;
				is.clear();
				hubs += tmp;
			}
			tag->mTotHubs = hubs;
		}
		// Open slot
		if(mParser.mSlotsRE.Exec( tag->mTagBody ) >= 2) {
			mParser.mSlotsRE.Extract(1,tag->mTagBody,str);
			is.str(str);
			is >> tag->mSlots;
			is.clear();
		}
		// Limit
		if(mParser.mLimitRE.Exec( tag->mTagBody ) >= 2) {
			mParser.mLimitRE.Extract(2,tag->mTagBody,str);
			is.str(str);
			is >> tag->mLimit;
			is.clear();
		}
		return tag;
	}

	cDCClientConsole::cDCClientConsole(cDCConsole *console) : tDCClientConsoleBase(console)
	{
		this->AddCommands();
	}

	cDCClientConsole::~cDCClientConsole()
	{}

	void cDCClientConsole::GetHelpForCommand(int cmd, ostream &os)
	{
		string help_str;
		switch(cmd)
		{
			case eLC_LST: 
				help_str = "!lstclient\r\nGive the list of clients"; 
				break;
			case eLC_ADD: 
			case eLC_MOD:
				help_str = "!(add|mod)client <name>"
						" -t <client_id>"
						"[ -b <\"yes/no\">]"
						"[ -v <\"min_version\">]"
						"[ -V <\"max_version\">]"
						"[ -e <enable/disable>]";
				break;
			case eLC_DEL:
				help_str = "!delclient <name>"; break;
				default: break;
		}
		cDCProto::EscapeChars(help_str,help_str);
		os << help_str;
	}
	
	void cDCClientConsole::GetHelp(ostream &os)
	{
		string help;
		help = "Available options are:\r\n";
		help += "-t \tClient identification (for ex. <++ V:0.75,M:A,H:1/0/0,S:2> client ID is '++')\r\n";
		help += "-b \tBan client matching this rule (possible values are 0 or 1)\r\n";
		help += "-v \tMinimum version number\r\n";
		help += "-V \tMaximum version number\r\n";
		help += "-e \tEnable or disable this rule (possible values are 0 or 1)\r\n";
		
		cDCProto::EscapeChars(help,help);
		os << help;
	}
	
	const char * cDCClientConsole::GetParamsRegex(int cmd)
	{
		switch(cmd)
		{
			case eLC_ADD:
			case eLC_MOD:
				return "^(\\S+)("
						"( -t ?(-?\\S+))?|" //[ -t<tag_id>]
						"( -b ?(-?\\d))?|" //[ -b<yes/no>]
						"( -v ?(-?[1-9]{0,1}[0-9]{1}(?:\\.[0-9]{0,3})?))?|" //[ -v<min_version>]
						"( -V ?(-?[1-9]{0,1}[0-9]{1}(?:\\.[0-9]{0,3})?))?|" //[ -V<max_version>]
						"( -e ?(-?\\d))?|" // [ -e<1/0>]
						")*\\s*$"; // the end of message
			case eLC_DEL:
				return "(\\S+)";
				default: return "";break;
		};
	}

	bool cDCClientConsole::ReadDataFromCmd(cfBase *cmd, int CmdID, cDCClient &data)
	{
		enum {
			eDATA_ALL, 
   			eDATA_NAME, eDATA_CHOICE,
   			eDATA_TAGIDp, eDATA_TAGID,
			eDATA_CLIENTBANNEDp, eDATA_CLIENTBANNED,
			eDATA_MINVp, eDATA_MINV,
			eDATA_MAXVp, eDATA_MAXV,
			eDATA_ENABLEp, eDATA_ENABLE };
		cmd->GetParStr(eDATA_NAME,data.mName);
		cmd->GetParStr(eDATA_TAGID,data.mTagID);
		if(CmdID == eLC_ADD && data.mTagID.empty()) {
			//os << _("Tag ID for a client cannot be empty");
			return false;
		}
		cmd->GetParBool(eDATA_CLIENTBANNED,data.mBan);
		cmd->GetParInt(eDATA_ENABLE, data.mEnable);
		cmd->GetParDouble(eDATA_MINV, data.mMinVersion);
		cmd->GetParDouble(eDATA_MAXV, data.mMaxVersion);
		return true;
	}
		
	cDCClients *cDCClientConsole::GetTheList()
	{
		return mOwner->mDCClients;
	}

	const char *cDCClientConsole::CmdSuffix(){ return "client";}
	const char *cDCClientConsole::CmdPrefix(){ return "!";}

	void cDCClientConsole::ListHead(ostream *os)
	{
		*os << _("Existing clients are:") << "\r\n";
	}
		
	bool cDCClientConsole::IsConnAllowed(cConnDC *conn,int cmd)
	{
		return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
	}
	};
};

 
