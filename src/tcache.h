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
#ifndef NCONFIGTCACHE_H
#define NCONFIGTCACHE_H
#include "thasharray.h"
#include "cconfmysql.h"
#include "ctime.h"

namespace nVerliHub {
	namespace nConfig {
		using namespace nUtils;
		/// @addtogroup Core
		/// @{
		/**
		 * Provide cache support for MySQL table content.
		 * Data of the table are loaded in memory and kept syncronized
		 * to improve performance.
		 * @author Daniel Muller
		 */
		template <class IndexType> class tCache : public cConfMySQL
		{
			public:
				/// Define an hash container of void pointers.
				typedef tHashArray<void*> tHashTab;

				/// Define the hash type.
				typedef tHashTab::tHashType tCacheHash;

				/**
				 * Class constructor.
				 * @param mysql cMySQL instance.
				 * @param tableName The name of the table.
				 * @param indexName
				 * @param dataName
				 */
				tCache(nMySQL::cMySQL &mysql, const char* tableName, const char* indexName, const char* dataName = NULL) :
					cConfMySQL(mysql), mDateName(dataName)
				{
					SetClassName("tCache");
					mMySQLTable.mName = tableName;
					cConfMySQL::Add(indexName,mCurIdx);
					SetBaseTo(this);
					mIsLoaded = false;
				}

				/**
				 * Class destructor.
				 */
				~tCache()
				{
					Clear();
				}

				/**
				 * Add new data to the cache.
				 * @param key The key.
				 */
				void Add(IndexType const &key)
				{
					tCacheHash hash = CalcHash(key);
					mHashTab.AddWithHash(this,hash);
				}

				/**
				* Calculate the hash for the given key.
				* @param key They key.
				* @return The hash of the key.
				*/
				tCacheHash CalcHash(string const &key)
				{
					return mHashTab.HashLowerString(key);
				}

				/**
				 * Clear the cache by removing all items.
				 */
				void Clear()
				{
					mHashTab.Clear();
					mIsLoaded = false;
				}

				/**
				 * Find the data in the cache with the given
				 * key.
				 * @param key The key.
				 * @return True if the data exists or false otherwise.
				 */
				bool Find(IndexType const &key)
				{
					tCacheHash hash = CalcHash(key);
					return mHashTab.ContainsHash(hash);
				}

				/**
				 * Return the time in seconds of last update operation.
				 * @return The last update.
				 */
				unsigned GetLastUpdate() const
				{
					return mLastUpdate.Sec();
				}

				/**
				* Return the time in seconds of last sync operation.
				* @return The last sync.
				*/
				unsigned GetLastSync()
				{
					return mLastSync.Sec();
				}

				/**
				 * Check if data has been loaded from the table.
				 * @return True if data are loaded or false otherwise.
				 */
				bool IsLoaded()
				{
					return mIsLoaded;
				}

				/**
				 * Load data from table to cache.
				 * @return The number of loaded items.
				 */
				int LoadAll()
				{
					SelectFields(mQuery.OStream());
					db_iterator it;
					for(it = db_begin(); it != db_end(); ++it)
						Add(mCurIdx);
					mQuery.Clear();

					if(Log(0))
						LogStream() << mHashTab.Size() << " items preloaded." << endl;
					mIsLoaded = (mHashTab.Size() > 0);
					mLastUpdate.Get();
					return mHashTab.Size();
				}

				/**
				 * Update the syn time.
				 */
				void Sync()
				{
					mLastSync.Get();
				}

				/**
				 * Update the cache.
				 *
				 * Please note that just new data since last
				 * update opeartion are fetched from database.
				 * The old one are not updated.
				 * @return The number of new items.
				 */
				int Update()
				{
					int n = 0;

					SelectFields(mQuery.OStream());
					if(mDateName)
						mQuery.OStream() << " WHERE " << mDateName << " > " << mLastUpdate.Sec();
					for(db_iterator it = db_begin(); it != db_end(); ++it) {
						if(!Find(mCurIdx))
							Add(mCurIdx);
						n++;
					}
					if(n && Log(0))
						LogStream() << "Updated cache for table " << mMySQLTable.mName << " [Total items: " << mHashTab.Size() << " | New items: " << n << "]" << endl;
					mQuery.Clear();
					mLastUpdate.Get();
					return n;
				}
			protected:
				/// Cache container.
				tHashTab mHashTab;

				/// True if data are loaded.
				bool mIsLoaded;

				/// Time of last update operation.
				/// @see Update()
				cTime mLastUpdate;

				/// Time of last sync operation.
				/// @see Sync()
				cTime mLastSync;

				const char *mDateName;
			private:
				IndexType mCurIdx;
		};
		/// @}
	}; // namespace nConfig
}; // namespace nVerliHub

#endif
