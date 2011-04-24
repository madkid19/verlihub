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
#ifndef CCONNDC_H
#define CCONNDC_H
#include "casyncconn.h"
#include "cmessagedc.h"
//#include "cuser.h"
#include "creguserinfo.h"
#include "ctimeout.h"

namespace nVerliHub {
	using namespace nSocket;
	using namespace nUtils;
	using namespace nTables;
	namespace nTables{ class cConnType; };
	namespace nProtocol { class cDCProto; }
	using nProtocol::cDCProto;

	class cUser;
	namespace nSocket {class cServerDC; };

	namespace nEnums {

		// Login status flags
		typedef enum
		{
			eLS_KEYOK   = 1 << 0, // Key is ok
			eLS_VALNICK = 1 << 1, // Validate nick sent
			eLS_PASSWD  = 1 << 2, // User does not need to send a password or password is ok
			eLS_VERSION = 1 << 3, // Client version is ok
			eLS_MYINFO  = 1 << 4, // MyInfo received and parsed
			eLS_ALOWED  = 1 << 5, // User can enter the hub
			eLS_NICKLST = 1 << 6, // User received complete nicklist
			eLS_LOGIN_DONE = eLS_KEYOK|eLS_VALNICK|eLS_PASSWD|eLS_VERSION|eLS_MYINFO|eLS_ALOWED|eLS_NICKLST // All flags set (login complete)
		} tLogStatus;

		// Login timeout flags
		typedef enum
		{
			eTO_KEY=0,  	// Waiting for key after lock
			eTO_VALNICK,	// Waiting for validate nick after lock
			eTO_LOGIN,  	// User is logging in
			eTO_MYINFO, 	// Waiting for MyINFO
			eTO_FLUSH, 	// Waiting for flushing data from buffer
			eTO_SETPASS, 	// Waiting for password
			eTO_MAXTO   	// Not used
		} tTimeOut;

		// Support flags ($Support)
		typedef enum
		{
			eSF_OPPLUS    = 1     , // OpPlus
			eSF_NOHELLO   = 1 << 1, // NoHello
			eSF_NOGETINFO = 1 << 2, // NoGetINFO
			eSF_PASSIVE   = 1 << 3, // Passive user
			eSF_QUICKLIST = 1 << 4, // Quicklist extention
			eSF_BOTINFO   = 1 << 5, // BOTinfo extention
			eSF_ZLIB      = 1 << 6 	// ZPipe extention
		} tSupportFeature;
	};

using namespace nEnums;

class cDCBanRecord;
//class cConnDC;

	namespace nSocket {

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

/**
* This class represents a connection.
*
* @author Daniel Muller
* @version 1.0
*/

class cConnDC : public cAsyncConn
{
	friend class nProtocol::cDCProto;
 public:
	/**
	* Class constructor.
	*/
	cConnDC(int sd=0, cAsyncSocketServer *server=NULL);

	/**
	* Class destructor.
	*/
	virtual ~cConnDC();

	/**
	* Check if timeout is expired for the given action.
	* @param t The action.
	* @param now Current time.
	* @return 1 if timeout is expired or 0 otherwise.
	*/
	int CheckTimeOut(tTimeOut t, cTime &now);

	/**
	* Reset and clear the timeout for the given action.
	* @return 1 if timeout is cleared or 0 otherwise.
	*/
	int ClearTimeOut(tTimeOut);

	/**
	* Close the connection with the given reason after msec milliseconds are elapsed.
	* @param Reason The reason.
	* @param msed Time in milliseconds before closing the connection.
	*/
	virtual void CloseNice(int msec, int Reason = 0);

	/**
	* Close the connection with the given reason.
	* @param Reason The reason.
	*/
	virtual void CloseNow(int Reason = 0);

	/**
	* Return if the given login status flag is set or not.
	* @param st Status flag.
	* @return Zero if flag is not set or the value of the given status flag if set.
	*/
	unsigned int GetLSFlag(unsigned int st);

