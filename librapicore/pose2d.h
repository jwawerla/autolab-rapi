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

#ifndef POSE2D_H
#define POSE2D_H

#include "point2d.h"
#include <string>
namespace Rapi
{

/**
 * Defines a pose in 2 dimenstions. Angle is normalized to [-pi, pi]
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CPose2d
{
  public:
    /**
    * Default constructor
    * @param x
    * @param y
    * @param yaw
    */
    CPose2d ( double x = 0.0, double y = 0.0, double yaw = 0.0 );
    /**
    * Constructor
    * @param pose to copy pose from
    */
    CPose2d ( CPose2d const& pose );
    /**
    * Constructor
    * @param point to copy x and y from, sets yaw to 0
    */
    CPose2d ( CPoint2d const& point );
    /** Default destructor */
    ~CPose2d();
    /**
     * Gets the euclidian distance to a given pose
     * @param pose to get distance to
     * @return distance [m]
     */
    double distance ( const CPose2d pose ) const;
    /**
     * Gets the angular difference between this pose and the given pose
     * @return [rad]
     */
    double angleDifference ( const CPose2d pose ) const;
    /**
     * Calculates the bearing from this pose to a given pose
     * @param pose to get bearing to
     * @return [rad]
     */
    double bearingTo(const CPose2d pose) const;
    /**
     * Prints the pose data to std out
     */
    void print() const;
    /**
     * Converts the pose data to a string
     * @return string
     */
    std::string toStr() const;
    /** Overloaded = operator */
    void operator= ( const CPose2d pose );
    /** Overloaded = operator */
    void operator= ( const CPoint2d point );
    /** Overloaded + operator */
    CPose2d operator+ ( const CPose2d pose );
    /** Overloaded - operator */
    CPose2d operator- ( const CPose2d pose );
    /** Overloaded != operator */
    bool operator!= ( const CPose2d pose );
    /** x position [m] */
    double mX;
    /** y position [m] */
    double mY;
    /** yaw angle [rad] */
    double mYaw;
};

} // namespace
#endif
