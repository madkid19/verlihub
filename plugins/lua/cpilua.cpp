/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz)                *
*                    Janos Horvath (bourne at freemail dot hu) 2004-05    *
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
#include "cpilua.h"
#include "src/stringutils.h"
#include "src/cbanlist.h"
#include "src/cserverdc.h"
#include <dirent.h>
#include <string>
#include "src/script_api.h"

namespace nVerliHub {
	using namespace nUtils;
	using namespace nSocket;
	using namespace nPlugin;
	using namespace nMySQL;
	namespace nLuaPlugin {

cpiLua    *cpiLua::me = NULL;
int	  cpiLua::log_level = 1;
cpiLua::cpiLua() : mConsole(this), mQuery(NULL)
{
	mName = LUA_PI_IDENTIFIER;
	mVersion = LUA_PI_VERSION;
	me = this;
}

cpiLua::~cpiLua()
{
	mQuery->Clear();
	delete mQuery;
	this->Empty();
}

bool cpiLua::IsNumber(const char* s)
{
	if (!s || !strlen(s)) return false;
	for (unsigned int i = 0; i < strlen(s); i++)
		switch (s[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': break;
			default: return false;
		}
	return true;
}

void cpiLua::SetLogLevel( int level )
{
	//int old = log_level;
	log_level = level;
	//SetConf("pi_lua", "log_level", o.str().c_str());
}

void cpiLua::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);

	mQuery = new cQuery(server->mMySQL);
	mScriptDir = mServer->mConfigBaseDir + "/scripts/";

