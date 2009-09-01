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
#ifndef RAPILOCALIZER2D_H
#define RAPILOCALIZER2D_H

#include "device.h"
#include "pose2d.h"

namespace Rapi
{

/**
 * Abstract base class for 2d (x,y, yaw) localizer devices, e.g. odometry,
 * gps etc.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class ALocalizer2d : public ADevice
{
  public:
    /** Default destructor */
    virtual  ~ALocalizer2d();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() {return RAPI_LOCALIZER; };
    /**
     * Sets the offset of the coordinate system
     * @param offset
     */
    void setCoordinateSystemOffset( CPose2d offset );
    /**
     * Sets the current pose
     * @param pose
     */
    void setPose( CPose2d pose );
    /**
     * Sets the pose to (0,0,0)
     */
    void setToZero();
    /**
     * Gets the current pose
     * @return pose
     */
    CPose2d getPose() const;

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ALocalizer2d( std::string devName );
    /** Current pose */
    CPose2d mPose;
    /** Offset of coordinate system */
    CPose2d mCoordinateSystemOffset;
};

} // namespace

#endif
