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
#include "cdcclient.h"
#include <iomanip>
#include "i18n.h"

namespace nVerliHub {
	namespace nTables {

		cDCClient::cDCClient()
		{
			mName = "Unknown";
			mEnable = 1;
			mMinVersion = -1;
			mMaxVersion = -1;
			mBan = false;
		}

		cDCClient::~cDCClient() {}

		ostream &operator << (ostream &os, cDCClient &tr)
		{
			os << " ";
			os << setw(15) << setiosflags(ios::left) << tr.mName;
			os << setw(15) << setiosflags(ios::left) << tr.mTagID;
			os << setw(30) << setiosflags(ios::left);
 			if(tr.mMinVersion < 0 && tr.mMaxVersion < 0)
 				os << _("All version");
 			else if(tr.mMinVersion >= 0 && tr.mMaxVersion < 0)
 				os << autosprintf(_("Min version: %.2f"), tr.mMinVersion);
 			else if(tr.mMinVersion < 0 && tr.mMaxVersion >= 0)
				os << autosprintf(_("Max version: %.2f"), tr.mMaxVersion);
 			else
 				os << tr.mMinVersion << "-" << tr.mMaxVersion;
			os << setw(15) << setiosflags(ios::left) << (tr.mBan ? _("Yes") : _("No"));
 			os << (tr.mEnable ? _("Enable") : _("Disable"));
			return os;
		}
	}; // namespace nTables
}; // namespace nVerliHub

