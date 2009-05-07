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

#ifndef RAPISTATEVARIABLE_H
#define RAPISTATEVARIABLE_H

#include "robotupdateinterface.h"
#include "robot.h"

namespace Rapi
{

/**
 * Abstract base class for state variables
 * @author Jens Wawerla
 */
class AStateVariable : public IRobotUpdate
{
  public:
    /** Default destructor */
    virtual ~AStateVariable();
    /**
     * Checks if variable has changed since last time step
     * @return true if changed, false other wise
     */
    bool hasChanged() { return mFgModified; };
    /**
     * Sets a robot for this variable to handle automatic time tracking
     * @param robot this variable is used by
     */
    void setRobot ( ARobot* robot );
    /**
     * Gets the time of the last modification
     * @return [s]
     */
    double getModificationTime() { return mModificationTimestamp; };

  protected:
    /**
     * Default constructor
     */
    AStateVariable();
    /** Time stamp of last update [s] */
    double mUpdateTimestamp;
    /** Time stamp of last modification [s] */
    double mModificationTimestamp;
    /** Flags if the value of the variable has been modified or not */
    bool mFgModified;
};

}  // namespace

#endif
