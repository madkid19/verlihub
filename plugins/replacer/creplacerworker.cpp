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
#include "creplacerworker.h"
#include "src/cconndc.h"
#include "src/cserverdc.h"

namespace nDirectConnect {

cReplacerWorker::cReplacerWorker()
{
	mAfClass = 4; // default affected class is <= 4 (operator)
}

cReplacerWorker::~cReplacerWorker()
{
}

bool cReplacerWorker::CheckMsg(const string &msg)
{
	return (0 < mRegex.Exec(msg));
}

bool cReplacerWorker::PrepareRegex()
{
	return mRegex.Compile(mWord.data(),PCRE_CASELESS);
}

};
