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
#ifndef RAPIRAPITYPESYSTEM_H
#define RAPIRAPITYPESYSTEM_H

#include <string>
#include <vector>

#include "rapivarinterface.h"
#include "pose2d.h"
#include "point2d.h"
#include "velocity2d.h"
#include "waypoint2d.h"

namespace Rapi {

/**
 * This class provides a type system for rapi. Classes like CVariableMonitor
 * or CDataLogger can use this class to implement an interface that allows
 * variables to be registered.
 * @author Jens Wawerla
 */
class CRapiTypeSystem {
  public:
    /** Default constructor */
    CRapiTypeSystem();
    /** Default destructor */
    virtual ~CRapiTypeSystem();
    /**
     * Adds a varible to the system
     * @param ptr pointer to varible to be added
     * @param name of variable
     * @param decimals number of decimal places
     */
    void addVar ( float* ptr, std::string name, unsigned int decimals = 1);
    void addVar ( double* ptr, std::string name, unsigned int decimals = 1);
    void addVar ( bool* ptr, std::string name );
    void addVar ( int* ptr, std::string name );
    void addVar ( unsigned int* ptr, std::string name );
    void addVar ( CPoint2d* ptr, std::string name );
    void addVar ( CWaypoint2d* ptr, std::string name );
    void addVar ( IRapiVar* ptr, std::string name );
    void addVar ( std::string* ptr, std::string name );

  protected:
    typedef enum {FLOAT, DOUBLE, INT, UINT, BOOL, POINT2D,
                  WAYPOINT2D, RAPI_VAR, STRING
               } tVar;
    typedef struct {
      void* ptr;
      tVar varType;
      std::string name;
      unsigned int decimalPlaces;
    } tVarEntry;
    /** Vector of monitored variables */
    std::vector<tVarEntry> mVarList;
    /**
     * Turns a double value to a string with a given number of decimal places
     * @param value to convert
     * @param decimals number of decimal places
     * @return string
     */
    std::string toDoubleToStr(double value, unsigned int decimals);
};

} // namespace

#endif
