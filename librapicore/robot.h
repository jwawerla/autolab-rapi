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

#ifndef RAPIROBOT_H
#define RAPIROBOT_H

#include "robotupdateinterface.h"
#include "rangefinder.h"
#include "drivetrain2dof.h"
#include "powerpack.h"
#include "fiducialfinder.h"
#include "lights.h"
#include "textdisplay.h"
#include "binarysensorarray.h"
#include "switcharray.h"
#include "analogsensorarray.h"
#include "blobfinder.h"
#include "variablemonitor.h"
#include <list>
#include <vector>
#include <string>

namespace Rapi
{

/**
 * Base class for all robot. Robot are used as a central point to obtain all
 * devices the robot is configured with.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class ARobot
{
  public:
    /** Default destructor */
    virtual ~ARobot();
    /**
     * Ininitializes the robot
     * @return 1 if successfull, 0 otherwise
     */
    virtual int init() = 0;
    /**
     * Registers a robot controller with the frame work. The frame work
     * calls the update method of a registered controller in every time
     * step.
     * @param ctrl to be registered
     */
    void registerRobotController ( IRobotUpdate* ctrl );
    /**
     * Registers a state variable with the robot
     * @param var to register
     */
    void registerStateVariable( IRobotUpdate* var);
    /**
     * Gets the name of the robot
     * @return robot name
     */
    std::string getName() const;
    /**
     * Gets the number of available devices
     * @return number of devices
     */
    unsigned int getNumOfDevices() const { return mDeviceList.size(); };
    /**
     * Gets a device by the index of the device list
     * @param index of device
     * @return device or NULL
     */
    ADevice* getDeviceByIndex(unsigned int index) const;
    /**
     * Gets the current time of the robot, this maybe simulated time, real time,
     * elapsed time since start of robot etc.
     * @return [s]
     */
    virtual double getCurrentTime() const = 0;
    /**
     * Checks if the robot is initialized
     * @return true if initialized false otherwise
     */
    bool isInitialized() const { return mFgInitialized; };
    /**
     * Terminates the execution of the main thread
     */
    virtual void quit() = 0;
    /**
     * Gets the robot controller
     * @return controller
     */
    IRobotUpdate* getRobotController() const { return mRobotCtrl; };
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( ARangeFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ADrivetrain2dof* &device,  std::string devName ) = 0;
    virtual int findDevice ( APowerPack* &device, std::string devName ) = 0;
    virtual int findDevice ( AFiducialFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ALights* &device, std::string devName ) = 0;
    virtual int findDevice ( ATextDisplay* &device, std::string devName ) = 0;
    virtual int findDevice ( ABinarySensorArray* &device, std::string devName ) = 0;
    virtual int findDevice ( ASwitchArray* &device, std::string devName ) = 0;
    virtual int findDevice ( AAnalogSensorArray* &device, std::string devName ) = 0;
    virtual int findDevice ( ABlobFinder* &device, std::string devName ) = 0;
    /** Monitor for variables */
    CVariableMonitor mVariableMonitor;
    /**
     * Set the update period
     * @param update period [s]
     */
    void setUpdateInterval ( double interval );
    /**
     * Get the current update period
     * @return update period [s]
     */
    double getUpdateInterval() const;
    /**
     * Checks if the main control loop has run consistently slowly.
     * Calling this function resets the status of the slow loop.
     * @return True if consistently slow
     */
    bool isSlow();
    /**
     * Gets the duration of the last update loop
     * @return duration [s]
     */
    double getDurationLastLoop() const;


  protected:
    /** Default constructor */
    ARobot();
    /**
     * Calls the update method in all registered robot controllers
     */
    void updateControllers();
    /**
     * Calls the update method in all registered state variables controllers
     */
    void updateStateVariable();
    /** Robot controller */
    IRobotUpdate* mRobotCtrl;
    /** State variable list */
    std::list<IRobotUpdate*> mStateVariableList;
    /** Update interval [s] */
    double mUpdateInterval;
    /** Name of robot */
    std::string mName;
    /** List of all devices generated */
    std::vector<ADevice*> mDeviceList;
    /** Flags if the robot is initialized or not */
    bool mFgInitialized;
    /** Flag if the robot control loop is running slowly */
    bool mFgRunningSlowly;
    /** Counts how often control loop has failed to complete on time */
    unsigned int mSlowRunCount;
    /** Threshold before slow runs are reported */
    unsigned int mSlowRunThreshold;
    /** Duration of last update loop [s] */
    double mLastLoopDuration;
};

} // namespace
#endif
