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
#ifndef CDCPROTO_H
#define CDCPROTO_H
#include <string>
#include "cpcre.h"
#include "cprotocol.h"

using nUtils::cPCRE;
using namespace std;
using namespace nServer;

/**
  * \brief Encaplulates all Direct Connect related classes methods etc..
  */
namespace nDirectConnect {

	namespace nTables{ class cConnType; };
	using namespace ::nDirectConnect::nTables;
	class cConnDC;
	class cServerDC;
	class cUser;
	class cUserBase;

/**
  * \brief Mostly DC protocol related stuff
  */
namespace nProtocol {

using namespace ::nDirectConnect;
using ::nDirectConnect::cConnDC;

class cMessageDC;

/**protocol part of the server
  *@author Daniel Muller
  */
class cDCProto : public cProtocol
{
	friend class ::nDirectConnect::cServerDC;
public:
	cDCProto(cServerDC *serv);
	virtual ~cDCProto(){};
	virtual int TreatMsg(cMessageParser *msg, cAsyncConn *conn); //override
	virtual cMessageParser *CreateParser();
	virtual void DeleteParser(cMessageParser *);	
	
	/** sends userlist and oplist */
	int NickList(cConnDC *);
	/** test if text is a console command and parse it by console eventually
	return 1 if it was a command else return 0 */
 	int ParseForCommands(const string &, cConnDC *);
protected:
	cServerDC *mS;
	/** Treat the DC message in a appropriate way */
	int DC_ValidateNick(cMessageDC *msg, cConnDC *conn);
	/** Treat the DC message in a appropriate way */
	int DC_Key(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_MyPass(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_GetNickList(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_Version(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_Chat(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_To(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_GetINFO(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_MyINFO(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_ConnectToMe(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_Kick(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_OpForceMove(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_RevConnectToMe(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_MultiConnectToMe(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_Search(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	int DC_SR(cMessageDC * msg, cConnDC * conn);
	/** Treat the DC message in a appropriate way */
	//int DC_ZON(cMessageDC * msg, cConnDC * conn);

	/****************************/
	/** extended supports features */
	int DCE_Supports(cMessageDC * msg, cConnDC * conn);
	/** Network info (neo Modus) */
	int DCM_NetInfo(cMessageDC * msg, cConnDC * conn);
	/** operator ban */
	int DCO_TempBan(cMessageDC * msg, cConnDC * conn);
	/** operator unban */
	int DCO_UnBan(cMessageDC * msg, cConnDC * conn);
	/** operator getbanlist */
	int DCO_GetBanList(cMessageDC * msg, cConnDC * conn);
	/** Bot information */
	int DCB_BotINFO(cMessageDC * msg, cConnDC * conn);
	/** who is with given ip */
	int DCO_WhoIP(cMessageDC * msg, cConnDC * conn);
	/** operator getbanlist filtered by the parameter */
	int DCO_Banned(cMessageDC * msg, cConnDC * conn);
	/** get hub topic */
	int DCO_GetTopic(cMessageDC * msg, cConnDC * conn);
	/** operator set hub topic */
	int DCO_SetTopic(cMessageDC * msg, cConnDC * conn);public: // Public attributes
	const string &GetMyInfo(cUserBase * User, int ForClass);
	static void Create_MyINFO(string &dest, const string&nick, const string &desc, const string&speed, const string &mail, const string &share);
	void Append_MyInfoList(string &dest, const string &MyINFO, const string &MyINFO_basic, bool DoBasic);
	static void Create_PM(string &dest,const string &from, const string &to, const string &sign, const string &text);
	static void Create_PMForBroadcast(string &start,string &end, const string &from, const string &sign, const string &text);
	static void Create_HubName(string &dest, string &name, string &topic);
	static void Create_Chat(string &dest, const string&nick,const string &text);
	static void Create_Quit(string &dest, const string&nick);
	cConnType *ParseSpeed(const std::string &speed);
	static void EscapeChars(const string &, string &, bool WithDCN = false);
	static void EscapeChars(const char *, int, string &, bool WithDCN = false);
	static void UnEscapeChars(const string &, string &, bool WithDCN = false);
	static void UnEscapeChars(const string &, char *, int &len ,bool WithDCN = false);
	static void Lock2Key(const string &lock, string &fkey);
	static bool CheckChatMsg(const string &text, cConnDC *conn);
	static bool CheckIP(cConnDC * conn, string &ip);
	static bool isLanIP(string);
	/** regex for kick chat messages */
	cPCRE mKickChatPattern;
	cPCRE mKickBanPattern;

};
};
};
#endif
