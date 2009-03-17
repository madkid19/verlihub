/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECT_NPLUGINCVHPLUGIN_H
#define NDIRECTCONNECT_NPLUGINCVHPLUGIN_H

#include "cpluginbase.h"
#include "cusercollection.h"

using ::nPlugin::cPluginBase;

namespace nDirectConnect {

namespace nProtocol {
class cMessageDC;
};
namespace nTables {
class cBan;
class cRegUserInfo;
};

using namespace nProtocol;
using namespace nTables;

class cServerDC;
class cConnDC;
class cDCTag;
class cUser;
class cUserRobot;
class cPluginRobot;

namespace nPlugin {

using namespace ::nDirectConnect;
using ::nDirectConnect::cServerDC;
using ::nDirectConnect::cConnDC;

class cPluginUserData
{
	public:
		cPluginUserData() 
		{
			mxUser = NULL;
		}
	private:
		string Nick; // in case user is already left this is still valid
		cUser *mxUser;
};

/** \brief The Verlihub's plugin class
  *
  * derive your plugins from this one
  *
  * @author Daniel Muller
*/

class cVHPlugin : public cPluginBase
{
public:
	/// constructor
	cVHPlugin();
	/// destructor
	virtual ~cVHPlugin();

	////////// Genral VH plugin's interface
	virtual bool SupportsScripts(){ return false;}
	virtual bool SupportsMultipleScripts(){ return false;}
	// to load the main single script if only one is supported
	virtual bool LoadScript(const string &filename, ostream &os);
	// to load another extra script if mutiple are supported
	virtual bool AddScript(const string &filename, ostream &os);
	virtual bool IsScriptLoaded(const string &filename){return false;}
	virtual bool UnLoadScript(const string &filename){return false;}
	virtual bool GetFirstScript(string &filename){return false;}
	virtual bool GetNextScript(string &filename){return false;}

	/** \brief this is called when plugin is being loaded
	  *
	  * \fn OnLoad(cServerDC* server)
	  * \param Server verlihub server that is loading this plugin
	  */
	virtual void OnLoad(cServerDC* server){mServer = server;}

	///////////////// Callbacks and events
	/** \brief when new conn is accepted, return false if it was closed
	  *
	  * \fn OnNewConn( cConnDC * conn)
	  * \param the new established connection
	  * \return true if ok, false if you have closed the connection
	  * \sa RegisterAll
	  * You need to call RegisterCallBack("VH_OnNewConn");
	  */
	virtual bool OnNewConn( cConnDC * conn){ return true; }

	/** \brief connection closed
	  *
	  * \param the just being closed connection
	  * \sa RegisterAll
	  * You need to call RegisterCallBack("VH_OnCloseConn");
	  */
	virtual bool OnCloseConn( cConnDC* ){ return true; }
	/// when unknown message data comes from a connection
	virtual bool OnUnknownMsg     ( cConnDC* , cMessageDC *){ return true; }
	/// when any parsed msg comes, return  false if message should be processed on...
	/// this is very different from other OnParsedMsg* function, because this one is called
	/// before they are even processed, others are called ofthen after they have been verified
	/// checked and accepted
	virtual bool OnParsedMsgAny     ( cConnDC* , cMessageDC *){ return true; }
	/// $Supports message came
	virtual bool OnParsedMsgSupport ( cConnDC* , cMessageDC *){ return true; }

	/** \brief $ValidateNick comes
		*
		* \return true if ok, false, when you closed the connection
		*/
	virtual bool OnParsedMsgValidateNick  ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgMyPass  ( cConnDC* , cMessageDC *){ return true; }

	/** \brief $MyINFO comes
		*
		* \return true if ok, false, when you closed the connection
		*/
	virtual bool OnParsedMsgMyINFO  ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgSearch  ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgSR      ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgChat    ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgPM      ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgConnectToMe ( cConnDC* , cMessageDC *){ return true; }
	virtual bool OnParsedMsgRevConnectToMe ( cConnDC* , cMessageDC *){ return true; }
	/// \return false when tag is not valid
	virtual bool OnValidateTag      ( cConnDC* , cDCTag *){ return true; }
	/// \return false when command is not to be treated anymore
	virtual bool OnOperatorCommand  ( cConnDC* , std::string *){ return true; }
	virtual bool OnUserCommand      ( cConnDC* , std::string *){ return true; }

	/** When op kicks someon with a reason..
		\param OP kicking operator
		\param User kicked user
		\param Reason the kick reason
		\return false if you want to omit the kick

		NOte, use user->mToBan and mBanTime to modify banning..
	*/
	virtual bool OnOperatorKicks    ( cUser* OP, cUser *User , std::string *Reason){ return true; }
	virtual bool OnOperatorDrops    ( cUser* OP, cUser *User ){ return true; }

	/// called when user login is finished, all checked and ok
	virtual bool OnUserLogin        ( cUser* ){ return true; }
	virtual bool OnUserLogout       ( cUser* ){ return true; }
	// when new bann added
	virtual bool OnNewBan           ( cBan * ){ return true; }
	// when new reg user added or modified
	virtual bool OnNewReg           ( cRegUserInfo * ){ return true; }

	virtual bool OnTimer            ( ){ return true; }
	/// Called when loading and  when it's the correct time to register for callbacks
	virtual bool RegisterAll() = 0;

	virtual bool AddRobot(cUserRobot *);
	virtual bool DelRobot(cUserRobot *);
	virtual cPluginRobot * NewRobot(const string &Nick, int);

	/// robot events
	virtual bool RobotOnPM( cPluginRobot *, cMessageDC *, cConnDC *){ return true;};
	// nick list extension
	virtual bool OnCreateUserNickList (string *NickList) {return true;};
	virtual bool OnCreateUserInfoList (string *InfoList) {return true;};
	virtual bool OnCreateOpList (string *OpList) {return true;};

	/// per-user data of the plugin
	virtual cPluginUserData *GetPluginUserData( cUser * );
	virtual cPluginUserData *SetPluginUserData( cUser *, cPluginUserData *NewData );
	
	/// Pointer for the verlihub server
	cServerDC *mServer;
	cUserCollection mRobots;
	tHashArray<cPluginUserData*> *mUserDataTable;
};

};

};

#endif
