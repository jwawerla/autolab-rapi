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

#ifndef RAPISTAGESONAR_H
#define RAPISTAGESONAR_H

#include "stage.hh"
#include "rangefinder.h"

namespace Rapi
{

/**
 * This class provides a sonar simulated by stage
 *  @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CStageSonar : public ARangeFinder
{
  // we are now friends with our robot
  friend class CStageRobot;

  public:
    /**
     * Default constructor
     * @param stgModel stage model of sonar
     * @param devName name of device
     */
    CStageSonar ( Stg::ModelRanger* stgModel, std::string devName );
    /** Default destructor */
    ~CStageSonar();
    /**
     * Initializes the robot
     * @param return 1 if success -1 otherwise
     */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );
    /**
     * Prints the devices main information
     */
    virtual void print();

  protected:
    /**
     * This method gets called by the framework every step to refresh
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt);
    /** Friend function of stage model updates */
    friend int sonarUpdate ( Stg::ModelRanger* ranger, CStageSonar* sonar );
    /** Stage model */
    Stg::ModelRanger* mStgRanger;
};

} // namespace

#endif
