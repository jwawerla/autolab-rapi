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

#ifndef RAPIDRIVETRAIN2DOF_H
#define RAPIDRIVETRAIN2DOF_H

#include "device.h"
#include "velocity2d.h"
#include "odometry.h"

namespace Rapi
{

/**
 * Abstract base for a drivetrain with 2 DOF
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class ADrivetrain2dof : public ADevice
{
  public:
    /** Default destructor */
    virtual ~ADrivetrain2dof();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_DRIVETRAIN2DOF; };
    /**
     * Gets the current velocity command set for the robot
     * @return velocity
     */
    CVelocity2d getVelocityCmd();
    /**
     * Sets the velocity and turn rate of the robot
     * @param velocity forward positive, backward negative [m/s]
     * @param turnRate positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setSpeedCmd( float velocity, float turnRate ) = 0;
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Checks if the robot got stuck somewhere
     * @return true if stuck, false otherwise
     */
    virtual bool isStuck();
    /**
     * Gets the odometry of this drivetrain
     * @return odometry
     */
    COdometry* getOdometry();

  protected:
    /** Default constructor
     * @param devName name of device
     */
    ADrivetrain2dof( std::string devName = NULL );
    /** Odometry */
    COdometry* mOdometry;
    /** Velocity command */
    CVelocity2d mVelocityCmd;
    /** Flags if robot is stuck somewhere */
    bool mFgStuck;
};

} // namespace

#endif
