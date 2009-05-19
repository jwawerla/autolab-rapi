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

#ifndef RAPIPOWERPACK_H
#define RAPIPOWERPACK_H

#include "device.h"

namespace Rapi
{

/** Type defintion for charging state */
typedef enum  {NOT_CHARGING, RECONDITIONING, FULL_CHARGING,
               TRICKLE_CHARGING, WAITING, CHARGING_FAULT
              } tChargeState;

/**
 * Abstract base class for power packs
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class APowerPack : public ADevice
{
  public:
    /** Default destructor */
    ~APowerPack();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() {return RAPI_POWERPACK; };
    /**
     * Gets the current, negative if drawn from battery or positive if charging
     * @return [A]
     */
    virtual double getCurrent() const;
    /**
     * Gets the actual momentary battery capacity
     * @return [Wh]
     */
    virtual double getBatteryCapacity() const;
    /**
     * Gets the maximal battery capacity
     * @return [Wh]
     */
    virtual double getMaxBatteryCapacity() const;
    /**
     * Gets the actual voltage of the system
     * @return [V]
     */
    virtual double getVoltage() const;
    /**
     * Gets the temperature of the battery
     * @return [C]
     */
    virtual double getBatteryTemperature() const;
    /**
     * Gets the total energy dissipated since start of robot
     * @return [Wh]
     */
    virtual double getTotalEnergyDissipated() const;
    /**
     * Gets the level of remaining energy in the battery
     * @return [0,1]
     */
    double getBatteryLevel() const;
    /**
     * Checks if the robot is charging or not
     * @return true if charging, false otherwise
     */
    virtual bool isCharging() = 0;
    /**
     * Gets the source id from which we are charging. By default this method
     * returns 0 since most robot have only one charging option. But some
     * robots, like the IRobot Create have more, e.g. docking station,
     * power plug.
     * @return charge source id
     */
    virtual int getChargingSource() const;
    /**
     * Gets the state of the charging process
     * @return state
     */
    virtual tChargeState getChargingState() = 0;
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Gets the name of a gui this device can be visualized
     * @return name of gui
     */
    virtual std::string getGuiName() { return "PowerPackWidget"; };

  protected:
    /** Default constructor
     * @param devName name of device
     */
    APowerPack ( std::string devName );
    /** Maximal battery capacity [Wh] */
    double mMaxBatteryCapacity;
    /** Actual battery capacity [Wh] */
    double mBatteryCapacity;
    /**
     * Current drawn from the battery (negative) or added to battery
     * (positive) [A]
     */
    double mCurrent;
    /** Maximum current drawn or charge [A] */
    double mMaxCurrent;
    /** Actual voltage [V] */
    double mVoltage;
    /** Battery temperature [C] */
    double mBatteryTemperature;
    /** Total amount of energy dissipated since start of robot [Wh] */
    double mTotalEnergyDissipated;
};

} // namespace

#endif
