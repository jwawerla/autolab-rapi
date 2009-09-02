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
#ifndef RAPICBPHOTOSENSOR_H
#define RAPICBPHOTOSENSOR_H

#include "analogsensorarray.h"
#include "cbdriver.h"

namespace Rapi {

/**
 * This class provides access to the photo sensor of chatterbox
 * @author Jens Wawerla
 */
class CCBPhotoSensor : public AAnalogSensorArray
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of device
     */
    CCBPhotoSensor(CCBDriver* cbDriver, std::string devName);
    /** Default destructor */
    ~CCBPhotoSensor();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Initializes the device
     * @param return 1 if success 0 otherwise
     */
    virtual int init();
    /**
     * Set the time constant for the low pass filter on robostix
     * to filter the photo sensor readings.
     * @param tau [s]
     */
     void setLpfTau( float tau );

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     */
    virtual void updateData();
    /** HAL of the chatterbox */
    CCBDriver* mCBDriver;
};

} // namespace

#endif
