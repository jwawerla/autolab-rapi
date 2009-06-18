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
#include <string>

namespace Rapi {

/**
 * A simple timer
 * @author Jens Wawerla
 */
class CTimer : public IRobotUpdate, public IRapiVar
{
  public:
    /**
     * Default constructor
     * @param robot this timer is attached to
     * @param name of variable
     */
    CTimer(ARobot* robot, std::string name = "timer");
    /** Default destructor */
    virtual ~CTimer();
    /**
     * Starts the timer with a give timeout
     * @param timeout [s]
     */
    void start(float timeout = 0.0);
    /**
     * Stops the timer
     */
    void stop();
    /**
     * Checks if the timer if running
     * @return true if running, false otherwise
     */
    bool isRunning() const;
    /**
     * Checks if the timer is expired, this is different from isRunning because a
     * stopped timer is not running but it has not expired.
     * @return true if expired, false otherwise
     */
    bool isExpired() const ;
    /**
     * Gets the remaining time
     * @return [s]
     */
    float getRemainingTime() const;
    /**
     * Gets the time that has elapsed since the last start
     * @return [s]
     */
    float getElapsedTime() const;
    /**
     * Returns the timer data as a string
     * @return string
     */
    std::string toStr() const;
    /**
     * Gets the variable type string
     * @return variable type name
     */
    std::string getTypeStr() const { return "CTimer"; };
    /**
     * Gets the data of the variable as a comma separated value string
     * @return comma separated value string
     */
    std::string toCSV() const;
    /**
     * Gets a header for the variable 
     * @return header
     */
    virtual std::string getCVSHeader() const;

  protected:
    /**
     * Called by the frame work to update the state variable
     * @param dt time since last call [s]
     */
    virtual void updateData(float dt);

  private:
    /** Our robot */
    ARobot* mRobot;
    /** Start time [s] */
    float mStartTime;
    /** Timeout [s] */
    float mTimeout;
    /** Flags if the timer is running */
    bool mFgRunning;
    /** Flags if the timer has expired or not */
    bool mFgExpired;
};

} // namespace

#endif
