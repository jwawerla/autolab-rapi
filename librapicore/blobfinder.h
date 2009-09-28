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
#ifndef RAPIBLOBFINDER_H
#define RAPIBLOBFINDER_H

#include "device.h"

namespace Rapi
{

/** Type definition of fiducial data */
typedef struct {
  /** Range to target [m] */
  double range;
  /** Color of blob */
  CRgbColor color;
  /** Left border of blob [pixel] */
  unsigned int left;
  /** Right border of blob [pixel] */
  unsigned int right;
  /** Top border of blob [pixel] */
  unsigned int top;
  /** Bottom border of blob [pixel] */
  unsigned int bottom;
} tBlobData;

/**
 * Abstract base class for blob finder
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class ABlobFinder : public ADevice
{
  public:

    /** Default destructor */
    ~ABlobFinder();
    /**
     * Get device type
     * @return device type
     */
    virtual tRapiDeviceType getDeviceType() { return RAPI_BLOBFINDER; };
    /**
     * Gets pan angle of the camera
     * @return [rad]
     */
    double getPanAngle() const;
    /**
     * Gets the maximum sensing range
     * @return [m]
     */
    double getMaxRange() const;
    /**
     * Gets the field of view
     * @return [rad]
     */
    double getFov() const;
    /**
     * Gets the width of the scan
     * @return [pixel]
     */
    unsigned int getScanWidth() const;
    /**
     * Gets the hight of the scan
     * @return [pixel]
     */
    unsigned int getScanHeight() const; 
    /**
     * Gets the number of current fiducial readings
     */
    unsigned int getNumReadings() const;
    /**
     * Prints the devices main information
     */
    virtual void print() const;
        /**
     * Gets the name of a gui this device can be visualized
     * @return name of gui
     */
    virtual std::string getGuiName() { return "BlobFinderWidget"; };
    /** Blob data */
    tBlobData* mBlobData;

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ABlobFinder ( std::string devName );
    /** Pan angle [rad] */
    double mPanAngle;
    /** Maximum sensing range [m] */
    double mMaxRange;
    /** Field of view [rad] */
    double mFov;
    /** Number of readings */
    unsigned int mNumReadings;
    /** Scan width [pixel] */
    unsigned int mScanWidth;
    /** Scan height [pixel] */
    unsigned int mScanHeight;

};

}  // namespace

#endif
