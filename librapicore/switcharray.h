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
#ifndef RAPISWITCHARRAY_H
#define RAPISWITCHARRAY_H

#include "device.h"

namespace Rapi {

/**
 * Abstract base class for an array of switches
 * @author Jens Wawerla
 */
class ASwitchArray : public ADevice
{
  public:
    /** Default destructor */
    ~ASwitchArray();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_SWITCHARRAY; };
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Sets the switch with id to a given state
     * @param on
     * @param id of switch
     */
    virtual void setSwitch(unsigned int id, bool on);
    /**
     * Gets the state of the switch with id
     * @param id of switch
     * @return state
     */
    virtual bool getSwitch(unsigned int id) const;
    /**
     * Gets the number of switches available
     * @return number of switches
     */
    virtual unsigned int getNumSwitches() const;

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ASwitchArray(std::string devName = "ASwitchArray " );
    /** Number of switches */
    unsigned int mNumSwitches;
    /** Status of the switch */
    bool* mSwitch;
};

} // namespace

#endif
