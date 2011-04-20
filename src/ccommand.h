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

#ifndef NCMDRCCOMMAND_H
#define NCMDRCCOMMAND_H
#include "cpcre.h"

using std::ostream;
using std::string;
namespace nVerliHub {
	namespace nCmdr {

	class cCmdr;
/**
describes how commmand should be recognized an then how it should be treated

@author Daniel Muller
*/
class cCommand
{
	friend class cCmdr;
public:
	/**
		A function calling structure for the commander
	*/
	class sCmdFunc
	{
	public:
		string mIdStr;
		string mParStr;
		cPCRE * mIdRex;
		cPCRE * mParRex;
		ostream *mOS;
		cCommand *mCommand;

		void *mExtra;
		sCmdFunc():mIdRex(NULL), mParRex(NULL), mOS(NULL), mCommand(NULL){}

		virtual ~sCmdFunc(){};
		// this is an executive function of a command
		virtual bool operator() (void) = 0;
		virtual bool operator() (cPCRE &idrex, cPCRE &parrex, ostream &os, void *extra)
		{
			mIdRex = &idrex;
			mParRex = &parrex;
			mExtra = extra;
			mOS = &os;
			return operator()();
		}

		virtual void GetSyntaxHelp(ostream &os, cCommand *){};

		virtual bool PartFound(int rank)
		{
			return this->mParRex->PartFound(rank);
		}

		virtual bool GetParStr(int rank, string &dest)
		{
			if(! this->mParRex->PartFound(rank)) return false;
			this->mParRex->Extract(rank, this->mParStr, dest);
			return true;
		}

		virtual bool GetParInt(int rank, int &dest)
		{
			string tmp;
			if (!GetParStr(rank, tmp)) return false;
			dest = atoi(tmp.c_str());
			return true;
		}

		virtual bool GetParBool(int rank, bool &dest)
		{
			string tmp;
			if (!GetParStr(rank, tmp)) return false;
			dest = (tmp == "1") || (tmp == "on") || (tmp == "true") || (tmp == "yes");
			return true;
		}

		virtual bool GetParDouble(int rank, double &dest)
		{
			string tmp;
			if (!GetParStr(rank, tmp)) return false;
			dest = atof(tmp.c_str());
			return true;
		}

		virtual bool GetParLong(int rank, long &dest)
		{
			string tmp;
			if (!GetParStr(rank, tmp)) return false;
			dest = atoi(tmp.c_str());
			return true;
		}

		virtual bool GetIDStr (int rank, string &dest)
		{
			if(! this->mIdRex->PartFound(rank)) return false;
			this->mIdRex->Extract(rank, this->mIdStr, dest);
			return true;
		}

		int StringToIntFromList(const string &str,const char *stringlist[], const int intlist[], int item_count);
	};

	cCommand(int ID, const char *IdRegex, const char *, sCmdFunc*);
	cCommand();
	virtual ~cCommand();
	virtual void Init(int ID, const char *IdRegex, const char *, sCmdFunc*);
	bool TestID(const string &cmd_line);
	bool TestParams();
	bool Execute(ostream &output, void *extrapar);
	int GetID(){ return mID;}
	virtual void Init(void *){};
	virtual void ListCommands(ostream &os);
	void GetParamSyntax(ostream &os);
	cCmdr *mCmdr;
protected:
	int mID;
	nUtils::cPCRE mIdentificator;
	nUtils::cPCRE mParamsParser;
	sCmdFunc *mCmdFunc;
	string mIdStr;
	string mParStr;
	string mIdRegexStr;
	string mParRegexStr;
};

	}; // namespace nCmdr
}; // namespace nVerliHub
#endif
