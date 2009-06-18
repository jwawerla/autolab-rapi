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
#ifndef RAPIVARIABLEMONITOR_H
#define RAPIVARIABLEMONITOR_H

#include "rapitypesystem.h"

namespace Rapi
{

/**
 * Helps to debug robot controllers by monitoring registered variables
 * @author Jens Wawerla
 */
class CVariableMonitor : public CRapiTypeSystem
{
  public:
    /** Default constructor */
    CVariableMonitor();
    /** Default destructor */
    ~CVariableMonitor();

    /**
     * Gets the variable with a given index as a string
     * @param index of variable to get
     * @return variable variable type, name and value
     */
    void getVariableString ( unsigned int index, std::string& varType,
                             std::string& name, std::string& value );
    /**
     * Gets the number of monitored variables
     * @return number of variables
     */
    unsigned int getNumOfVariables() { return mVarList.size(); };



};

}// namespace

#endif
