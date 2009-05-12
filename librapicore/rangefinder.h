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

#ifndef ARANGEFINDER_H
#define ARANGEFINDER_H

#include "pose2d.h"
#include "point2d.h"
#include "device.h"

namespace Rapi
{

/** Type definition of a range data */
typedef struct {
  /** Range for sensor to object or max sensor range [m] */
  double range;
  /** Intensity of the reflection 0.0 to 1.0 */
  double reflectance;
} tRangeData;

/**
 * Abstract base class for all range finder devices such as lasers,
 * IR sensors or sonars
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 11/2007
 */
class ARangeFinder : public ADevice
{
  public:
    /** Default destructor */
    virtual ~ARangeFinder();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_RANGEFINDER; };
    /** Pose of a beam, relative to the parents position */
    CPose2d* mRelativeBeamPose;
    /** Range data of a beam, range [m], reflectance [0,1] */
    tRangeData* mRangeData;
    /** Number of samples in one scan */
    unsigned int getNumSamples();
    /**
     * Gets the angle of the beam cone, for this like lases the
     * angle is practically 0
     * @return angle [rad]
     */
    float getBeamConeAngle();
    /**
     * Calculates the response for a point in the sensors local coordinate
     * system from the inverse range sensor model
     * @param x ordinate of point (global cs)
     * @param y ordinate of point (global cs)
     * @param robotPos map center relative position of robot
     * @param heading of robot [rad]
     * @return log odds of obstacle at (x,y)
     */
    float localInverseRangeSensorModel( float x, float y, CPoint2d robotPos,
                                        float heading );
    /**
     * Calculates the response for a point in the sensors local coordinate
     * system from the inverse range sensor model
     * @param x ordinate of point
     * @param y ordinate of point
     * @param robotPose map center relative pose of robot
     * @return log odds of obstacle at (x,y)
     */
    float localInverseRangeSensorModel( float x, float y, CPose2d robotPose );
    /**
     * Gets the maximum possible range of the sensor
     * @return [m]
     */
    float getMaxRange();
    /**
     * Gets the minimum possible range of the sensor
     * @return [m]
     */
    float getMinRange();
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Gets the field of view
     * @return [rad]
     */
    float getFov();
    /**
     * Gets the name of a gui this device can be visualized
     * @return name of gui
     */
    virtual std::string getGuiName() { return "RangeFinderWidget"; };

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ARangeFinder( std::string devName );
    /** Number sample readings */
    unsigned int mNumSamples;
    /** Maximum range [m] */
    float mMaxRange;
    /** Minimum range [m] */
    float mMinRange;
    /** Range resolution of a sample [m] */
    float mRangeResolution;
    /** Angle of the beam cone [rad] */
    float mBeamConeAngle;
    /** Log odds of occupied cell */
    float mLOccupied;
    /** Log odds for free cell */
    float mLFree;
    /** Half obstacle thickness [m] */
    float mHalfObstacleThickness;
    /** Log liklihood for prior, basically for no information */
    float mL0;
    /** Field of view [rad] */
    float mFov;
};

} // namespace
#endif
