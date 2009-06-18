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

#ifndef RAPICSTAGEROBOT_H
#define RAPICSTAGEROBOT_H

#include "robot.h"
#include "stage.hh"

namespace Rapi
{

const std::string RANGER_MODEL_NAME = "ranger";
const std::string LASER_MODEL_NAME = "laser";

/**
 * This robot gets devices for a robot simulated in Stage
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CStageRobot : public ARobot
{

  public:
    /** Default constructor */
    CStageRobot ( Stg::Model* mod );
    /** Default destructor */
    ~CStageRobot();
    /**
     * Ininitializes the robot
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
    virtual int findDevice ( AAnalogSensorArray* &device, std::string devName );

  protected:
    /** Friend function of stage model updates */
    friend int ctrlUpdate ( Stg::World* world, CStageRobot* controller );

    /**
     * Searches the list of previously found devices and returns a match or
     * NULL if no device was filed under the given device name
     * @param devName to search
     * @return device or NULL
     */
    ADevice* findDeviceByName ( std::string devName );
    /** Main Stage model */
    Stg::Model*mStageModel;

};

} // namespace

#endif
