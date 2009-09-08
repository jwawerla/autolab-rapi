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
#ifndef RAPICBOVERCURRENTSENSOR_H
#define RAPICBOVERCURRENTSENSOR_H

#include "binarysensorarray.h"
#include "cbdriver.h"

namespace Rapi
{

/**
 * Chatterbox implementation of the wheel drop sensor of a ICreate robot
 * index 0 = left wheel overcurrent
 * index 1 = right wheel over current,
 * index 2 = low side driver 0 over current
 * index 3 = low side driver 1 over current
 * index 4 = low side driver 2 over current
 * @author Jens Waweral
 */
class CCBOverCurrentSensor : public ABinarySensorArray
{

    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBOverCurrentSensor ( CCBDriver* driver, std::string devName );
    /** Default destructor */
    ~CCBOverCurrentSensor();
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

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt);

  private:
    /** HAL of the chatterbox */
    CCBDriver* mCBDriver;
};

} // namespace

#endif
