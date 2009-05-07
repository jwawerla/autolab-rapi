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
#ifndef ALIGHTS_H
#define ALIGHTS_H

#include "device.h"
#include "rgbcolor.h"

namespace Rapi
{

#define ALL_LIGHTS -1

/**
 * Abstract base class for all lights stuff
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 01/2008
 */
class ALights : public ADevice
{
  public:
    /** Default destructor */
    virtual ~ALights();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_LIGHTS; };
    /**
     * Sets a given rgb value for a light, note for none rgb lights
     * just set the red channel and leave the remaining open
     * @param id of light, -1 all
     * @param red [0..255]
     * @param green [0..255]
     * @param blue [0..255]
     * @return 1 if successfull, 0 otherwise
     */
    virtual int setLight(int id, unsigned char red,
                         unsigned char green,
                         unsigned char blue);
    /**
     * Sets a given rgb value for a light, note for none rgb lights
     * just set the red channel and leave the remaining open
     * @param id of light, -1 all
     * @param color to be set
     * @return 1 if successfull, 0 otherwise
     */
    virtual int setLight(int id, CRgbColor color)  = 0;;
    /**
     * Sets the blinking frequency of a led
     * @param id of led to be set, -1 all leds
     * @param enabled true enables blinking, false disables blinking
     * @param freq to be set [Hz]
     */
    virtual int setBlink(int id, bool enabled, float freq) = 0;
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Gets the number of available lights
     * @return number of lights
     */
    unsigned int getNumLights() { return mNumLights; };

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ALights(std::string devName);
    /** Number of lights */
    unsigned int mNumLights;
};

} // namespace
#endif
