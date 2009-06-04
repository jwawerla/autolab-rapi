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

#include <string>
#include <vector>

#include "rapivarinterface.h"
#include "pose2d.h"
#include "point2d.h"
#include "velocity2d.h"
#include "waypoint2d.h"

namespace Rapi
{

/**
 * Helps to debug robot controllers by monitoring registered variables
 * @author Jens Wawerla
 */
class CVariableMonitor
{
  public:
    /** Default constructor */
    CVariableMonitor();
    /** Default destructor */
    ~CVariableMonitor();
    /**
     * Adds a varible to the monitor
     * @param ptr pointer to varible to be added
     * @param name of variable
     */
    void addVar ( float* ptr, std::string name );
    void addVar ( double* ptr, std::string name );
    void addVar ( bool* ptr, std::string name );
    void addVar ( int* ptr, std::string name );
    void addVar ( CPose2d* ptr, std::string name );
    void addVar ( CPoint2d* ptr, std::string name );
    void addVar ( CVelocity2d* ptr, std::string name );
    void addVar ( CWaypoint2d* ptr, std::string name );
    void addVar ( IRapiVar* ptr, std::string name );
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

  protected:
    typedef enum {FLOAT, DOUBLE, INT, BOOL, POSE2D, POINT2D, VELOCITY2D,
                  WAYPOINT2D, RAPI_VAR
               } tVar;
    typedef struct {
      void* ptr;
      tVar varType;
      std::string name;
    } tVarEntry;
    /** Vector of monitored variables */
    std::vector<tVarEntry> mVarList;

};

}// namespace

#endif
