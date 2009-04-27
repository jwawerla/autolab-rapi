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
#ifndef RAPISTAGEPOWERPACK_H
#define RAPISTAGEPOWERPACK_H

#include "powerpack.h"
#include "stage.hh"

namespace Rapi
{

/**
 * Stage implementation of a power pack
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CStagePowerPack : public APowerPack
{
  public:
    /**
     * Default constructor
     * @param stgModel stage model of sonar
     * @param devName name of device
     */
    CStagePowerPack ( Stg::ModelPosition* stgModel, std::string devName );
    /** Default destructor */
    ~CStagePowerPack();
    /**
     * Initializes the robot
     * @param return 1 if success -1 otherwise
     */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Gets the state of the charging process
     * @return state
     */
    virtual tChargeState getChargingState();

  protected:
    /**
     * This method gets called by the framework every step to refresh
     * the sensor data
     */
    virtual void updateData();
    /** Friend function of stage model updates */
    friend int powerUpdate ( Stg::ModelPosition* mod, CStagePowerPack* power );
    /** Stage model */
    Stg::PowerPack* mStgPowerPack;
    /** Update interval [s] */
    double mSimInterval;
};

} // namespace

#endif
