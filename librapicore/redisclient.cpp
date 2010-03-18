/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
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
 **************************************************************************/
#include "redisclient.h"
#include "redis/credis.h"

namespace Rapi
{

/** Redis connection handle */
REDIS mRedisCon = NULL;
const int REDIS_TIMEOUT = 10000;

//-----------------------------------------------------------------------------
CRedisClient::CRedisClient( std::string hostname, int port )
{
  mHostName = hostname;
  mPort = port;
  mRedisCon = credis_connect(hostname.c_str(), port, REDIS_TIMEOUT);
}
//-----------------------------------------------------------------------------
CRedisClient::~CRedisClient()
{
  credis_close(mRedisCon);
}
//-----------------------------------------------------------------------------
CRedisClient* CRedisClient::getInstance(std::string hostname, int port)
{
  static CRedisClient* instance = NULL;

  if ( instance == NULL )
    instance = new CRedisClient( hostname, port );

  return instance;
}
//-----------------------------------------------------------------------------
int CRedisClient::ping()
{
  int r = -1;

  if (mRedisCon)
    r = credis_ping(mRedisCon);

  if (r == 0)
    return 1; // success

  return 0; // failure
}
//-----------------------------------------------------------------------------
int CRedisClient::printInfo()
{
  REDIS_INFO info;

  if (not mRedisCon)
    return 0; // failure

  if (credis_info(mRedisCon, &info) != 0) {
    PRT_ERR2("No answer from Redis server %s:%d", mHostName.c_str(), mPort);
    return 0; // failure
  }

  printf("REDIS Information: \n"                   \
         "  version: %s\n"                         \
         "  uptime in seconds: %d\n"               \
         "  uptime in days: %d\n"                  \
         "  connected clients: %d\n"               \
         "  connected slaves: %d\n"                \
         "  used memory: %u\n"                     \
         "  changes since last save: %lld\n"       \
         "  bgsave in progress: %d\n"              \
         "  last save time: %d\n"                  \
         "  total connections received: %lld\n"    \
         "  total commands processed: %lld\n"      \
         "  role: %d\n",
         info.redis_version,
         info.uptime_in_seconds,
         info.uptime_in_days,
         info.connected_clients,
         info.connected_slaves,
         info.used_memory,
         info.changes_since_last_save,
         info.bgsave_in_progress,
         info.last_save_time,
         info.total_connections_received,
         info.total_commands_processed,
         info.role);
  return 1; // success
}
//-----------------------------------------------------------------------------
int CRedisClient::set(const std::string key, const std::string value)
{
  int r;

  if (mRedisCon) {
    r = credis_set( mRedisCon, key.c_str(), value.c_str() );
    if (r == 0)
      return 1; // succes
  }
  return 0; // failure
}
//-----------------------------------------------------------------------------
int CRedisClient::get(const std::string key, std::string& value)
{
  int ret;
  char* val;

  if (mRedisCon) {
    ret = credis_get( mRedisCon, key.c_str(), &val );
    if (ret == 0) {
      value = val;
      return 1; // succes
    }
  }
  return 0; // failure
}
//-----------------------------------------------------------------------------

} // namespace
