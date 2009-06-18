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
#ifndef RAPIANALOGSENSORARRAY_H
#define RAPIANALOGSENSORARRAY_H

#include "device.h"

namespace Rapi
{

/**
 * Abstract base class for sensor arrays with analog values
 * @author Jens Wawerla
 */
class AAnalogSensorArray : public ADevice
{
  public:
    /** Default destructor */
    virtual ~AAnalogSensorArray();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_ANALOGSENSORARRAY; };
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Gets the number available samples
     * @return number of samples
     */
    virtual unsigned int getNumSamples() const;
    /** Data */
    double* mData;

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    AAnalogSensorArray( std::string devName );
    /** Number of samples */
    unsigned int mNumSamples;
};

} // namespace

#endif
