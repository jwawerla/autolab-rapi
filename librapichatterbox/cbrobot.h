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

#ifndef RAPICHATTERBOXROBOT_H
#define RAPICHATTERBOXROBOT_H

#include "robot.h"
#include "cbdriver.h"
#include "cbdrivetrain2dof.h"
#include "cbpowerpack.h"
#include "cblaser.h"
#include "cbirsensor.h"
#include "cblights.h"
#include "cbbumper.h"
#include "cbtextdisplay.h"
#include "cbwallsensor.h"
#include "cbcliffsensor.h"
#include "cbwheeldropsensor.h"
#include "cbovercurrentsensor.h"
#include "cblowsidedriver.h"
#include "fiducialfinder.h"


namespace Rapi
{

/**
 * A Rapi based implementation for a chatterbox robot
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CCBRobot : public ARobot
{
  public:
    /**
     * Default constructor
     */
    CCBRobot();
    /** Default destructor */
    virtual ~CCBRobot();
    /**
     * Ininitializes the robot, this includes powering up ICreate, resetting
     * robostix and opening the serial port. The robot is ready to use after
     * this call.
     * @return 1 if successfull, 0 otherwise
     */
    virtual int init();
    /**
     * Gets the current time of the robot, this maybe simulated time, real time,
     * elapsed time since start of robot etc.
     * @return [s]
     */
    virtual double getCurrentTime() const;
    /**
     * This is the main of the thread
     */
    void run();
    /**
     * Terminates the execution of the main thread
     */
    void terminate();
    /**
     * Checks if the main thread is running or not
     * @return true if running, false otherwise
     */
    bool isRunning() const { return mFgRunning; };
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( ARangeFinder* &device, std::string devName );
    virtual int findDevice ( ADrivetrain2dof* &device, std::string devName );
    virtual int findDevice ( APowerPack* &device, std::string devName );
    virtual int findDevice ( AFiducialFinder* &device, std::string devName );
    virtual int findDevice ( ALights* &device, std::string devName );
    virtual int findDevice ( ATextDisplay* &device, std::string devName );
    virtual int findDevice ( ABinarySensorArray* &device, std::string devName );
    virtual int findDevice ( ASwitchArray* &device, std::string devName );

  protected:
    /**
     * This method sleeps for interval seconds since the last call
     * @param interval [s]
     */
    void synchronize ( double interval );
    /** Flag if main loop is running or not */
    bool mFgRunning;

  private:
    /** Chatterbox hardware driver */
    CCBDriver* mCBDriver;
    /** Drivetrain */
    CCBDrivetrain2dof* mCBDrivetrain;
    /** Power pack */
    CCBPowerPack* mCBPowerPack;
    /** Laser */
    CCBLaser* mCBLaser;
    /** Infrared senors */
    CCBIrSensor* mCBIrSensor;
    /** Lights */
    CCBLights* mCBLights;
    /** Text display */
    CCBTextDisplay* mCBTextDisplay;
    /** Bumper */
    CCBBumper* mCBBumper;
    /** Wall sensor */
    CCBWallSensor* mCBWallSensor;
    /** Wheel drop sensor */
    CCBWheelDropSensor* mCBWheelDropSensor;
    /** Cliff sensor */
    CCBCliffSensor* mCBCliffSensor;
    /** Over current sensors */
    CCBOverCurrentSensor* mCBOverCurrentSensor;
    /** Low side driver */
    CCBLowSideDriver* mCBLowSideDriver;
    /** Last time synchronize was called */
    double mLastSynchronizeTime;
};

} // namespace

#endif
