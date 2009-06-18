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
#ifndef RAPIODOMETRY_H
#define RAPIODOMETRY_H

#include "localizer2d.h"


namespace Rapi
{

/**
 * A simple odometry
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class COdometry : public ALocalizer2d
{
  public:
    /**
     * Default constructor
     * @param devName name of device
     */
    COdometry ( std::string devName );
    /** Default destructor */
    ~COdometry();
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
    /**
    * Prints the devices main information
    */
    virtual void print() const;
    /**
     * Start logging of the devices data to a file. Note there can only be
     * one log file for all devices, hence the first device to enable logging
     * determines the name of the log file.
     * @param filename to log to
     */
    virtual void startLogging(std::string filename);

  protected:
    /**
     * Updates the data of the device
     */
    void updateData();
};

} // namespace

#endif
