/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cdcclients.h"
#include "cserverdc.h"
#include "cdcconsole.h"

namespace nDirectConnect
{

namespace nTables
{


cDCClient::cDCClient() :
	mIncertitude(0),
	mPrefixVersionRank(-1),
	mLimitIsPerSlot(false),
	mMinVersion(0.),
	mMaxVersion(100.)
{}

void cDCClient::OnLoad(){}

ostream &operator << (ostream &os, cDCClient &cli)
{
	os << cli.mName << " Versions:" << cli.mMinVersion << ".." << cli.mMaxVersion << " LimitIsPerSlot:" << (cli.mLimitIsPerSlot?"yes":"no");
	return os;
}

cDCClients::cDCClients(cServerDC *server)
        : tDCClientsBase(server->mMySQL, server, "dc_clients")
{}

cDCClients::~cDCClients()
{}

void cDCClients::AddFields()
{
	AddCol("name","varchar(16)","", false, mModel.mName);
	AddPrimaryKey("name");
	AddCol("prefix_regex","varchar(16)","",true,mModel.mPrefixRegex);
	AddCol("prefix_version_rank","tinyint(4)","-1",true, mModel.mPrefixVersionRank);
	AddCol("intag_id","varchar(8)","",true,mModel.mInTagID);
	AddCol("per_slot_limit","tinyint(1)","0", true, mModel.mLimitIsPerSlot);
	AddCol("min_version","int","0", true, mModel.mMinVersion);
	AddCol("max_version","int","100", true, mModel.mMaxVersion);
	mMySQLTable.mExtra =" PRIMARY KEY(name)";
}

bool cDCClients::CompareDataKey(const cDCClient &D1, const cDCClient &D2)
{
	return D1.mName == D2.mName;
}

cDCClientConsole::cDCClientConsole(cDCConsole *console) : tDCClientConsoleBase(console)
{
	this->AddCommands();
}

cDCClientConsole::~cDCClientConsole(){}

const char * cDCClientConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_MOD:
			return "^(\\S+)("
			"( -p ?(\")?((?(3)[^\"]+?|\\S+))(?(3)\"))?|" //[ -p<"prefix_regex">
//			"( -S ?(-?\\d+))?|" //[ -S<max_slots>]
			"( -r ?(-?\\d+))?|" //[ -r<prefix_version_rank>]
			"( -i ?(\\S+))?|" //[ -i<in_tag_id>]
			"( -l ?(0|1))?|" //[ -l<per_slot_limit>]
			"( -v ?(-?[0-9.]+))?|" //[ -v<min_version>]
			"( -V ?(-?[0-9.]+))?|" //[ -V<max_version>]
			")*\\s*$"; // the end of message
		case eLC_DEL:
			return "(\\S+)";
		default: return "";break;
	};
}

bool cDCClientConsole::ReadDataFromCmd(cfBase *cmd, int CmdID, cDCClient &data)
{
	enum{ 	eADD_ALL, eADD_NAME, eADD_PARAM, 
//		eADD_DESCp, eADD_QUOTE, eADD_DESC,
		eADD_PVRp, eADD_PVR,
		eADD_INTAGIDp, eADD_INTAGID,
		eADD_LIMSLOTp, eADD_LIMSLOT,
		eADD_MINVp, eADD_MINV,
		eADD_MAXVp, eADD_MAXV,
		};
		
	cmd->GetParStr(eADD_NAME,data.mName);
	cmd->GetParStr(eADD_INTAGID,data.mInTagID);
	cmd->GetParDouble(eADD_MINV, data.mMinVersion);
	cmd->GetParDouble(eADD_MAXV, data.mMaxVersion);
//	cmd->GetParInt(eADD_MAX_SLOTS, data.mTagMaxSlots);
	return true;
}

cDCClients *cDCClientConsole::GetTheList()
{
	//@todo return mOwner->mServer->mDCClients;
}

const char *cDCClientConsole::CmdSuffix(){ return "client";}
const char *cDCClientConsole::CmdPrefix(){ return "!";}

void cDCClientConsole::ListHead(ostream *os)
{
	*os << "Defined client types are:\r\n";
}

bool cDCClientConsole::IsConnAllowed(cConnDC *conn,int cmd)
{
	return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
}


};

};
