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
#ifndef CDCTAG_H
#define CDCTAG_H
#include <string>
#include "cdcclient.h"
#include "cserverdc.h"

using namespace std;
/// VerliHub namespace that contains all classes.
namespace nVerliHub {
	namespace nEnums {
		/**
		 * Identify the mode used by a user as corresponding in client tag.
		 */
		typedef enum
		{
			/// The user sends no tag in $MyINFO to the hub
			eCM_NOTAG,
			/// The user sends a valid tag in $MyINFO and client is in active mode
			eCM_ACTIVE,
			/// The user sends a valid tag in $MyINFO and client is in passive mode
			eCM_PASSIVE,
			/// The user sends a valid tag in $MyINFO and client is connected
			/// to the hub through a proxy
			eCM_SOCK5
		} tClientMode;

		/// Used to signal that an error occurred while validating the tag of the client.
		/// These error codes are returned in response to a cDCTag::ValidateTag() call.
		/// For example the user has too many open hubs or client version is not valid.
		enum {
			/// The client of the user is banned
			eTC_BANNED,
			/// The client of the user is unknown
			eTC_UNKNOWN,
			/// The tag cannot be parsed
			eTC_PARSE,
			/// User has too many open hubs
			eTC_MAX_HUB,
			// user has too few open hubs
			eTC_MIN_HUB,
			// user has too few open hubs as user
			eTC_MIN_HUB_USR,
			// user has too few open hubs as registered user
			eTC_MIN_HUB_REG,
			// user has too few open hubs as operator
			eTC_MIN_HUB_OP,
			/// User has too many open slots
			eTC_MAX_SLOTS,
			/// User has too few open slots
			eTC_MIN_SLOTS,
			/// The ratio between open hubs and slots is too high
			eTC_MAX_HS_RATIO,
			/// The client is limiting the upload bandwidth
			/// and the limit is too low
			eTC_MIN_LIMIT,
			/// The upload limit per slot is too low
			eTC_MIN_LS_RATIO,
			/// The client version is not valid
			/// and too old
			eTC_MIN_VERSION,
			/// The client version is not valid
			/// and too recent
			eTC_MAX_VERSION,
			/// The client is connected to the hub through a
			/// proxy and it is not allowed
			eTC_SOCK5,
			/// The client is in passive mode
			/// but passive connections are restricted
			eTC_PASSIVE,
		};
	}

	namespace nTables {
		class cDCConf;
		class cConnType;
	};
	using namespace nTables;
	/// @addtogroup Core
	/// @{
	/**
	 * @brief This class represents a tag of a client that is sent in $MyINFO protocol message..
	 * A tag looks like this <++ V:0.00,S:0,H:1>.
	 * Every token of the tag are parsed and st*ored inside
	 * an instance of this class. The parsing process is
	 * triggered by cDCTag::ValidateTag() call.
	 * @author Daniel Muller
	 * @author Davide Simoncelli
	 */
	class cDCTag
	{
		public:
			/**
			 * Class constructor.
			 * @param mS A pointer to cServerDC instance.
			 */
			cDCTag(nSocket::cServerDC *mS);

			/**
			 * Class constructor.
			 * @param mS A pointer to cServerDC instance.
			 * @param c A pointer to cDCClient instance.
			 */
			cDCTag(nSocket::cServerDC *mS, cDCClient *c);

			/**
			 * Class destructor.
			 */
			~cDCTag();

			/**
			 * Validate the tag of the client and identify it.
			 * @param os A stream that contains an error message after validation.
			 * @param conn_type The type of the connection of the user.
			 * @param code An integer that contains an error code after validation.
			 * @return The parsing result
			 */
			bool ValidateTag(ostream &os, cConnType *conn_type, int &code);

			/**
			 * Friend << operator to output a cDCTag instance.
			 * @param os A stream that should contains the output.
			 * @param tag An instance of this clas.
			 */
			friend ostream &operator << (ostream&os, cDCTag &tag);

			/// Instance of cServerDC class that is used
			/// to access VerliHub configuration.
			nSocket::cServerDC *mServer;

			/// Instance of cDCClient class that describes
			/// the client of the user.
			cDCClient *client;

			/// The tag of the client.
			string mTag;

			/// The identification of the client inside the tag.
			string mTagID;

			/// The version of the client.
			double mClientVersion;

			/// The number of open hubs.
			int mTotHubs;

			// the number of open hubs as user
			int mHubsUsr;

			// the number of open hubs as registered user
			int mHubsReg;

			// the number of open hubs as operator
			int mHubsOp;

			/// The number of open slots.
			int mSlots;

			/// The upload bandwidth limit.
			int mLimit;

			/// The mode of the client.
			tClientMode mClientMode;

			/// The body of the tag (the string after the mode part)
			string mTagBody;
	};
	/// @}
}; // namespace nVerliHub
#endif
