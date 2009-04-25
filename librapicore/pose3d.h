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

#ifndef POSE3D_H
#define POSE3D_H

namespace Rapi
{

/**
 * Defines a pose in 3 dimenstions. Angles are normalized to [-pi, pi]
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CPose3d
{
  public:
    /**
     * Default constructor
     */
    CPose3d ( double x = 0.0, double y = 0.0, double z = 0.0,
              double roll = 0.0, double pitch = 0.0, double yaw = 0.0 );
    /**
     * Constructor
     * @param pose to copy pose from
     */
    CPose3d ( CPose3d const& pose );
    /** Default destructor */
    ~CPose3d();
    /**
     * Prints the class data to std out
     */
    void print();
    /**
     * Gets the euclidian distance to a given pose
     * @param pose to get distance to
     * @return distance [m]
     */
    double distance( const CPose3d pose );
    /** Overloaded = operator */
    void operator= (const CPose3d pose );
    /** Overloaded + operator */
    CPose3d operator+ (const CPose3d pose );
    /** Overloaded != operator */
    bool operator!= (const CPose3d pose);
    /** x position [m] */
    double mX;
    /** y position [m] */
    double mY;
    /** z position [m] */
    double mZ;
    /** Roll angle [rad] */
    double mRoll;
    /** Pitch angle [rad] */
    double mPitch;
    /** Yaw angle [rad] */
    double mYaw;

};

} // namespace
#endif
