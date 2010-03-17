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

#include "localizer2d.h"
#include "redisclient.h"


 namespace Rapi
{

/**
 * Interface class to access the Brian's overhead camera tracker. Pose
 * measurements are given in the coordinate system set by the
 * coordinate system offset (by default, absolute).
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CAutolabTracker : public ALocalizer2d
{
  public:
    /**
     * Default constructor
     * @param devName name of device
     * @param robotName name of robot
     * @param hostName of Redis server
     * @param port of Redis server
     */
    CAutolabTracker(std::string devName, std::string robotName, std::string hostName,
                    int port );
    /** Default destructor */
    virtual ~CAutolabTracker();
    /**
     * Updates the data of the device
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt );
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
    * Initializes the device
    * @param return 1 if success 0 otherwise
    */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled( bool enable );

  private:
    /** Redis client */
    CRedisClient* mRedisClient;
    /** Name of robot */
    std::string mRobotName;
    /** Id of camera on with the robot was last seen */
    int mCameraId;
};

} // namespace
