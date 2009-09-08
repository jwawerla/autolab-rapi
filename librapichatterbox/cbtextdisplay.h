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
#ifndef RAPICBTEXTDISPLAY_H
#define RAPICBTEXTDISPLAY_H

#include "textdisplay.h"
#include "cbdriver.h"

namespace Rapi {

/**
 * Chatterbox implementation of a text display, this on is for the 7 segment display
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CCBTextDisplay : public ATextDisplay
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
    * Default constructor
    * @param driver chatterbox driver
    * @param devName name of device
    */
    CCBTextDisplay(CCBDriver* driver, std::string devName);
    /** Default destructor */
    ~CCBTextDisplay();
    /**
      * Initializes the device
      * @param return 1 if success 0 otherwise
      */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Sets the text to be displayed
     * @param text to be displayed
     * @return 1 if successfull, 0 otherwise
     */
    virtual int setText(std::string text);
   /**
     * Gets the number of characters the display can show
     * @return number characters
     */
    virtual int getSize() const;

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt);

  private:
    /** Hardware driver of chatterbox */
    CCBDriver* mCBDriver;
};

}

#endif
