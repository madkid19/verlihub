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
#include "cserverdc.h"
#include "ctrigger.h"
#include "cconndc.h"
#include "curr_date_time.h"
#include "stringutils.h"
#include <stdlib.h>
#include <time.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "i18n.h"

using namespace nStringUtils;
namespace nDirectConnect {
namespace nTables {

  /**
  
  Class constructor
 
  */
  
cTrigger::cTrigger()
{
	mSeconds = mLastTrigger = mFlags = 0;
	mMinClass=0;
	mMaxClass=10;
	mMaxLines=0;
	mMaxSize=0;
	mDelayUser=0;
	mDelayTotal=0;
}

  /**

  Class destructor

  */

cTrigger::~cTrigger(){}
  /**

  Send the trigger message. Message can be the content of a file or a string stored in the database (in this case def column)

  @param[in,out] conn The connection that triggers the trigger
  @param[in] server Reference to CServerDC
  @param[in] cmd_line The message of the trigger
  @return 0 if an error occurs or the user cannot see the trigger
  */

int cTrigger::DoIt(istringstream &cmd_line, cConnDC *conn, cServerDC &server, bool timer)
{
	bool timeTrigger = timer && conn == NULL;
	string buf, filename, sender;
	string par1, end1, parall;
	
	// Check if it has been triggered by timeout. If not, check the connection and the user rights
	if(!timeTrigger) {
		if(!conn) return 0;
		if(!conn->mpUser) return 0;
	       
		int uclass = conn->mpUser->mClass;
		if ((uclass < this->mMinClass)  || (uclass > this->mMaxClass)) return 0;
	}
	
	if(cmd_line.str().size() > mCommand.size()) {
		parall.assign(cmd_line.str(),mCommand.size()+1,string::npos);
	}
	cmd_line >> par1;
	end1 = cmd_line.str();
	
	// Replace sender
	sender = server.mC.hub_security;
	if (mSendAs.size()) sender = mSendAs;
	
	ReplaceVarInString(sender, "PAR1", sender, par1);
	if(!timeTrigger) ReplaceVarInString(sender, "NICK", sender, conn->mpUser->mNick);
	
	
	if (mFlags & eTF_DB) {
	  buf = mDefinition;
	} else {
	  ReplaceVarInString(mDefinition,"CFG", filename, server.mConfigBaseDir);
	  if(!timeTrigger) ReplaceVarInString(filename,"CC", filename, conn->mCC);
	  if (!LoadFileInString(filename,buf)) return 0;
	}
	if (mFlags & eTF_VARS) {
	cTime theTime(server.mTime);
	time_t curr_time;
	time(&curr_time);
	#ifdef _WIN32
		//TODO: do we really need reentrant version?
		struct tm *lt = localtime(&curr_time);
	#else
		struct tm *lt = new tm();
		localtime_r(&curr_time, lt);
	#endif
	  
	  theTime -= server.mStartTime;	
	  ReplaceVarInString(buf, "PARALL", buf, parall);
	  ReplaceVarInString(buf, "PAR1", buf, par1);
	  ReplaceVarInString(buf, "END1", buf, end1);
	  if(!timeTrigger) {
		ReplaceVarInString(buf, "CC", buf, conn->mCC);
		ReplaceVarInString(buf, "IP", buf, conn->AddrIP());
		ReplaceVarInString(buf, "HOST", buf, conn->AddrHost());
		ReplaceVarInString(buf, "NICK", buf, conn->mpUser->mNick);
		ReplaceVarInString(buf, "SHARE", buf, convertByte(conn->mpUser->mShare, false));
	  }
	  
	  ReplaceVarInString(buf, "USERS", buf, (int)server.mUserList.size());
	  ReplaceVarInString(buf, "USERSPEAK", buf, (int)server.mUsersPeak);
	  ReplaceVarInString(buf, "UPTIME", buf, theTime.AsPeriod().AsString());
	  ReplaceVarInString(buf, "VERSION", buf, VERSION);
	  ReplaceVarInString(buf, "HUBNAME", buf, server.mC.hub_name);
	  ReplaceVarInString(buf, "HUBTOPIC", buf,server.mC.hub_topic);
	  ReplaceVarInString(buf, "VERSION_DATE", buf, __CURR_DATE_TIME__);
	  ReplaceVarInString(buf, "TOTAL_SHARE", buf, convertByte(server.mTotalShare, false));
	  char tmf[3];
	  sprintf(tmf,"%02d",lt->tm_sec);
	  ReplaceVarInString(buf, "ss",buf, tmf);
	  sprintf(tmf,"%02d",lt->tm_min);
	  ReplaceVarInString(buf, "mm",buf, tmf);
	  sprintf(tmf,"%02d",lt->tm_hour);
	  ReplaceVarInString(buf, "HH",buf, tmf);
	  sprintf(tmf,"%02d",lt->tm_mday);
	  ReplaceVarInString(buf, "DD",buf, tmf);
	  sprintf(tmf,"%02d",lt->tm_mon+1);
	  ReplaceVarInString(buf, "MM",buf, tmf);
	  ReplaceVarInString(buf, "YY",buf, 1900 + lt->tm_year);
	  #ifndef _WIN32
	  delete lt;
	  #endif
	}	
	
	if(timeTrigger) {
	  server.DCPublicToAll(sender,buf);
	  return 1;
	}
	
	
	// @CHANGED by dReiska +BEGINS+
	if (mFlags & eTF_SENDTOALL) {
	  if (!(mFlags & eTF_SENDPM)) {
	    server.DCPublicToAll(sender,buf);
	  } else {/*
	    server.DCPrivateToAll(sender,buf);
	    */
	  }
	} else {
	  if (!(mFlags & eTF_SENDPM)) {
	    server.DCPublic(sender, buf, conn);
	  } else {
	    server.DCPrivateHS(buf, conn, &sender);
	  }
	}
	return 1;
}

  /**

  This function is called when cTrigger object is created

  */

void cTrigger::OnLoad()
{}

  /**

  Redefine << operator to show and describe a trigger

  @param[in,out] ostream The stream where to write
  @param[in] tr Reference to cTrigger
  @return The stream
  */

ostream &operator << (ostream &os, cTrigger &tr)
{
    	os << autosprintf(_("%s %s Flags: %d  SendAs: %s  Class: %d-%d") , tr.mCommand.c_str(), tr.mDefinition.substr(0,64).c_str(), tr.mFlags, tr.mSendAs.c_str(), tr.mMinClass, tr.mMaxClass);
	if(tr.mSeconds) {
		cTime timeout = cTime(tr.mSeconds);
		os << " Timeout: " <<  timeout.AsPeriod();
	}
	return os;
}

};
};
