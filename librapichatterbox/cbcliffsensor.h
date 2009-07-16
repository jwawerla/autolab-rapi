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
#ifndef RAPICBCLIFFSENSOR_H
#define RAPICBCLIFFSENSOR_H

#include "binarysensorarray.h"
#include "cbdriver.h"

namespace Rapi
{

/**
 * Chatterbox implementation of the wheel drop sensor of a ICreate robot
 * index 0 = left, index 1 = left front, index 2 = right front, index 3 = right
 * @author Jens Waweral
 */
class CCBCliffSensor : public ABinarySensorArray
{

    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBCliffSensor ( CCBDriver* driver, std::string devName );
    /** Default destructor */
    ~CCBCliffSensor();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Initializes the device
     * @param return 1 if success -1 otherwise
     */
    virtual int init();
    /**
     * Gets the raw cliff sensor data
     * <STRONG>
     * This method is Create specific and not supported by other implementations
     * of this device. The use of this method will make your code not portable
     * to other plattforms.
     * </STRONG>
     * @param id of sensor
     * @return raw data [0..4095]
     */
    float getRawSensorData(unsigned int id);

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     */
    virtual void updateData();

  private:
    /** HAL of the chatterbox */
    CCBDriver* mCBDriver;
};

} // namespace

#endif