	/**
	* Return a string describing the timeout.
	* @param t tTimeOut Timeout type.
	* @return A translated string describing the timeout.
	*/
	const char *GetTimeOutType(tTimeOut t);

	/**
	* Try to guess the class of the user. Remember a valid class is returned only when the user is validated
	* @param ostr The message of the event
	* @param level The log level of the event.
	* @return The user class.
	*/
	int GetTheoricalClass()
	{
		if (!mRegInfo)
			return 0;
		if(!mRegInfo->mEnabled)
			return 0;
		//if (mRegInfo->mClass < 0) return 1; /* wtf ?*/
		return mRegInfo->mClass;
	}

	/**
	* Check if the user needs a password.
	* @return True if the user must provide a password, false otherwise.
	*/
	bool NeedsPassword();

	/**
	* This method is called when output/write buffer is flushed and got empty.
	*/
	void OnFlushDone();

	/**
	* This method is called every period of time.
	* @param now Current time.
	* @return Always zero.
	*/
	virtual int OnTimer(cTime &now);

	/**
	* Reset login status flag and set a new value.
	* @param st Status flag.
	*/
	void ReSetLSFlag(unsigned int st);

	/**
	* Set login status flag. Other flags are not override.
	* @param st Status flag.
	*/
	void SetLSFlag(unsigned int st);

	/**
	* Set the cUser object linked with the connection.
	* @param usr Pointer to cUser object.
	* @return True if successful, otherwise false.
	*/
	bool SetUser(cUser *usr);

	/**
	* Send raw data to the user.
	* @param data Raw data to send.
	* @param AddPipe Set it to true if a pipe must be added to the end of data.
	* @param Flush Set it to true if raw data should be send immediatly or stored in internal buffer.
	* @return The result.
	*/
	int Send(string & data, bool AddPipe=true, bool Flush = true);

	/**
	* Return a pointer to cServerDC object.
	* @return The pointer to cServerDC object.
	*/
	inline cServerDC * Server(){return (cServerDC*) mxServer;}

	/**
	* Set the timeout for the given action.
	* If timeout is not cleared (action is timeout), the connection is closed.
	* @param action The action.
	* @param Sec Timeout in seconds.
	* @param now Current time.
	* @return 1 if timeout is set or 0 otherwise.
	*/
	int SetTimeOut(tTimeOut, double Sec, cTime &now);

	/**
	* Log an event.
	* @param ostr The message of the event.
	* @param level The log level of the event.
	* @return 1 if message is logged or 0 otherwise.
	*/
	virtual int StrLog(ostream & ostr, int level);

	// Pointer to cUser object
	cUser * mpUser;

	// Client version
	string mVersion;

	// Protocol extenstion supported by the client
	unsigned mFeatures;

	// Pointer to cRegUserInfo that handles registration
	cRegUserInfo *mRegInfo;

	// True if nicklist is sent on user login
	bool mSendNickList;

	// True while sending nicklist
	bool mNickListInProgress;

	// True if nicklist should be skipped
	bool mSkipNickList;

	// Pointer to cConnType object that represent the type of connection
	cConnType *mConnType;

	// Country code
	string mCC;

	// Geographic zone according to country code
	int mGeoZone;

	// Reason of why connection was closed
	int mCloseReason;

 private:
	// Last login attempt
	cTime mTimeLastAttempt;

	// Login status flags
	unsigned int mLogStatus;

 protected:
	/**
	* This method is called before closing the connection.
	* Send the redirect protocol message ($ForceMove) to the user.
	* @return 1 if message is sent or 0 otherwise.
	* @see CloseNice(int msec, int Reason = 0)
	*/
	int OnCloseNice();

	// Timeout handlers
	cTimeOut mTO[eTO_MAXTO];

	// Structure that is used to ping the user and check if he is stil alive
	struct sTimes
	{
		cTime key;
		cTime ping;
		sTimes():key(0l),ping(0l){};
	};
	// Ping handler
	sTimes mT;

	// Search result counter
	int mSRCounter;
};

	}; // namespace nSocket
}; // namespace nVerliHub

#endif
