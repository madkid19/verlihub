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
#ifndef CCONNDC_H
#define CCONNDC_H
#include "casyncconn.h"
#include "cmessagedc.h"
#include "cuser.h"
#include "creguserinfo.h"
#include "ctimeout.h"

using namespace nServer;
using namespace nUtils;

namespace nDirectConnect
{

	namespace nTables{ class cConnType; };
	using namespace nTables;
	namespace nProtocol { class cDCProto; }
	using nProtocol::cDCProto;


class cUser;
class cServerDC;


/** LOGIN STATUS, unless all these aren't ok, user won't be let to do no more
 * than something .. */

namespace nEnums {

typedef enum
{
	eLS_KEYOK   = 1 << 0, //< the key was all right
	eLS_VALNICK = 1 << 1, //< validate valid nick
	eLS_PASSWD  = 1 << 2, //< password was ok, if userneeds no passw this is set to true
	eLS_VERSION = 1 << 3, //< version of client is ok
	eLS_MYINFO  = 1 << 4, //< myinfo is received and ok
	eLS_ALOWED  = 1 << 5, //< user is allowd to enter the hub, hub isn't full
	eLS_NICKLST = 1 << 6, //< user received complete nicklist
	eLS_LOGIN_DONE = eLS_KEYOK|eLS_VALNICK|eLS_PASSWD|eLS_VERSION|eLS_MYINFO|eLS_ALOWED|eLS_NICKLST // all
} tLogStatus;

typedef enum
{
	eTO_KEY=0,  //< after lock, waiting for key
	eTO_VALNICK, //< after lock also waiting for the validation of nick
	eTO_LOGIN,  //< after accept, until DoUserLogin
	eTO_MYINFO, //< after getionfo until myinfo
	eTO_FLUSH,  //< from some wites to flush
	eTO_SETPASS, //< setting password after login without pass
	eTO_MAXTO   //< no use
} tTimeOut;

typedef enum
{
	eSF_OPPLUS    = 1     , //< OpPlus commands
	eSF_NOHELLO   = 1 << 1, //< NoHello
	eSF_NOGETINFO = 1 << 2, //< NoGetINFO
	eSF_PASSIVE   = 1 << 3, //< passive users, they don't get passive search
	eSF_QUICKLIST = 1 << 4, //< a quicklist extention
	eSF_BOTINFO   = 1 << 5, // BOTInfo extention
 	eSF_ZLIB      = 1 << 6 //< ZPipe extention
}tSupportFeature;

/**  \brief DC user's connection type
  *
  *  As is defined by user in the options dialog
  * /
typedef enum tConnectionType
{
	eCT_DEFAULT, //< Any non defined connection type
	eCT_MODEM28, //< 28.8 kbps modem
	eCT_MODEM33, //< 33.6 kbps modem connection
	eCT_MODEM56, //< 56kbps modem
	eCT_MODEM,  //< just a modem
	eCT_ISDN,  //< isdn
	eCT_CABLE, //< cable
	eCT_DSL, //< DSL
	eCT_SATELLITE, //< satellite
	eCT_MICROWAVE, //< microwave
	eCT_WIRELESS, //< Wireless
	eCT_LANT1, //< Lan T1
	eCT_LANT3, //< Lan T3
	eCT_UNDEF //< wrong type
};
*/

};

using namespace ::nDirectConnect::nEnums;

// forward declarations
class cDCBanRecord;

class cConnDC;

class cDCConnFactory : public cConnFactory
{
public:
	cDCConnFactory(cServerDC *server);
	virtual ~cDCConnFactory(){}
	virtual cAsyncConn * CreateConn(tSocket sd=0);
	virtual void DeleteConn(cAsyncConn * &);
protected:
	cServerDC *mServer;
};

/**a direct connect client connection
  *@author Daniel Muller
  */


//***************** the class
class cConnDC : public cAsyncConn
{
	friend class nProtocol::cDCProto;
public:
	cConnDC(int sd=0, cAsyncSocketServer *server=NULL);
	virtual ~cConnDC();
	/** returns true if ok, unless false */
	bool SetUser(cUser *usr);
	/** set log status flag to a given one in the user*/
	void SetLSFlag(unsigned int st);
	// set flags to given value
	void ReSetLSFlag(unsigned int st);
public: // Public attributes
	/** a pointer to dc user, this is the only class that can destroy it,
		and that must keep it until it's end, server creates it */
	cUser * mpUser;
	/** client's version string */
	string mVersion;
	/** supported extended protocol features */
	unsigned mFeatures;
	/** info about registration */
	cRegUserInfo *mRegInfo;
	/** if nicklist is to send on userlogin */
	bool mSendNickList;
	/** true while sending first nicklist */
	bool mNickListInProgress;
	/** indicates whether nicklist should be skipped or not */
	bool mSkipNickList;
	/** conetion type */
	cConnType *mConnType;
	/** country code */
	string mCC;
	/** geographic zone according to country code */
	int mGeoZone;
	/** closing details ..*/
	int mCloseReason;
	virtual void CloseNow(int Reason = 0);
	virtual void CloseNice(int msec, int Reason = 0);
	/** return pointer to the server */
	inline cServerDC * Server(){return (cServerDC*) mxServer;}
	/** Send raw data whenever it's next possible,
	n is the size of data, if not specified, or zero, the null unterminated string is mesured
	the dc separator is appended |
	*/
	//int Send(const char * data, int n=-1);
	/** Send raw data whenever it's next possible,
		n is the size of data, if not specified, or zero, the null terminated string is mesured */
	int Send(string & data, bool AddPipe=true, bool Flush = true);
	/** log the event */
	virtual int StrLog(ostream & ostr, int level);
	/** get log status flag to a given one in the user */
	unsigned int GetLSFlag(unsigned int st);
	/** the ithe class that user may have if he validates password */
	int GetTheoricalClass()
	{
		if (!mRegInfo) return 0;
		if(!mRegInfo->mEnabled) return 0;
		//if (mRegInfo->mClass < 0) return 1; /* wtf ?*/
		return mRegInfo->mClass;
	}
	/** this is called every period of time */
	virtual int OnTimer(cTime &now);
private:
	cTime mTimeLastAttempt;
	/** login status */
	unsigned int mLogStatus;
	//bool isCompressed;

public: // Protected methods
	/** Set's the timeout for given action, after timeout if not cleared, connection is closed */
	int SetTimeOut(tTimeOut, double Sec, cTime &now);
	/** storno the timeout */
	int ClearTimeOut(tTimeOut);
	/** return true if time is not out yet */
	int CheckTimeOut(tTimeOut t, cTime &now);
	/** this is called when write buffer gets empty */
	void OnFlushDone();
	/** return true if timeout is set */
	//int IsSetTimeOut(tTimeOut);
	bool NeedsPassword();
	
protected: // Protected attributes
	/** time out's */
	cTimeOut mTO[eTO_MAXTO];
	/** time ins */
	struct sTimes
	{
		cTime key;
		cTime ping;
		sTimes():key(0l),ping(0l){};
	};
	sTimes mT;
	int mSRCounter;
protected: // Protected methods
	/** function called before closing nicely */
	int OnCloseNice();
};

};

#endif
