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

#ifndef RAPIBUMPER_H
#define RAPIBUMPER_H

#include "device.h"
#include "pose2d.h"

namespace Rapi
{

/**
 * Abstract base class for bumber sensor
 * @author Jens Wawerla
 */
class ABumper : public ADevice
{
  public:
    /** Default destructor */
    ~ABumper();
    /**
     * Gets the number of bumper samples
     * @return number of samples
     */
    unsigned int getNumSamples();
    /** Bumper data */
    bool* mBumper;
    /** Pose information for each bumper */
    CPose2d* mBumperPose;
    /**
     * Prints the devices main information
     */
    virtual void print();
    /**
     * Checks if any bumper was triggered
     * @return true if one bumper was triggered, false otherwise
     */
    virtual bool isAnyTriggered();

  protected:
    /**
     * Default constructor
     * @param devName name of device
     */
    ABumper ( std::string devName );
    /** Number of bumper samples */
    unsigned int mNumSamples;
    /** Flags if any bumber was triggered */
    bool mFgAnyTriggered;
};

} // namespace

#endif
