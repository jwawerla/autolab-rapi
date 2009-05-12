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
     * @param turnrate positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setVelocityCmd( const float velocity, const float turnrate ) = 0;
    /**
     * Sets the velocity and turn rate of the robot
     * @param velocity forward positive, backward negative [m/s]
     *                 positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setVelocityCmd( CVelocity2d velocity ) = 0;
    /**
     * Sets a rotational speed for the robot
     * @param turnrate positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setRotationalSpeedCmd( float turnrate);
    /**
     * Sets a translational speed for the robot
     * @param velocity forward positive, backward negative [m/s]
     */
    virtual void setTranslationalSpeedCmd( float velocity);
    /**
     * Stops the robot by setting all velocities to zero
     */
    virtual void stop();
    /**
     * Checks if robot is stopped or not
     * @return true if stopped, false otherwise
     */
    virtual bool isStopped();
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Checks if the robot got stuck somewhere and is thus stalled
     * @return true if stalled, false otherwise
     */
    virtual bool isStalled();
    /**
     * Gets the odometry of this drivetrain
     * @return odometry
     */
    COdometry* getOdometry();
    /**
     * Gets the upper velocity limits
     * @return limits
     */
    virtual CVelocity2d getUppererVelocityLimit() { return mUpperVelocityLimit; };
    /**
     * Gets the lower velocity limits
     * @return limits
     */
    virtual CVelocity2d getLowerVelocityLimit() { return mLowerVelocityLimit; };
    /**
     * Sets the upper velocity limits
     * @param limits
     */
    virtual void setUppererVelocityLimit(CVelocity2d limit);
    /**
     * Sets the lower velocity limits
     * @param limits
     */
    virtual void setLowerVelocityLimit(CVelocity2d limit);
    /**
     * Gets the name of a gui this device can be visualized
     * @return name of gui
     */
    virtual std::string getGuiName() { return "DrivetrainWidget";};
  protected:
    /** Default constructor
     * @param devName name of device
     */
    ADrivetrain2dof( std::string devName = NULL );
    /**
     * Limits mVelocityCmd to be within mLowerVelocityLimit and mUpperVelocityLimit
     */
    void applyVelocityLimits();
    /** Odometry */
    COdometry* mOdometry;
    /** Velocity command */
    CVelocity2d mVelocityCmd;
    /** Velocity measurement */
    CVelocity2d mVelocityMeas;
    /** Upper velocity limit */
    CVelocity2d mUpperVelocityLimit;
    /** Lower velocity limit */
    CVelocity2d mLowerVelocityLimit;
    /** Flags if robot is stalled somewhere */
    bool mFgStalled;
};

} // namespace

#endif
