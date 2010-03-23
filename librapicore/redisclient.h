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
#ifndef CREDISCLIENT_H
#define CREDISCLIENT_H

#include "printerror.h"
#include <string>


namespace Rapi
{

/**
 * RAPI interface class to a REDIS server. This class uses credis to access
 * the server. A singelton pattern makes sure we only have one client
 * @author Jens Wawerla
 */
class CRedisClient
{
  public:
    /** Default destructor */
    virtual ~CRedisClient();
    /**
     * Gets the only instance of this client class
     * @param hostname name of server
     * @param port of server
     * @return only instance of this class
     */
    static CRedisClient* getInstance( std::string hostname="", int port=0);
    /**
     * Prints the server information
     * @return 1 if success, 0 otherwise
     */
    int printInfo();
    /**
     * pings the redis server
     * @return 1 if success, 0 otherwise
     */
    int ping();
    /**
     * Sets a string value
     * @param key
     * @param value
     * @return 1 if success, 0 otherwise
     */
    int set(const std::string key, const std::string value);
    /**
     * Gets a string value
     * @param key
     * @param value
     * @return 1 if success, 0 otherwise
     */
    int get(const std::string key, std::string& value);
    /**
     * Increment a counter
     * @param key name of counter
     * @return newValue of counter
     * @return 1 if success, 0 otherwise
     */
    int increment(const std::string key, int& newValue);
    /**
     * Increment a counter
     * @param key name of counter
     * @return newValue of counter
     * @return 1 if success, 0 otherwise
     */
    int decrement(const std::string key, int& newValue);

  protected:
    /**
     * Default constructor
     * @param hostname of server
     * @param port of server
     */
    CRedisClient( std::string hostname, int port );

  private:
    /** Hostname of server */
    std::string mHostName;
    /** Port number of server */
    int mPort;
};

} // namespace
#endif // CREDISCLIENT_H
