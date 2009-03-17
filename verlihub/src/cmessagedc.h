/***************************************************************************
                          cmessagedc.h  -  description
                             -------------------
    begin                : Fri May 9 2003
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

#ifndef CMESSAGEDC_H
#define CMESSAGEDC_H

/**a dc Message incomming or outgoing
  *@author Daniel Muller
  */

#include "cobj.h"
#include "cprotocol.h"

using namespace std;
using namespace nServer;
namespace nDirectConnect {
namespace nProtocol {

namespace nEnums {
// these constants correspond to sDC_Commands in the .cpp file
typedef enum
{
	eDC_GETINFO,
	eDC_SEARCH_PAS,
	eDC_SEARCH,
	eDC_SR,
	eDC_MYNIFO,
	eDC_KEY,
	eDC_VALIDATENICK,
	eDC_MYPASS,
	eDC_VERSION,
	eDC_GETNICKLIST,
	eDC_CONNECTTOME,
	eDC_MCONNECTTOME,
	eDC_RCONNECTTOME,
	eDC_TO,
	eDC_CHAT,
	eDC_QUIT,
	eDC_OPFORCEMOVE,
	eDC_KICK,
	eDC_MSEARCH_PAS,
	eDC_MSEARCH,
	eDCE_SUPPORTS,
	eDCM_NETINFO,
	eDCO_BAN,
	eDCO_TBAN,
	eDCO_UNBAN,
	eDCO_GETBANLIST,
	eDCO_WHOIP,
	eDCO_BANNED,
	eDCO_SETTOPIC,
	eDCO_GETTOPIC,
	eDCB_BOTINFO,
	eDC_UNKNOWN
} tDCMsg;

/** Following constants design the placements of corresponding attribute of every
	DC protocol message comming from the client... in the mChunks,
	these are used in the cDCServer::DC_* functions
	and also in the cMessageDC::SplitChunks ... they must be corresponding
	 */
// chunk numbers for simple commands
enum { eCH_0_ALL }; // eDC_GETNICKLIST
// chunk numbers for cmd with one param only
enum { eCH_1_ALL, eCH_1_PARAM }; //KEY VALIDATENICK VERSION MYPASS QUIT KICK UNBAN
// -----*** chunk numbers for all other ***-----
// chat
enum { eCH_CH_ALL, eCH_CH_NICK, eCH_CH_MSG };
// GetINFO
enum { eCH_GI_ALL, eCH_GI_OTHER, eCH_GI_NICK };
// RevConnectToMe   $RevConnectToMe <nick> <remoteNick>
enum { eCH_RC_ALL, eCH_RC_NICK, eCH_RC_OTHER };
// private chat : CHMSH is a "<nick> msg" together
enum { eCH_PM_ALL, eCH_PM_TO, eCH_PM_FROM, eCH_PM_CHMSG, eCH_PM_NICK, eCH_PM_MSG } ;
// MyINFO : INFO is all the rest together
enum { eCH_MI_ALL, eCH_MI_DEST, eCH_MI_NICK, eCH_MI_INFO, eCH_MI_DESC, eCH_MI_SPEED, eCH_MI_MAIL, eCH_MI_SIZE };
/// connecttome   $ConnectToMe <remoteNick> <senderIp>:<senderPort>
enum {eCH_CM_ALL, eCH_CM_NICK, eCH_CM_ACTIVE, eCH_CM_IP, eCH_CM_PORT};
//OpForce move
enum {eCH_FM_ALL, eCH_FM_NICK, eCH_FM_DEST, eCH_FM_REASON };
// active search	@dReiska:(query format:  <sizerestricted>?<isminimumsize>?<size>?<datatype>?<searchpattern> --> searchpattern is added here)
enum {eCH_AS_ALL, eCH_AS_ADDR, eCH_AS_IP, eCH_AS_PORT, eCH_AS_QUERY, eCH_AS_SEARCHLIMITS, eCH_AS_SEARCHPATTERN};
// passive search	 @dReiska:(query format:  <sizerestricted>?<isminimumsize>?<size>?<datatype>?<searchpattern> --> searchpattern is added here)
enum {eCH_PS_ALL, eCH_PS_NICK, eCH_PS_QUERY, eCH_PS_SEARCHLIMITS, eCH_PS_SEARCHPATTERN};
// search result $SR <resultNick> <filepath>^E<filesize> <freeslots>/<totalslots>^E<hubname> (<hubhost>[:<hubport>])^E<searchingNick>
enum {eCH_SR_ALL, eCH_SR_FROM, eCH_SR_PATH, eCH_SR_SIZE, eCH_SR_SLOTS, eCH_SR_SL_FR, eCH_SR_SL_TO, eCH_SR_HUBINFO, eCH_SR_TO};
// $NetInfo slots$hubs$active|
enum {eCH_NI_ALL, eCH_NI_SLOTS, eCH_NI_HUBS, eCH_NI_ACTIVE};
// $Ban nick$reason
// $TempBan nick$time$reason
enum {eCH_NB_ALL, eCH_NB_NICK, eCH_NB_REASON, eCH_NB_TIME};

};

using namespace nDirectConnect::nProtocol::nEnums;


/**
* The Direct Connect Protocol Message parser and container
* provides access to all important parts of the message as std::string
*/
class cMessageDC : public cMessageParser
{
public:
	cMessageDC();
	virtual ~cMessageDC();
	/** parses the string and sets the state variables */
	virtual int Parse(); // override
	/** splits message to it's important parts and stores their info in the chunkset mChunks */
	virtual bool SplitChunks(); // override

private: // Private methods
	/** the static buffer saving memory and allocations */
//	static char *mBuffer;
//	static const int msBuffSize;
//	static int msCounterMessageDC;
protected: // Protected methods
};
};
};

#endif
