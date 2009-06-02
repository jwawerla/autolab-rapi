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

#ifndef RAPICBLASER_H
#define RAPICBLASER_H

#include "rangefinder.h"
#include "cbdriver.h"
#include <stdio.h>

namespace Rapi
{

// Forward declaration for friendship
class CCBRobot;

/**
 * Hardware abstraction class for a urg laser. This class is based on the
 * URG driver from the player/stage project.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 01/2008
*/
class CCBLaser : public ARangeFinder
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBLaser(CCBDriver* driver, std::string devName );
    /** Default destructor */
    virtual ~CCBLaser();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable 
     */
    virtual void setEnabled(bool enable);
    /**
     * Initializes the device
     * @param return 1 if success -1 otherwise
     */
    virtual int init();
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Opens a serial port to the laser
     * @param portName name of port/device
     * @param baud baud rate to use (19200, 56700, 112500)
     * @return 1 if successful, 0 otherwise
     */
    int openPort (const char * portName, int baud);
    /**
     * Closes the port to the laser
     */
    int closePort();

  protected:
    /**
     * Reads characters (and throws them away) until the nth occurence of char c
     * @param n how often do we need to see c
     * @param c character we are looking for
     * @return 1 if successful, -1 otherwise
     */
    int readUntilNthOccurence (int n, char c);
    /**
     * Gets the protocol the Hokuyo supports. Old firmware revisions support
     * protocol SCIP1.0, and a max range of 4 meters Since Firmware Revision 3.0.00,
     * it's called SCIP2.0, and the max range is 5.6 meters. Result gets stored in
     * mScpiVersion
     * @return 1 if successful, -1 otherwise
     */
    int getSCIPVersion ();
    /**
     * Reads the configuration from the laser and fills the data structur of this class
     * @return 1 if successful, -1 otherwise
     */
    int getSensorConfig ();
    /**
     * Read until, len bytes are read or a timeout occurred
     * @param buffer to store the read bytes
     * @param len of buffer, and number of expected bytes
     * @param timeout time out [ms]
     * @return number of bytes read, or -1 in case of an error
     */
    int readUntil (unsigned char *buf, int len, int timeout);
    /**
     * Change the baud rate to a different rate
     * @param currBaud current baud rate
     * @param newBaud new baud rate to set
     * @param timeout well.. [ms]
     * @return 1 if successful, -1 otherwise
     */
    int changeBaud (int currBaud, int newBaud, int timeout);
    /**
     * Gets the device info
     */
    int getIDInfo ();
    /**
     * Reads data from the laser
     * @return 1 if successful, -1 otherwise
     */
    int readData();
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     */
    virtual void updateData();

  private:
    /** HAL of the chatterbox */
    CCBDriver* mCBDriver;
    /** Firmware version of laser */
    int mScpiVersion;
    /** File destcriptor of serial port */
    int mFd;
    /** File pointer */
    FILE* mLaserPort;
    /** Device for the laser */
    char mLaserDevice[20];
    /** Default baud rate to use */
    int mBaudRate;
};

} // namespace

#endif
