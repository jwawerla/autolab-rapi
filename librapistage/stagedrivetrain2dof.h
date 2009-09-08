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

#ifndef RAPISTAGEDRIVETRAIN2DOF_H
#define RAPISTAGEDRIVETRAIN2DOF_H

#include "drivetrain2dof.h"
#include "stage.hh"

namespace Rapi
{

/**
 * Stage drivetrain with 2 dof
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CStageDrivetrain2dof : public ADrivetrain2dof
{
  // we are now friends with our robot
  friend class CStageRobot;

  public:
    /**
     * Default constructor
     * @param stgModel stage position model
     * @param devName name of device
     */
    CStageDrivetrain2dof ( Stg::ModelPosition* stgModel, std::string devName );
    /** Default destructor */
    ~CStageDrivetrain2dof();
    /**
      * Initializes the device
      * @param return 1 if success 0 otherwise
      */
    virtual int init();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled ( bool enable );

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt);
    /** Friend function of stage model updates */
    friend int positionUpdate ( Stg::ModelPosition* pos,
                                CStageDrivetrain2dof* drivetrain );
    /** Stage model */
    Stg::ModelPosition* mStgPosition;
};

} // namespace

#endif
