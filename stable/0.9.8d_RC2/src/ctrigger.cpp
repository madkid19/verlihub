/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cserverdc.h"
#include "ctrigger.h"
#include "cconndc.h"
#include "curr_date_time.h"
#include "stringutils.h"
#include <stdlib.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

using namespace nStringUtils;
namespace nDirectConnect {
namespace nTables {
/// Constructor
cTrigger::cTrigger()
{
	mFlags = 0;
	mMinClass=0;
	mMaxClass=10;
	mMaxLines=0;
	mMaxSize=0;
	mDelayUser=0;
	mDelayTotal=0;
}
/// Destructor
cTrigger::~cTrigger(){}

/** \fn cTrigger::DoIt(istringstream &cmd_line, cConnDC *conn, cServerDC &server)
	\brief Send the defined file contents to the user connection
	\param conn the connecten that trigger the trigger
	\param server reference for the DC server
	\param cmd_line what user has writter
*/
int cTrigger::DoIt(istringstream &cmd_line, cConnDC *conn, cServerDC &server)
{
	string buf, filename, sender;
	string par1, end1, parall;
	if (conn && conn->mpUser)
	{
		int uclass = conn->mpUser->mClass;
		if ((uclass >= this->mMinClass) &&(uclass <= this->mMaxClass)) {

			if(cmd_line.str().size() > mCommand.size()) {
				parall.assign(cmd_line.str(),mCommand.size()+1,string::npos);
			}
			cmd_line >> par1;
			end1 = cmd_line.str();

			sender = server.mC.hub_security;
			if (mSendAs.size()) sender = mSendAs;

			ReplaceVarInString(sender, "PAR1", sender, par1);
			ReplaceVarInString(sender, "NICK", sender, conn->mpUser->mNick);
			
			if (mFlags & eTF_DB || mFlags & eTF_EXECUTE) {
				buf = mDefinition;
			} else {
				ReplaceVarInString(mDefinition,"CFG", filename, server.mConfigBaseDir);
				ReplaceVarInString(filename,"CC", filename, conn->mCC);
				if (!LoadFileInString(filename,buf)) return 0;
			}
			if (mFlags & eTF_VARS) {
				cTime theTime(server.mTime);
				time_t curr_time;
				struct tm lt;

				time(&curr_time);
				localtime_r(&curr_time, &lt);
				
				theTime -= server.mStartTime;	
				ReplaceVarInString(buf, "PARALL", buf, parall);
				ReplaceVarInString(buf, "PAR1", buf, par1);
				ReplaceVarInString(buf, "END1", buf, end1);
				ReplaceVarInString(buf, "CC", buf, conn->mCC);
				ReplaceVarInString(buf, "IP", buf, conn->AddrIP());
				ReplaceVarInString(buf, "HOST", buf, conn->AddrHost());
				ReplaceVarInString(buf, "USERS", buf, (int)server.mUserList.size());
				ReplaceVarInString(buf, "USERSPEAK", buf, (int)server.mUsersPeak);
				ReplaceVarInString(buf, "UPTIME", buf, theTime.AsPeriod().AsString());
				ReplaceVarInString(buf, "VERSION", buf, VERSION);
				ReplaceVarInString(buf, "HUBNAME", buf, server.mC.hub_name);
				ReplaceVarInString(buf, "HUBTOPIC", buf,server.mC.hub_topic);
				ReplaceVarInString(buf, "VERSION_DATE", buf, __CURR_DATE_TIME__);
				ReplaceVarInString(buf, "NICK", buf, conn->mpUser->mNick);
				ReplaceVarInString(buf, "SHARE", buf, conn->mpUser->mShare);
				ReplaceVarInString(buf, "TOTAL_SHARE", buf,server.mTotalShare);
				ReplaceVarInString(buf, "ss",buf, lt.tm_sec);
				ReplaceVarInString(buf, "mm",buf, lt.tm_min);
				ReplaceVarInString(buf, "HH",buf, lt.tm_hour);
				ReplaceVarInString(buf, "DD",buf, lt.tm_mday);
				ReplaceVarInString(buf, "MM",buf, lt.tm_mon+1);
				ReplaceVarInString(buf, "YY",buf, lt.tm_year);
				
			}

			if (mFlags & eTF_EXECUTE && server.mDBConf.allow_exec) {
				string command(buf);
				filename = server.mConfigBaseDir;
				filename.append("/tmp/trigger.tmp");
				command.append(" > ");
				command.append(filename);
				cout << command << endl;
				system(command.c_str());
				buf = "";
				if (!LoadFileInString(filename,buf)) return 0;
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
			// @CHANGED by dReiska -ENDS-
			return 1;
		}
	}
	return 0;
}

void cTrigger::OnLoad()
{}

ostream &operator << (ostream &os, cTrigger &tr)
{
	os << tr.mCommand << " " << tr.mDefinition.substr(0,64) << " Flags:" << tr.mFlags << " SendAs:" << tr.mSendAs << " Class: " << tr.mMinClass << ".." << tr.mMaxClass;
	return os;
}

};
};