	AutoLoad();
}

bool cpiLua::RegisterAll()
{
	RegisterCallBack("VH_OnNewConn");
	RegisterCallBack("VH_OnCloseConn");
	RegisterCallBack("VH_OnParsedMsgChat");
	RegisterCallBack("VH_OnParsedMsgPM");
	RegisterCallBack("VH_OnParsedMsgSearch");
	RegisterCallBack("VH_OnParsedMsgConnectToMe");
	RegisterCallBack("VH_OnParsedMsgRevConnectToMe");
	RegisterCallBack("VH_OnParsedMsgSR");
	RegisterCallBack("VH_OnParsedMsgMyINFO");
	RegisterCallBack("VH_OnParsedMsgValidateNick");
	RegisterCallBack("VH_OnParsedMsgAny");
	RegisterCallBack("VH_OnParsedMsgSupport");
	RegisterCallBack("VH_OnParsedMsgMyPass");
	RegisterCallBack("VH_OnUnknownMsg");
	RegisterCallBack("VH_OnOperatorCommand");
	RegisterCallBack("VH_OnOperatorKicks");
	RegisterCallBack("VH_OnOperatorDrops");
	RegisterCallBack("VH_OnValidateTag");
	RegisterCallBack("VH_OnUserCommand");
	RegisterCallBack("VH_OnUserLogin");
	RegisterCallBack("VH_OnUserLogout");
	RegisterCallBack("VH_OnTimer");
	RegisterCallBack("VH_OnNewReg");
	RegisterCallBack("VH_OnDelReg");
	RegisterCallBack("VH_OnNewBan");
	RegisterCallBack("VH_OnUnBan");
	RegisterCallBack("VH_OnUpdateClass");
	RegisterCallBack("VH_OnHubName");
	return true;
}

bool cpiLua::AutoLoad()
{
	if(Log(0))
		LogStream() << "Open dir: " << mScriptDir << endl;
	string pathname, filename;
	DIR *dir = opendir(mScriptDir.c_str());
	if(!dir) {
		if(Log(1))
			LogStream() << "Error opening directory" << endl;
		return false;
	}
	struct dirent *dent = NULL;

	while(NULL != (dent=readdir(dir))) {
		filename = dent->d_name;
		if((filename.size() > 4) && (StrCompare(filename,filename.size()-4,4,".lua")==0)) {
			pathname = mScriptDir + filename;
			cLuaInterpreter *ip = new cLuaInterpreter(pathname);
			if(ip) {
				if(ip->Init()) {
					AddData(ip);
					ip->Load();
					if(Log(1))
						LogStream() << "Success loading and parsing LUA script: " << filename << endl;
				} else {
					if(Log(1))
						LogStream() << "Failed loading or parsing LUA script: " << filename << endl;
					delete ip;
				}
			}
		}
	}

	closedir(dir);
	return true;
}

bool cpiLua::CallAll(const char * fncname, char * args[])
{
	bool ret = true;
	if(Size()) {
		tvLuaInterpreter::iterator it;
		for(it = mLua.begin(); it != mLua.end(); ++it) {
			if(!(*it)->CallFunction(fncname, args))
				ret = false;
		}
	}
	return ret;
}

bool cpiLua::OnNewConn(cConnDC *conn)
{
	if(conn != NULL) {
		char * args[] = {
					(char *)conn->AddrIP().c_str(),
					NULL
		};
		return CallAll("VH_OnNewConn", args);
	}
	return true;
}

bool cpiLua::OnCloseConn(cConnDC *conn)
{
	if(conn != NULL)
	{
		char * args[] = {
					(char *)conn->AddrIP().c_str(),
					NULL
		};
		return CallAll("VH_OnCloseConn", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgChat(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_CH_MSG).c_str(),
					NULL
		}; // eCH_CH_ALL, eCH_CH_NICK, eCH_CH_MSG
		return CallAll("VH_OnParsedMsgChat", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgPM(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_PM_MSG).c_str(),
					(char *) msg->ChunkString(eCH_PM_TO).c_str(),
					NULL
		}; // eCH_PM_ALL, eCH_PM_TO, eCH_PM_FROM, eCH_PM_CHMSG, eCH_PM_NICK, eCH_PM_MSG
		return CallAll("VH_OnParsedMsgPM", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgSupport(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (msg != NULL)) {
	    	char * args[] = {
		    			(char *)conn->AddrIP().c_str(),
					(char *)msg->mStr.c_str(),
					NULL
		};
		return CallAll("VH_OnParsedMsgSupport", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgMyPass(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_1_ALL).c_str(),
					NULL
		}; // eCH_1_ALL, eCH_1_PARAM
		return CallAll("VH_OnParsedMsgMyPass", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgRevConnectToMe(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_RC_OTHER).c_str(),
					NULL
		};
		return CallAll("VH_OnParsedMsgRevConnectToMe", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgConnectToMe(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_CM_NICK).c_str(),
					(char *)msg->ChunkString(eCH_CM_IP).c_str(),
					(char *)msg->ChunkString(eCH_CM_PORT).c_str(),
					NULL
		}; // eCH_CM_NICK, eCH_CM_ACTIVE, eCH_CM_IP, eCH_CM_PORT
		return CallAll("VH_OnParsedMsgConnectToMe", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgSearch(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_AS_ALL).c_str(),
					NULL
		}; // active: eCH_AS_ALL, eCH_AS_ADDR, eCH_AS_IP, eCH_AS_PORT, eCH_AS_QUERY; passive: eCH_PS_ALL, eCH_PS_NICK, eCH_PS_QUERY
		return CallAll("VH_OnParsedMsgSearch", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgSR(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_SR_ALL).c_str(),
					NULL
		}; // eCH_SR_ALL, eCH_SR_FROM, eCH_SR_PATH, eCH_SR_SIZE, eCH_SR_SLOTS, eCH_SR_SL_FR, eCH_SR_SL_TO, eCH_SR_HUBINFO, eCH_SR_TO
		return CallAll("VH_OnParsedMsgSR", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgMyINFO(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->ChunkString(eCH_MI_ALL).c_str(),
					NULL
		}; // eCH_MI_ALL, eCH_MI_DEST, eCH_MI_NICK, eCH_MI_INFO, eCH_MI_DESC, eCH_MI_SPEED, eCH_MI_MAIL, eCH_MI_SIZE
		return CallAll("VH_OnParsedMsgMyINFO", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgValidateNick(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
					(char *)msg->ChunkString(eCH_1_ALL).c_str(),
					NULL
		}; // eCH_1_ALL, eCH_1_PARAM
		return CallAll("VH_OnParsedMsgValidateNick", args);
	}
	return true;
}

bool cpiLua::OnParsedMsgAny(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL)) {
		char * args[] = {
		    			(char *)conn->mpUser->mNick.c_str(), // send nick instead of IP
					(char *)msg->mStr.c_str(),
					NULL
		};
		return CallAll("VH_OnParsedMsgAny", args);
	}
	return true;
}

bool cpiLua::OnUnknownMsg(cConnDC *conn, cMessageDC *msg)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (msg != NULL) && msg->mStr.size() > 0) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)msg->mStr.c_str(),
					NULL
		};
		return CallAll("VH_OnUnknownMsg", args);
	}
	return true;
}

