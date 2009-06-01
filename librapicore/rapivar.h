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

#ifndef RAPI_RAPIVAR
#define RAPI_RAPIVAR

#include <string>

namespace Rapi
{

/**
 * Interface class for classes that should be monitored with CVariableMonitor
 * @author Jens Wawerla
 */
class IRapiVar
{
  public:
    /** Default destructor */
    virtual ~IRapiVar() {}
    /**
     * Gets the data of the varible as a string
     * @return string data
     */
    virtual std::string toStr() const = 0;
    /**
     * Gets the variable type string
     * @return variable type name
     */
    virtual std::string getTypeStr() const = 0;

  protected:
    /** Default constructor */
    IRapiVar() {};
};

} // namespace

#endif