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

#ifndef RAPIWAYPOINT2D_H
#define RAPIWAYPOINT2D_H

#include "pose2d.h"
#include "rgbcolor.h"
#include <string>

#define DEFAULT_WP_COLOR CRgbColor(255,0,0)

namespace Rapi
{

/**
 * This class defines a waypoint in 2D
 * @author Jens Wawerla
 */
class CWaypoint2d
{
  public:
    /**
     * Default constructor
     * @param x
     * @param y
     * @param yaw
     * @param label
    * @param color
     */
    CWaypoint2d ( double x = 0.0, double y = 0.0, double yaw = 0.0,
                  std::string label="", CRgbColor color=DEFAULT_WP_COLOR );
    /**
     * Default constructor
     * @param pose
     * @param label
     * @param color
     */
    CWaypoint2d ( CPose2d pose, std::string label="",
                  CRgbColor color=DEFAULT_WP_COLOR );
    /** Default destructor */
    ~CWaypoint2d();
    /**
     * Prints the pose data to std out
     */
    void print() const;
    /** Overloaded = operator */
    CWaypoint2d& operator= ( const CPose2d& pose );
    /** Overloaded = operator */
    CWaypoint2d& operator= ( const CWaypoint2d& wp );
    /**
     * Gets the pose of the waypoint
     * @return pose
     */
    CPose2d getPose() const;
    /**
     * Set pose of waypoint
     * @param pose
     */
    void setPose(CPose2d pose);
    /**
     * Sets the pose of a waypoint
     * @param point
     * @param heading [rad]
     */
    void setPose(CPoint2d point, double heading = 0.0);
    /**
     * Gets the label of the waypoint
     * @return label
     */
    std::string getLabel() const;
    /**
     * Sets the label
     * @param label
     */
    void setLabel ( std::string label );
    /**
     * Gets the color of the waypoint
     * @return color
     */
    CRgbColor getColor() const;
    /**
     * Sets the color of the waypoint
     * @param color
     */
    void setColor(CRgbColor color);
    /**
     * Converts the point data to a string
     * @return string
     */
    std::string toStr() const;

  protected:
    /** Pose of waypoint */
    CPose2d mPose;
    /** Label */
    std::string mLabel;
    /** Color of waypoint */
    CRgbColor mColor;
};

} // namespace

#endif
