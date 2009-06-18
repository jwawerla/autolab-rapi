/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca), Adam Lein (alein@sfu.ca)       *
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
#ifndef RAPICBFRONTFIDUCIALFINDER_H
#define RAPICBFRONTFIDUCIALFINDER_H

#include "fiducialfinder.h"
#include "cbdriver.h"

/* Remote control bytes */
#define CB_FIDUCIAL_LEFT                      129
#define CB_FIDUCIAL_FORWARD                   130
#define CB_FIDUCIAL_RIGHT                     131
#define CB_FIDUCIAL_SPOT                      132
#define CB_FIDUCIAL_MAX                       133
#define CB_FIDUCIAL_SMALL                     134
#define CB_FIDUCIAL_MEDIUM                    135
#define CB_FIDUCIAL_LARGE_CLEAN               136
#define CB_FIDUCIAL_PAUSE                     137
#define CB_FIDUCIAL_POWER                     138
#define CB_FIDUCIAL_ARC_FORWARD_LEFT          139
#define CB_FIDUCIAL_ARC_FORWARD_RIGHT         140
#define CB_FIDUCIAL_DRIVE_STOP                141

/* Scheduling remote bytes */
#define CB_FIDUCIAL_SEND_ALL                  142
#define CB_FIDUCIAL_SEEK_DOCK                 143

/* Home base bytes */
#define CB_FIDUCIAL_RESERVED                  240
#define CB_FIDUCIAL_RED_BUOY                  248
#define CB_FIDUCIAL_GREEN_BUOY                244
#define CB_FIDUCIAL_FORCE_FIELD               242
#define CB_FIDUCIAL_RED_AND_GREEN_BUOYS       252
#define CB_FIDUCIAL_RED_AND_FORCE_FIELD       250
#define CB_FIDUCIAL_GREEN_AND_FORCE_FIELD     246
#define CB_FIDUCIAL_RED_GREEN_AND_FORCE_FIELD 254

#define CB_FIDUCIAL_NO_IR_DATA                255

namespace Rapi
{

/**
 * Front side fiducial finder for chatterbox
 * @author Jens Wawerla
*/
class CCBFrontFiducialFinder : public AFiducialFinder
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBFrontFiducialFinder( CCBDriver* driver, std::string devName );
    /** Default destructor */
    ~CCBFrontFiducialFinder();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled( bool enable );
    /**
     * Initializes the device
     * @param return 1 if success -1 otherwise
     */
    virtual int init();
    /**
     * Sets this robot's fiducial ID.
     * @param data the robot's new id in the range 0...255.
     */
    virtual void setFiducialSignal( int data );
    /**
     * Prints info and state about this device.
     */
    virtual void print( void ) const;

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
