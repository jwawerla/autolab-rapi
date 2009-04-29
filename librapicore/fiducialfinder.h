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
#ifndef RAPIFIDUCIALFINDER_H
#define RAPIFIDUCIALFINDER_H

#include "device.h"

namespace Rapi
{

/** Type definition of fiducial data */
typedef struct {
  /** Range to target [m] */
  double range;
  /** Bearing to target [rad] */
  double bearing;
  /** Id of target, 0 if target has no id */
  int id;
} tFiducialData;

/**
 * Abstract base class for fiducial finder
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class AFiducialFinder : public ADevice
{
  public:

    /** Default destructor */
    ~AFiducialFinder();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_FIDUCIALFINDER; };
    /**
     * Gets the minimum sensing range
     * @return [m]
     */
    double getMinRange();
    /**
     * Gets the maximum sensing range
     * @return [m]
     */
    double getMaxRange();
    /**
     * Gets the field of view
     * @return [rad]
     */
    double getFov();
    /**
     * Gets the number of current fiducial readings
     */
    unsigned int getNumReadings();
    /**
     * Prints the devices main information
     */
    virtual void print();
    /** Fiducial data */
    tFiducialData* mFiducialData;

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    AFiducialFinder ( std::string devName );
    /** Minimum sensing range [m] */
    double mMinRange;
    /** Maximum sensing range [m] */
    double mMaxRange;
    /** Field of view [rad] */
    double mFov;
    /** Number of readings */
    unsigned int mNumReadings;


};

}  // namespace

#endif
