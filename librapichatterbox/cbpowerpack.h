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

#ifndef RAPICBPOWERMANAGEMENT_H
#define RAPICBPOWERMANAGEMENT_H

#include "powerpack.h"
#include "cbdriver.h"

namespace Rapi
{

// Forward declaration for friendship
class CCBRobot;

/** Typedefinition for charger sources the Create can use */
typedef enum {NO_CHARGER = 0, INTEGRAL_CHARGER, HOMEBASE } tChargeSource;

/**
 * This class provides an interface to the power pack of a chatterbox
 * robot.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CCBPowerPack : public APowerPack
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of device
     */
    CCBPowerPack ( CCBDriver* cbDriver, std::string devName );
    /** Default destructor */
    virtual ~CCBPowerPack();
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
     * Gets the source type from which we are charging
     * @return charge source type
     */
    virtual int getChargingSource();
    /**
     * Gets the state of the charging process
     * @return state
     */
    virtual tChargeState getChargingState();
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Checks if the robot is charging or not
     * @return true if charging, false otherwise
     */
    virtual bool isCharging();

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