bool cpiLua::OnOperatorCommand(cConnDC *conn, string *command)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (command != NULL)) {
		if(mConsole.DoCommand(*command, conn))
			return false;

		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)command->c_str(),
					NULL
		};
		return CallAll("VH_OnOperatorCommand", args);
	}
	return true;
}

bool cpiLua::OnOperatorKicks(cUser *OP, cUser *user, string *reason)
{
	if((OP != NULL) && (user !=NULL) && (reason != NULL)) {
		char * args[] = {
					(char *)OP->mNick.c_str(),
					(char *)user->mNick.c_str(),
					(char *)reason->c_str(),
					NULL
		};
		return CallAll("VH_OnOperatorKicks", args);
	}
	return true;
}

bool cpiLua::OnOperatorDrops(cUser *OP, cUser *user)
{
	if((OP != NULL) && (user != NULL)) {
		char * args[] = {
					(char *)OP->mNick.c_str(),
					(char *)user->mNick.c_str(),
					NULL
		};
		return CallAll("VH_OnOperatorDrops", args);
	}
	return true;
}

bool cpiLua::OnUserCommand(cConnDC *conn, string *command)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (command != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)command->c_str(),
					NULL
		};
		return CallAll("VH_OnUserCommand", args);
	}
	return true;
}

bool cpiLua::OnValidateTag(cConnDC *conn, cDCTag *tag)
{
	if((conn != NULL) && (conn->mpUser != NULL) && (tag != NULL)) {
		char * args[] = {
					(char *)conn->mpUser->mNick.c_str(),
					(char *)tag->mTag.c_str(),
					NULL
		};
		return CallAll("VH_OnValidateTag", args);
	}
	return true;
}

bool cpiLua::OnUserLogin(cUser *user)
{
	if(user != NULL) {
		char * args[] = {
					(char *)user->mNick.c_str(),
					NULL
		};
		return CallAll("VH_OnUserLogin", args);
	}
	return true;
}

bool cpiLua::OnUserLogout(cUser *user)
{
	if(user != NULL) {
		char * args[] = {
					(char *)user->mNick.c_str(),
					NULL
		};
		return CallAll("VH_OnUserLogout", args);
	}
	return true;
}

bool cpiLua::OnTimer()
{
	char * args[] = { NULL };
	return CallAll("VH_OnTimer", args);
}

bool cpiLua::OnNewReg(string mNick, int mClass)
{
	char * args[] = {
				(char *) mNick.c_str(),
				 (char *) toString(mClass),
				NULL
	};
	return CallAll("VH_OnNewReg", args);
}

bool cpiLua::OnDelReg(string mNick, int mClass)
{
	ostringstream os;
	os << mClass;
	char * args[] = {
				(char *) mNick.c_str(),
				(char *) toString(mClass),
		 		NULL
	};
	return CallAll("VH_OnDelReg", args);
}

bool cpiLua::OnUpdateClass(string mNick, int oldClass, int newClass)
{
	char * args[] = {
				(char *) mNick.c_str(),
				(char *) toString(oldClass),
				(char *) toString(newClass),
		  		NULL
	};
	return CallAll("VH_OnUpdateClass", args);
}

bool cpiLua::OnNewBan(cBan *ban)
{
	if(ban != NULL) {
		char * args[] = {	(char *)ban->mNickOp.c_str(),
					(char *)ban->mIP.c_str(),
					(char *)ban->mNick.c_str(),
					(char *)ban->mReason.c_str(),
					NULL
		};
		return CallAll("VH_OnNewBan", args);
	}
	return true;
}

bool cpiLua::OnUnBan(string nick, string op, string reason)
{
	char * args[] = {
				(char *) nick.c_str(),
				(char *) op.c_str(),
				(char *) reason.c_str(),
				NULL
	};
	return CallAll("VH_OnUnBan", args);

}
bool cpiLua::OnHubName(string nick, string hubname)
{
	char * args[] = {
				(char *) nick.c_str(),
				(char *) hubname.c_str(),
				NULL
	};
	return CallAll("VH_OnHubName", args);

}

const char * cpiLua::toString(int number)
{
	ostringstream os;
	os << number;
	return os.str().c_str();
}
	}; // namepsace nLuaPlugin
}; // namespace nVerliHub
REGISTER_PLUGIN(nVerliHub::nLuaPlugin::cpiLua);
