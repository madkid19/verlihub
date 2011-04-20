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
#include "src/cconndc.h"
#include "src/i18n.h"
#include "cconsole.h"
#include "cpiisp.h"
#include "cisps.h"

using namespace nDirectConnect;


cISPs *cISPConsole::GetTheList()
{
	return mOwner->mList;
}

const char *cISPConsole::CmdSuffix()
{
	return "isp";
}

const char *cISPConsole::CmdPrefix()
{
	return "!";
}

void cISPConsole::GetHelpForCommand(int cmd, ostream &os)
{
	string help_str;
	switch(cmd)
	{
		case eLC_LST: 
		help_str = "!lstisp\r\n" + string(_("Give a list of ISPs"));
		break;
		case eLC_ADD: 
		case eLC_MOD:
		help_str = "!(add|mod)isp <iprange>"
			" [-N <\"name\">]"
			" [-CC <country_codes>]"
			" [-n <nick_pattern>]"
			" [-d <\"desc_tag\">]"
			" [-c <conn_type>]"
			" [-g <share_guest>]"
			" [-r <share_reg>]"
			" [-v <share_vip>]"
			" [-o <share_op>]"
			" [-G <max_share_guest>]"
			" [-R <max_share_reg>]"
			" [-V <max_share_vip>]"
			" [-O <max_share_op>]"
			" [-mn <\"nick error message\">]"
			" [-mc <\"conn error message\">]";
		break;
		case eLC_DEL:
		help_str = "!delisp <iprange>"; break;
		default: break;
	}
	cDCProto::EscapeChars(help_str,help_str);
	os << help_str;
}

const char * cISPConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_MOD:	
			return "^(\\S+)(" // <iprange>
			      "( -N ?(\")?((?(4)[^\"]+?|\\S+))(?(4)\"))|" // <name>|"<<name>>"
			      "( -CC ?(\\S+))|"   //[ -CC<country_codes>]
			      "( -n ?(\\S+))|" // [ -n<nick_pattern>]
			      "( -d ?(\")?((?(11)[^\"]+?|\\S+))(?(11)\"))|" //[ -d(<desc_tag>|"<<desc_tag>>")]
			      "( -c ?(\\S+))|"   //[ -c<conn_type>]
			      "( -g ?(-?\\d+))|" //[ -g<share_guest>]
			      "( -v ?(-?\\d+))|" //[ -v<share_vip>]
			      "( -r ?(-?\\d+))|" //[ -r<share_reg>]
			      "( -o ?(-?\\d+))|" //[ -o<share_op>]
			      "( -G ?(-?\\d+))|" //[ -G<max_share_guest>]
			      "( -R ?(-?\\d+))|" //[ -R<max_share_reg>]
			      "( -V ?(-?\\d+))|" //[ -V<max_share_vip>]
			      "( -O ?(-?\\d+))|" //[ -O<max_share_op>]
			      "( -mn ?(\")?((?(32)[^\"]+?|\\S+))(?(32)\"))|" 
			      "( -mc ?(\")?((?(35)[^\"]+?|\\S+))(?(35)\"))|"
			      ")*\\s*$" // the end of message
			      ; break;
		case eLC_DEL: return "(\\S+)"; break;
		default : return ""; break;
	};
}

bool cISPConsole::ReadDataFromCmd(cfBase *cmd, int id, cISP &data)
{
	/// regex parts for add command
	enum {aADD_ALL, eADD_RANGE, eADD_CHOICE,
		eADD_NAMEp, eADD_QUOTE, eADD_NAME,
		eADD_CCP, eADD_CC,
		eADD_NICKP, eADD_NICK,
		eADD_DESCP, aADD_DESCQ, eADD_DESC,
		eADD_CONNP, eADD_CONN,
		eADD_GUESTP, eADD_GUEST,
		eADD_REGP, eADD_REG,
		eADD_VIPP, eADD_VIP,
		eADD_OPP, eADD_OP,
		eADD_MGUESTP, eADD_MGUEST,
		eADD_MREGP, eADD_MREG,
		eADD_MVIPP, eADD_MVIP,
		eADD_MOPP, eADD_MOP,
		eADD_MNp, eADD_MNq, eADD_MN,
		eADD_MCp, eADD_MCq, eADD_MC
	};
	if (!cmd->GetParIPRange(eADD_RANGE, data.mIPMin, data.mIPMax)) {
		*(cmd->mOS) << _("Unknown range format.");
		return false;
	}

	cmd->GetParStr(eADD_NAME, data.mName);
	cmd->GetParStr(eADD_CC, data.mCC);
	if(!cmd->GetParRegex(eADD_NICK, data.mNickPattern) && cmd->PartFound(eADD_NICK)) {
		*(cmd->mOS) << _("Sorry the regular expression for nickname you provided is not valid.");
		return false;
	}
	
	cmd->GetParStr(eADD_DESC, data.mAddDescPrefix);
	
	if(!cmd->GetParRegex(eADD_CONN, data.mConnPattern) && cmd->PartFound(eADD_CONN)) {
		data.mConnPattern = "";
		*(cmd->mOS) << _("Sorry the regular expression for connection you provided is not valid.");
		return false;
	}

	cmd->GetParLong(eADD_GUEST, data.mMinShare[0]);
	cmd->GetParLong(eADD_REG  , data.mMinShare[1]);
	cmd->GetParLong(eADD_VIP  , data.mMinShare[2]);
	cmd->GetParLong(eADD_OP   , data.mMinShare[3]);

	cmd->GetParLong(eADD_MGUEST, data.mMaxShare[0]);
	cmd->GetParLong(eADD_MREG  , data.mMaxShare[1]);
	cmd->GetParLong(eADD_MVIP  , data.mMaxShare[2]);
	cmd->GetParLong(eADD_MOP   , data.mMaxShare[3]);

	cmd->GetParStr(eADD_MN, data.mPatternMessage);
	cmd->GetParStr(eADD_MC, data.mConnMessage);
	return true;
}

cISPConsole::~cISPConsole(){}

