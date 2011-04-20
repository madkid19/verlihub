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

#include "src/cconndc.h"
#include "src/cserverdc.h"
#include "src/i18n.h"
#include "cconsole.h"
#include "cpireplace.h"
#include "creplacer.h"
#include "src/stringutils.h"

using namespace nStringUtils;
using namespace ;

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
	if(mCmdr.ParseAll(str, os, conn) >= 0) {
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
	(*mOS) << _("Replaced words:") << "\r\n";

	(*mOS) << "\n ";
	(*mOS) << setw(20) << setiosflags(ios::left) << "Word";
	(*mOS) << setw(20) << setiosflags(ios::left) << toUpper(_("Replace with"));
	(*mOS) << toUpper(_("Affected class")) << "\n";
	(*mOS) << " " << string(20+20+15,'=') << endl;
	for(int i = 0; i < GetPI()->mReplacer->Size(); i++) {
		fw = (*GetPI()->mReplacer)[i];
		cDCProto::EscapeChars(fw->mWord, word);
		cDCProto::EscapeChars(fw->mRepWord, rep_word);
		(*mOS) << " " << setw(20) << setiosflags(ios::left) << word.c_str();
		(*mOS) << setw(20) << setiosflags(ios::left) << rep_word.c_str();
		(*mOS) << fw->mAfClass << endl;
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

	if(!isInList) {
		(*mOS) << autosprintf(_("Word '%s' does not exist."), word_backup.c_str()) << " ";
		return false;
	}

	cReplacerWorker FWord;
	FWord.mWord = word;

	GetPI()->mReplacer->DelReplacer(FWord);
	(*mOS) << autosprintf(_("Word '%s' deleted."), word_backup.c_str()) << " ";

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
	if(this->GetParStr(3,num)) {
		istringstream is(num);
		is >> FWord.mAfClass;
	}

	cPCRE TestRE;
	cDCProto::UnEscapeChars(word_backup, word);
	if(!TestRE.Compile(word.data(), 0)) {
		(*mOS) << _("Sorry the regular expression you provided cannot be parsed.") << " ";
		return false;
	}

	for(int i = 0; i < GetPI()->mReplacer->Size(); i++) {
		if((*GetPI()->mReplacer)[i]->mWord == word) {
			(*mOS) << autosprintf(_("Word '%s' already exists."), word.c_str()) << " ";
			return false;
		}
	}

	FWord.mWord = word;
	FWord.mRepWord = rep_word;
	string ch, cl;

	if(GetPI()->mReplacer->AddReplacer(FWord))
		(*mOS) << autosprintf(_("Added word %s. This word will be filtered in public chat for users with class that is less than or equal to %d."), word_backup.c_str(), FWord.mAfClass) << " ";
	else
	    (*mOS) << autosprintf(_("Error adding word '%s'."), word_backup.c_str()) << " ";

	GetPI()->mReplacer->LoadAll();

	return true;
}

};
