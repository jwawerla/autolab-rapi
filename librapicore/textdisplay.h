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

#ifndef RAPITEXTDISPLAY_H
#define RAPITEXTDISPLAY_H

#include "device.h"

namespace Rapi
{

/**
 * Abstract base class for text displays
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class ATextDisplay : public ADevice
{
  public:
    /** Default destructor */
    ~ATextDisplay();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_TEXTDISPLAY; };
    /**
     * Sets the text to be displayed
     * @param text to be displayed
     * @return 1 if successfull, 0 otherwise
     */
    virtual int setText(std::string text) = 0;
    /**
     * Gets the text currently displayed
     * @return text
     */
    std::string getText() const;
    /**
     * Gets the number of characters the display can show
     * @return number characters
     */
    virtual int getSize() const = 0;
    /**
     * Prints the devices main information
     */
    virtual void print() const;
        /**
     * Gets the name of a gui this device can be visualized
     * @return name of gui
     */
    virtual std::string getGuiName() { return "TextDisplayWidget"; };

  protected:
    /** Default constructor
     * @param devName name of device
     */
    ATextDisplay ( std::string devName );
    /** Text currently displayed */
    std::string mText;

};

} // namespace

#endif
