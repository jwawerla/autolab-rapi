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

#ifndef RAPIROBOT_H
#define RAPIROBOT_H

#include "robotctrlinterface.h"
#include "rangefinder.h"
#include "drivetrain2dof.h"
#include "powerpack.h"
#include "fiducialfinder.h"
#include "lights.h"
#include "textdisplay.h"
#include <list>
#include <string>

namespace Rapi
{

/**
 * Base class for all robot. Robot are used as a central point to obtain all
 * devices the robot is configured with.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class ARobot
{
  public:
    /** Default destructor */
    virtual ~ARobot();
    /**
     * Ininitializes the robot
     * @return 1 if successfull, 0 otherwise
     */
    virtual int init() = 0;
    /**
     * Registers a robot controller with the frame work. The frame work
     * calls the update method of a registered controller in every time
     * step.
     * @param ctrl to be registered
     */
    void registerRobotController ( ARobotCtrlInterface* ctrl );
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( ARangeFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ADrivetrain2dof* &device,  std::string devName ) = 0;
    virtual int findDevice ( APowerPack* &device, std::string devName ) = 0;
    virtual int findDevice ( AFiducialFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ALights* &device, std::string devName ) = 0;
    virtual int findDevice ( ATextDisplay* &device, std::string devName ) = 0;

  protected:
    /** Default constructor */
    ARobot();
    /**
     * Calls the update method in all registered robot controllers
     */
    void updateControllers();
    /** Robot controller list */
    std::list<ARobotCtrlInterface*> mRobotCtrlList;
    /** Update interval [s] */
    double mUpdateInterval;

};

} // namespace
#endif
