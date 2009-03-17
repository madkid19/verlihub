#include "cdccommand.h"
#include "cdcproto.h"
#include "cdcconsole.h"
#include "cserverdc.h"

using namespace ::nCmdr;
using namespace ::nDirectConnect;
using namespace ::nDirectConnect::nProtocol;

namespace nDirectConnect
{
	cDCCommand::cDCCommand(){};
	cDCCommand::cDCCommand(int ID, const char *IdRegex, const char *ParRegex, sDCCmdFunc *CmdFunc, long Action) :
		cCommand(ID, IdRegex, ParRegex, CmdFunc), mActionType(Action)
	{}

	bool cDCCommand::sDCCmdFunc::GetIDEnum(int rank, int &id, const char *ids[], const int enums[])
	{
		string tmp;
		if (!GetIDStr(rank, tmp)) return false;
		int i;

		for(i = 0; ids[i] != NULL; i++)
		{
			if (tmp == ids[i])
			{
				id = enums[i];
				return true;
			}
		}
		return false;
	}

	bool cDCCommand::sDCCmdFunc::GetParUnEscapeStr(int rank, string &dest)
	{
		string tmp;
		if (!GetParStr(rank, tmp)) return false;
		cDCProto::UnEscapeChars(tmp, dest);
		return true;
	}

	bool cDCCommand::sDCCmdFunc::GetParIPRange(int rank, unsigned long &ipmin, unsigned long &ipmax)
	{
		string tmp;
		if (!GetParStr(rank, tmp)) return false;
		cDCConsole::GetIPRange(tmp, ipmin, ipmax);
		return true;
	}

	bool cDCCommand::sDCCmdFunc::GetParRegex(int rank, string &dest)
	{
		string tmp;
		if (!GetParUnEscapeStr(rank, tmp)) return false;
		cPCRE rex;
		if (rex.Compile(tmp.c_str(),0))
		{
			dest = tmp;
			return true;
		}
		return false;
	}

	bool cDCCommand::sDCCmdFunc::GetParOnlineUser(int rank, cUser *&dest, string &nick)
	{
		if (!GetParUnEscapeStr(rank, nick)) return false;
		dest = mS->mUserList.GetUserByNick(nick);
		return true;
	}

	void cDCCommand::sDCCmdFunc::Bind(cDCConsoleBase *co)
	{
		mS = co->mOwner; mCo = co;
	}

	bool cDCCommand::sDCCmdFunc::operator() (cPCRE &idrex, cPCRE &parrex, ostream &os, void *extra)
	{
		mConn = (cConnDC*) extra;
		if (!mConn ||
			!mConn->mpUser /*||
			(
			//mCommand //&&
			// @todo ((cDCCmdBase*)(mCommand)->mActionType >= 0) &&
			//!mConn->mpUser->Can((cDCCmdBase*)(mCommand)->mActionType)
			)*/
		) return false;
		// call the inherited parent's implementation of the () operator
		return this->sCmdFunc::operator ()(idrex, parrex, os, extra);
	}


	void cDCCommand::Init(void *co)
	{
		((sDCCmdFunc*)mCmdFunc)->Bind((cDCConsoleBase *)co);
	}

	void cDCCommand::Init(int ID, const char *IdRegex, const char *ParRegex, sCmdFunc*cf)
	{
		cCommand::Init(ID, IdRegex, ParRegex, cf);
	}

};
