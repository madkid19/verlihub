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

#include "ccustomredirects.h"
#include "cconfigitembase.h"
#include "cserverdc.h"

namespace nDirectConnect {
	namespace nTables {

	cRedirects::cRedirects( cServerDC *server ) :
		tMySQLMemoryList<cRedirect, cServerDC>(server->mMySQL, server, "custom_redirects")
	{
		SetClassName("nDC::cRedirects");
	}

	void cRedirects::AddFields()
	{
		AddCol("address", "varchar(125)", "", false, mModel.mAddress);
		AddPrimaryKey("address");
		AddCol("flag", "tinyint(2)", "", false, mModel.mFlag);
		AddCol("enable", "tinyint(1)", "1", true, mModel.mEnable);
		mMySQLTable.mExtra = "PRIMARY KEY(address)";
		SetBaseTo(&mModel);
	}
	
	int cRedirects::MapTo(int Type)
	{
		switch(Type) {
			case eCR_INVALID_USER:
			case eCR_KICKED:
				return eKick;	
			break;
			case eCR_USERLIMIT:
				return eUserLimit;
			break;
			case eCR_SHARE_LIMIT:
				return eShareLimit;
			break;
			case eCR_TAG_INVALID:
			case eCR_TAG_NONE:
				return eTag;
			break;
			case eCR_PASSWORD:	
				return eWrongPasswd;
			break;
			case eCR_INVALID_KEY:
				return eInvalidKey;
			break;
			default: return 0;
		}
	}
	
	/**
	
	Find a redirect address from a given type ID. If there is more than one address in the list...
	
	@param[in,out] os The stream where to store the description.
	@param[in,out] tr The cRedirect object that describes the redirect
	@return The stream
	*/
	
	char *cRedirects::MatchByType(int Type)
	{
		iterator it;
		cRedirect *redirect;
		char *redirects[10];
		char *DefaultRedirect[10];
		int i = 0, j = 0, iType = MapTo(Type);
		for (it= begin(); it != end(); ++it)
		{
			if(i >= 10) break;
			redirect = *it;
			if (redirect->mEnable && (redirect->mFlag & iType)) {
				redirects[i] = (char *) redirect->mAddress.c_str();
				i++;
			}
			if(redirect->mEnable && !redirect->mFlag && j < 10) { DefaultRedirect[j] = (char *) redirect->mAddress.c_str(); j++;}
		}
		if(!i) {
			// use default redirect	
			if(!j) return "";
			Random(j);
			return DefaultRedirect[j];
		}
		Random(i);
		return redirects[i];
	}
	
	void cRedirects::Random(int &key)
	{
		srand (time(NULL));
		int temp = (int) (1.0 * key * rand()/(RAND_MAX+1.0));
		if(temp < key) key = temp;
		else key -= 1;
	}

	bool cRedirects::CompareDataKey(const cRedirect &D1, const cRedirect &D2)
	{
		return (D1.mAddress == D2.mAddress);
	}

	cRedirectConsole::cRedirectConsole(cDCConsole *console) : tRedirectConsoleBase(console)
	{
		this->AddCommands();
	}

	cRedirectConsole::~cRedirectConsole()
	{}

	void cRedirectConsole::GetHelpForCommand(int cmd, ostream &os)
	{
		string help_str;
		switch(cmd)
		{
			case eLC_LST: 
				help_str = "!lstredirect\r\nGive the list of redirect"; 
				break;
			case eLC_ADD: 
			case eLC_MOD:
				help_str = "!(add|mod)redirect <address>"
						"[ -f <\"redirect flag\">]"
						"[ -e <enable/disable>]";
				break;
			case eLC_DEL:
				help_str = "!delredirect <address>"; break;
				default: break;
		}
		cDCProto::EscapeChars(help_str,help_str);
		os << help_str;
	}
	
	void cRedirectConsole::GetHelp(ostream &os)
	{
		string help;
		help = "Available redirect flags are:\r\n";
		help += "0  \tFor any other reason\r\n";
		help += "1  \tBan and kick\r\n";
		help += "2  \tHub full\r\n";
		help += "4  \tUser does not enforce share rules\r\n";
		help += "8  \tWrong or unknown tag\r\n";
		help += "16\tWrong password\r\n";
		
		help += "\r\nRemember to make the sum of the selected flags above\r\n";
		cDCProto::EscapeChars(help,help);
		os << help;
	}
	
	const char * cRedirectConsole::GetParamsRegex(int cmd)
	{
		switch(cmd)
		{
			case eLC_ADD:
			case eLC_MOD:
				return "^(\\S+)("
						"( -f ?(-?\\d+))?|" //[ -f<flag>]
						"( -e ?(-?\\d))?|" // [ -e<1/0>]
						")*\\s*$"; // the end of message
			case eLC_DEL:
				return "(\\S+)";
				default: return "";break;
		};
	}

	bool cRedirectConsole::ReadDataFromCmd(cfBase *cmd, int CmdID, cRedirect &data)
	{
		enum {
			eADD_ALL, 
   			eADD_ADDRESS, eADD_CHOICE,
   			eADD_FLAGp, eADD_FLAG,
			eADD_ENABLEp, eADD_ENABLE };
		cmd->GetParStr(eADD_ADDRESS,data.mAddress);
		cmd->GetParInt(eADD_FLAG,data.mFlag);
		cmd->GetParInt(eADD_ENABLE, data.mEnable);
		return true;
	}
		
	cRedirects *cRedirectConsole::GetTheList()
	{
		return mOwner->mRedirects;
	}

	const char *cRedirectConsole::CmdSuffix(){ return "redirect";}
	const char *cRedirectConsole::CmdPrefix(){ return "!";}

	void cRedirectConsole::ListHead(ostream *os)
	{
		*os << "Existing redirects are:\r\n";
	}
		
	bool cRedirectConsole::IsConnAllowed(cConnDC *conn,int cmd)
	{
		return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
	}
	};
};

 
