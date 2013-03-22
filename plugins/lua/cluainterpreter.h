/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz)                *
*                    Janos Horvath (bourne at freemail dot hu) 2004-05    *
*                                                                         *
*   Copyright (C) 2006-2013 by Verlihub Project                           *
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
#ifndef NSCRIPTSCLUAINTERPRETER_H
#define NSCRIPTSCLUAINTERPRETER_H

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "src/cconndc.h"
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
namespace nVerliHub {
	namespace nLuaPlugin {

class cLuaInterpreter
{
public:
	cLuaInterpreter(string);
	~cLuaInterpreter();

	bool Init();
	void ReportLuaError(char *);
	bool CallFunction(const char *, char * [], cConnDC *conn = NULL);
	void RegisterFunction(const char *, int (*)(lua_State *));
	void Load();

	string mScriptName;

	struct mScriptBot {
		char *uNick;
		char *uMyINFO;
		int uShare;
		int uClass;
	};

	typedef vector<mScriptBot *> tvBot;
	tvBot botList;

	void addBot(char *Nick, char *MyINFO, int Share, int Class) {
		bool found = false;

		for (unsigned int i = 0; i < botList.size(); i++) {
			if (strcmp(botList[i]->uNick, Nick) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			mScriptBot *temp = new mScriptBot;
			temp->uNick = Nick;
			temp->uMyINFO = MyINFO;
			temp->uShare = Share;
			temp->uClass = Class;
			botList.push_back(temp);
		}
	}

	void editBot(char *Nick, char *MyINFO, int Share, int Class) {
		mScriptBot *bot = NULL;

		for (unsigned int i = 0; i < botList.size(); i++) {
			if (strcmp(botList[i]->uNick, Nick) == 0) {
				bot = botList[i];
				break;
			}
		}

		if (bot != NULL) {
			// dont need to set nick
			bot->uMyINFO = MyINFO;
			bot->uShare = Share;
			bot->uClass = Class;
		}
	}

	void delBot(char *Nick) {
		for (unsigned int i = 0; i < botList.size(); i++) {
			if (strcmp(botList[i]->uNick, Nick) == 0) {
				botList.erase(botList.begin() + i);
				break;
			}
		}
	}

	void clean() {
		tvBot::iterator it;

		for (it = botList.begin(); it != botList.end(); ++it) {
			if (*it != NULL) delete *it;
			*it = NULL;
		}

		botList.clear();
	}

	lua_State *mL;
};

	}; // namespace nLuaPlugin
}; // namespace nVerliHub
#endif
