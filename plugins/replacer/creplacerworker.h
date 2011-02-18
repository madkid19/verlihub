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
 ***************************************************************************/
#ifndef NDIRECTCONNECTCREPLACERWORKER_H
#define NDIRECTCONNECTCREPLACERWORKER_H

#include <string>
#include "src/cpcre.h"

using namespace std;
using namespace nUtils;

namespace nDirectConnect {

class cConnDC;
class cServerDC;

namespace nTables {

/**
@author Daniel Muller
@changes for Replacer Pralcio
*/

class cReplacerWorker
{
public:
	cReplacerWorker();
	~cReplacerWorker();
	bool CheckMsg(const string &msg);
	bool PrepareRegex();
	// the word to change
	string mWord;
	// the word that will replace mWord
	string mRepWord;
	// affected class
	int mAfClass;
private:
	cPCRE mRegex;
};
};
};

#endif
