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
#ifndef RAPIPOINT2D_H
#define RAPIPOINT2D_H

#include <string>
namespace Rapi
{

/**
 * Defines a two dimensional point
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CPoint2d
{
  public:
    /** Default constructor */
    CPoint2d ( double x = 0.0, double y = 0.0 );
    /** Default destructor */
    ~CPoint2d();
    /**
     * Prints the points data to std out
     */
    void print() const;
    /**
     * Converts the point data to a string
     * @return string
     */
    std::string toStr() const;
    /**
     * Sets the point from polar coordinates
     * @param length
     * @param angle [rad]
     */
    void fromPolar(double length, double angle);
    /**
     * Gets the distance to a given point
     * @return distance
     */
    double distance(const CPoint2d point) const;
    /** X coordinate */
    double mX;
    /** Y coordinate */
    double mY;
    /** Overloaded = operator */
    void operator= ( const CPoint2d point );
};

}  // name space

#endif
