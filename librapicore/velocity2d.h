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

namespace Rapi
{

/**
 * Defintion of a 2d velocity
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CVelocity2d
{
  public:
    /** Default constructor
     * @param vX translational speed [m/s]
     * @param vY translational speed [m/s]
     * @param yawData rotational speed [rad/s]
     */
    CVelocity2d( double vX = 0.0, double vY = 0.0, double yawDot = 0.0);
    /** Default destructor */
    ~CVelocity2d();
    /**
     * Prints the velocity data to std out
     */
    void print();
    /** Assignment operator */
    void operator = ( CVelocity2d const &vel );
    /**
     * Set a zero velocity
     */
    void setZero();
    /** Translational speed in x direction [m/s] */
    double mVX;
    /** Translational speed in y direction [m/s] */
    double mVY;
    /** Rotational speed [rad/s] */
    double mYawDot;
};

} // namespace

#endif
