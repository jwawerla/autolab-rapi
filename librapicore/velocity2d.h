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
#ifndef RAPIVELOCITY2D_H
#define RAPIVELOCITY2D_H

#include "rapivarinterface.h"

namespace Rapi
{

/**
 * Defintion of a 2d velocity
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1
 */
class CVelocity2d : public IRapiVar
{
  public:
    /** Default constructor
     * @param vX translational speed [m/s]
     * @param vY translational speed [m/s]
     * @param yawData rotational speed [rad/s]
     */
    CVelocity2d( double vX = 0.0, double vY = 0.0, double yawDot = 0.0,
                 std::string name = "velocity" );
    /** Default destructor */
    ~CVelocity2d();
    /**
     * Prints the velocity data to std out
     */
    void print() const;
    /**
     * Converts the velocity data to a string
     * @return string
     */
    std::string toStr() const;
    /**
     * Gets the variable type string
     * @return variable type name
     */
    std::string getTypeStr() const { return "CVelocity2d"; };
    /**
     * Gets the data of the variable as a comma separated value string
     * @return comma separated value string
     */
    std::string toCSV() const;
    /**
     * Gets a header for the variable 
     * @return header
     */
    std::string getCVSHeader() const;
    /** Assignment operator */
    void operator = ( CVelocity2d const &vel );
    /**
     * Set a zero velocity
     */
    void setZero();
    /** Translational speed in x direction [m/s] */
    double mXDot;
    /** Translational speed in y direction [m/s] */
    double mYDot;
    /** Rotational speed [rad/s] */
    double mYawDot;
};

} // namespace

#endif
