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

#ifndef RAPICBDRIVETRAIN2DOF_H
#define RAPICBDRIVETRAIN2DOF_H

#include "drivetrain2dof.h"
#include "cbdriver.h"
#include "cbodometry.h"

namespace Rapi
{

// Forward declaration for friendship
class CCBRobot;

/**
 * Chatterbox implementation of a 2dof drivetrain
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CCBDrivetrain2dof : public ADrivetrain2dof
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
    * Default constructor
    * @param driver chatterbox driver
    * @param devName name of device
    */
    CCBDrivetrain2dof ( CCBDriver* driver, std::string devName );
    /** Default destructor */
    virtual ~CCBDrivetrain2dof();
    /**
      * Initializes the device
      * @param return 1 if success 0 otherwise
      */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Sets the velocity and turn rate of the robot
     * @param velocity forward positive, backward negative [m/s]
     *                 positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setVelocityCmd ( CVelocity2d velocity );
    /**
     * Sets the velocity and turn rate of the robot
     * @param velocity forward positive, backward negative [m/s]
     * @param turnrate positve counterclockwise, negative clockwise [rad/s]
     */
    virtual void setVelocityCmd( const float velocity, const float turnrate );
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Sets the default OI mode, this mode gets selected when ever a speed command
     * is set.
     * <STRONG>
     * This method is Create specific and not supported by other implementations
     * of this device. The use of this method will make your code not portable
     * to other plattforms.
     * </STRONG>
     * @param mode to be used as the default
     */
    void setDefaultOIMode(tOIMode mode);
    /**
     * Actives one of the preprogrammed demos of the Create
     * <STRONG>
     * This method is Create specific and not supported by other implementations
     * of this device. The use of this method will make your code not portable
     * to other plattforms.
     * </STRONG>
     * @param demo to activate
     * @return 1 if successfull, 0 otherwise
     */
    int activateDemo(tDemo demo);
    /**
     * Gets the open interface mode of the Create
     * <STRONG>
     * This method is Create specific and not supported by other implementations
     * of this device. The use of this method will make your code not portable
     * to other plattforms.
     * </STRONG>
     * @return mode
     */
    tOIMode getOIMode();

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     */
    virtual void updateData();

  private:
    /** Hardware driver of chatterbox */
    CCBDriver* mCBDriver;
    /** Default OI mode, to be used when ever a speed command is set */
    tOIMode mOIMode;
    /** Maximum by with the velocity can change in one cycle [m/s] */
    float mMaxVelocityDelta;
    /** Maximum by with the turn rate can change in one cycle [rad/s] */
    float mMaxTurnRateDelta;
    /** Velocity command from previous time step */
    CVelocity2d mPrevVelocityCmd;
	/** Odometry */
	CCBOdometry *mOdometry;
};

} // namespace

#endif
