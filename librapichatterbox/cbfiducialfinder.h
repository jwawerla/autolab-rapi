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
#ifndef RAPICBFIDUCIALFINDER_H
#define RAPICBFIDUCIALFINDER_H

#include "fiducialfinder.h"
#include "cbdriver.h"

namespace Rapi
{

/**
 * Wall sensor of chatterbox
 * @author Jens Wawerla
*/
class CCBFiducialFinder : public AFiducialFinder
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBFiducialFinder ( CCBDriver* driver, std::string devName );
    /** Default destructor */
    ~CCBFiducialFinder();
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
	 * Sets this robot's fiducial ID.
	 * @param id the robot's new id in the range 0...255.
	 */
	virtual void setFiducialSignal(int id);

	/**
	 * Prints info and state about this device.
	 */
	virtual void print(void) const;

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
