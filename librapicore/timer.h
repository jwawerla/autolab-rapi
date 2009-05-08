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
#ifndef RAPITIMER_H
#define RAPITIMER_H

#include "robotupdateinterface.h"
#include "robot.h"

namespace Rapi {

/**
 * A simple timer
 * @author Jens Wawerla
 */
class CTimer : public IRobotUpdate
{
  public:
    /**
     * Default constructor
     * @param robot this timer is attached to
     */
    CTimer(ARobot* robot);
    /** Default destructor */
    virtual ~CTimer();
    /**
     * Starts the timer with a give timeout
     * @param timeout [s]
     */
    void start(float timeout);
    /**
     * Stops the timer
     */
    void stop();
    /**
     * Checks if the timer if running
     * @return true if running, false otherwise
     */
    bool isRunning();
    /**
     * Checks if the timer is expired, this is different from isRunning because a
     * stopped timer is not running but it has not expired.
     * @return true if expired, false otherwise
     */
    bool isExpired();
    /**
     * Gets the remaining time
     * @return [s]
     */
    float getTimeRemaining();

  protected:
    /**
     * Called by the frame work to update the state variable
     * @param dt time since last call [s]
     */
    virtual void updateData(float dt);

  private:
    /** mTime [s] */
    float mTime;
    /** Flags if the timer is running */
    bool mFgRunning;
    /** Flags if the timer has expired or not */
    bool mFgExpired;
};

} // namespace

#endif
