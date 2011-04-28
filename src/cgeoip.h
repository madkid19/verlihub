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
#ifndef NUTILSCGEOIP_H
#define NUTILSCGEOIP_H
#include <string>
#include <GeoIP.h>

using std::string;
namespace nVerliHub {
	namespace nUtils {
		/// @addtogroup Core
		/// @{
		/**
		 * Wrapper class for geoip library.
		 * This class is used to get the country code of an
		 * IP address.
		 * @author Daniel Muller
		 */
		class cGeoIP
		{
			public:
				/**
				* Class constructor.
				*/
				cGeoIP();

				/**
				* Class destructor.
				*/
				~cGeoIP();

				/**
				* Return the country code for the given hostname.
				* @param hostname The hostname.
				* @param CC String where to store country code.
				* @return True if it is possible to get country code, false otherwise.
				*/
				bool GetCC(const string &hostname, string &cc);
			private:
				/// Pointer to GeoIP instance.
				GeoIP * mGI;
		};
		/// @}
	}; // namespace nUtils
}; // namespace nVerliHub

#endif
