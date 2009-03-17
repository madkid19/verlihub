/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cconntypes.h"
#include "cserverdc.h"
#include "cdcconsole.h"

namespace nDirectConnect
{

namespace nTables
{

cConnType::cConnType() :
	mTagMaxSlots(100),
	mTagMinSlots(0),
	mTagMinLimit(-1.),
	mTagMinLSRatio(-1.)
{}

cConnType::~cConnType (){}
void cConnType::OnLoad(){}

ostream &operator << (ostream &os, cConnType &ct)
{
	os.width(15);
	os << ct.mIdentifier;
       	os.width(0);
	os << " : Slots: " << 
		ct.mTagMinSlots << ".." << ct.mTagMaxSlots << " Min limiter: " <<
		ct.mTagMinLimit << " , " << ct.mTagMinLSRatio << "/slot" << " - " << ct.mDescription ;
	return os;
}


cConnTypes::cConnTypes(cServerDC *server)
        : tConnTypesBase(server->mMySQL, server, "conn_types")
{}


cConnTypes::~cConnTypes() {}

void cConnTypes::AddFields()
{
	AddCol("identifier","varchar(16)","", false, mModel.mIdentifier);
	AddPrimaryKey("identifier");
	AddCol("description","varchar(64)","no description",true,mModel.mDescription);
	AddCol("tag_min_slots","tinyint(4)", "0", true, mModel.mTagMinSlots); 
	AddCol("tag_max_slots","tinyint(4)", "100", true, mModel.mTagMaxSlots); 
	AddCol("tag_min_limit","double", "-1", true, mModel.mTagMinLimit); 
	AddCol("tag_min_ls_ratio","double", "-1", true, mModel.mTagMinLSRatio); 
	mMySQLTable.mExtra =" PRIMARY KEY(identifier)";
}

bool cConnTypes::CompareDataKey(const cConnType &D1, const cConnType &D2)
{
	return D1.mIdentifier == D2.mIdentifier;
}

cConnType *cConnTypes::FindConnType(const string &identifier)
{
	iterator it;
	cConnType *ConnType, *DefaultType = NULL;
	for( it=begin(); it != end(); ++it)
	{
		ConnType = *it;
		if(ConnType->mIdentifier == identifier)
			return ConnType;
		if(ConnType->mIdentifier == "default")
			DefaultType = ConnType;
	}
	if(DefaultType != NULL) return DefaultType;
	return &mModel;
}

cConnTypeConsole::cConnTypeConsole(cDCConsole *console) : tConnTypeConsoleBase(console)
{
	this->AddCommands();
}

cConnTypeConsole::~cConnTypeConsole()
{}

void cConnTypeConsole::GetHelpForCommand(int cmd, ostream &os)
{
	string help_str;
	switch(cmd)
	{
		case eLC_LST: 
		help_str = "!lstconntype\r\nGive a list of registered connection types"; 
		break;
		case eLC_ADD: 
		case eLC_MOD:
		help_str = "!(add|mod)conntype <type>[ -d <\"desc\">][ -S <max_slots>][ -s <min_slots>][ -l <min_limiter>][ -ls <min_ls_ratio>]\r\n"
		"      add or edit a connection type\r\n"
		"     * type - textual part of the connection type name\r\n"
		"     * max_slots, min_slots - slot settings\r\n"
		"     * desc - for your info\r\n"
		"     * min_limiter - minimum value for upload limiter (decimal)\r\n"
		"     * min_ls_ratio - minimum upload per slot";
		break;
		case eLC_DEL:
		help_str = "!delconntype <type>"; break;
		default: break;
	}
	
	cDCProto::EscapeChars(help_str,help_str);
	os << help_str;
}

const char * cConnTypeConsole::GetParamsRegex(int cmd)
{
	switch(cmd)
	{
		case eLC_ADD:
		case eLC_MOD:
			return "^(\\S+)("
			"( -d ?(\")?((?(3)[^\"]+?|\\s+))(?(3)\"))?|" //[ -d<description>|"<<description>>]"
			"( -S ?(-?\\d+))?|" //[ -S<max_slots>]
			"( -s ?(-?\\d+))?|" //[ -s<min_slots>]
			"( -l ?(-?[0-9.]+))?|" //[ -l<min_limit>]
			"( -ls ?(-?[0-9.]+))?|" // [ -ls<min_ls_ratio>]
			")*\\s*$"; // the end of message
		case eLC_DEL:
			return "(\\S+)";
		default: return "";break;
	};
}

bool cConnTypeConsole::ReadDataFromCmd(cfBase *cmd, int CmdID, cConnType &data)
{
	enum{ 	eADD_ALL, eADD_IDENT, 
		eADD_PARAM, 
		eADD_DESCp, eADD_QUOTE, eADD_DESC,
		eADD_MAX_SLOTSp, eADD_MAX_SLOTS,
		eADD_MIN_SLOTSp, eADD_MIN_SLOTS,
		eADD_MIN_LIMITp, eADD_MIN_LIMIT,
		eADD_MIN_LS_RATIOp, eADD_MIN_LS_RATIO
		};
		
	cmd->GetParStr(eADD_IDENT,data.mIdentifier);
	cmd->GetParStr(eADD_DESC, data.mDescription);
	cmd->GetParInt(eADD_MAX_SLOTS, data.mTagMaxSlots);
	cmd->GetParInt(eADD_MIN_SLOTS, data.mTagMinSlots);
	cmd->GetParDouble(eADD_MIN_LIMIT, data.mTagMinLimit);
	cmd->GetParDouble(eADD_MIN_LS_RATIO, data.mTagMinLSRatio);
	return true;
}

cConnTypes *cConnTypeConsole::GetTheList()
{
	return mOwner->mServer->mConnTypes;
}

const char *cConnTypeConsole::CmdSuffix(){ return "conntype";}
const char *cConnTypeConsole::CmdPrefix(){ return "!";}

void cConnTypeConsole::ListHead(ostream *os)
{
	*os << "Existing connection types are:\r\n";
}

bool cConnTypeConsole::IsConnAllowed(cConnDC *conn,int cmd)
{
	return (conn && conn->mpUser && conn->mpUser->mClass >= eUC_ADMIN);
}


};

};
