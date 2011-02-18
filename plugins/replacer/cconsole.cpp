/***************************************************************************
 *   Copyright (C) 2004 by Pralcio                                         *
 *   based on "Forbid" code made by                                        *
 *   Daniel Muller                                                         *
 *   dan at verliba dot cz                                                 *
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

#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "cconsole.h"
#include "cpireplace.h"
#include "creplacer.h"

using namespace nDirectConnect;

namespace nReplace
{

cConsole::cConsole(cpiReplace *replace) :
	mReplace(replace),
	/** syntax : !addreplacer word replace class  */
	mCmdReplaceAdd(1,"!addreplacer ", "(.*[^ \\s]) (.*[^ \\d])( \\d+)?", &mcfReplaceAdd),
	mCmdReplaceGet(0,"!getreplacer", "", &mcfReplaceGet),
	mCmdReplaceDel(2,"!delreplacer ", "(.*)", &mcfReplaceDel),
	mCmdr(this)
{
	mCmdr.Add(&mCmdReplaceAdd);
	mCmdr.Add(&mCmdReplaceDel);
	mCmdr.Add(&mCmdReplaceGet);
}


cConsole::~cConsole()
{}

int cConsole::DoCommand(const string &str, cConnDC * conn)
{
	ostringstream os;
	if(mCmdr.ParseAll(str, os, conn) >= 0)
	{
		mReplace->mServer->DCPublicHS(os.str().data(),conn);
		return 1;
	}
	return 0;
}

bool cConsole::cfGetReplacer::operator ( )()
{
	string word;
	string rep_word;
	cReplacerWorker *fw;
	(*mOS) << "Replaced words: " << "\r\n";
	for(int i = 0; i < GetPI()->mReplacer->Size(); i++)
	{
		fw = (*GetPI()->mReplacer)[i];
		cDCProto::EscapeChars(fw->mWord, word);
		cDCProto::EscapeChars(fw->mRepWord, rep_word);
		(*mOS) << word << " ---> " << rep_word << "  Affected: " << fw->mAfClass <<  "\r\n";
	}

	return true;
}

bool cConsole::cfDelReplacer::operator ( )()
{
	string word, word_backup;
	GetParStr(1,word_backup);
	cDCProto::UnEscapeChars(word_backup, word);

	bool isInList=false;

	for(int i = 0; i < GetPI()->mReplacer->Size(); i++)
		if((* GetPI()->mReplacer)[i]->mWord == word)
			isInList = true;

	if(!isInList)
	{
		(*mOS) << "Replaced word: " << word_backup << " is NOT in list, so couldn't delete!" << "\r\n";
		return false;
	}

	cReplacerWorker FWord;
	FWord.mWord = word;

	GetPI()->mReplacer->DelReplacer(FWord);
	(*mOS) << "Replaced word: " << word_backup << " deleted." << "\r\n";

	GetPI()->mReplacer->LoadAll();
	return true;
}

bool cConsole::cfAddReplacer::operator ( )()
{
	string word, word_backup, rep_word;
	GetParStr(1,word_backup);
	cReplacerWorker FWord;
	string num;
	GetParStr(2,rep_word);

	/** third parameter is the affected class */
	if(this->GetParStr(3,num))
	{
		istringstream is(num);
		is >> FWord.mAfClass;
	}

	cPCRE TestRE;
	// regex can't be lowecased
	//transform(word.begin(), word.end(), word.begin(), ::tolower);

	cDCProto::UnEscapeChars(word_backup, word);
	if (!TestRE.Compile(word.data(), 0))
	{
		(*mOS) << "Sorry the regular expression you provided did not parse well\r\n";
		return false;
	}

	for(int i = 0; i < GetPI()->mReplacer->Size(); i++)
	{
	    if((*GetPI()->mReplacer)[i]->mWord == word)
	    {
	        (*mOS) << "Replacer word: " << word << " already in list! NOT added!" << "\r\n";
	        return false;
	    }
	}

	FWord.mWord = word;
	FWord.mRepWord = rep_word;
	string ch, cl;

	if (GetPI()->mReplacer->AddReplacer(FWord))
	{

		switch(FWord.mAfClass)
		{
			case 1: cl = "normal"; break;
			case 2: cl = "vip"; break;
			case 3: cl = "cheef"; break;
			case 4: cl = "operator"; break;
			case 5: cl = "admin"; break;
			case 10: cl = "master"; break;
			default: cl = "operator"; break;
		}

		(*mOS) << "Replacer word: " << word_backup << " added! This word will be filtered in public chat for users with class that is less than or equal with " << cl << " class" << "\r\n";
	}
	else
	    (*mOS) << "Replacer word: " << word_backup << " NOT added!" << "\r\n";

	GetPI()->mReplacer->LoadAll();

	return true;
}


};
