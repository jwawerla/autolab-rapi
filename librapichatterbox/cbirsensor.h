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
#ifndef RAPICBIRSENSOR_H
#define RAPICBIRSENSOR_H

#include "rangefinder.h"
#include "cbdriver.h"

namespace Rapi
{

/**
 * This class provides excess to the IR distance sensors of chatterbox
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 01/2008
 */
class CCBIrSensor : public ARangeFinder
{
  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBIrSensor(CCBDriver* cbDriver, std::string devName);
    /** Default destructor */
    virtual ~CCBIrSensor();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable 
     */
    virtual void setEnabled(bool enable);
    /**
     * Initializes the device
     * @param return 1 if success 0 otherwise
     */
    virtual int init();
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt );
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Set the time constant for the low pass filter on robostix
     * to filter the the sensor readings.
     * @param tau [s]
     */
    void setLpfTau( float tau );

  private:
    /** HAL of the chatterbox */
    CCBDriver* mCBDriver;
};

} // namespace

#endif
