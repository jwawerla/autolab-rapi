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

#ifndef RAPISTAGEODOMETRY_H
#define RAPISTAGEODOMETRY_H

#include "odometry.h"
#include "stage.hh"

namespace Rapi
{

/**
 * Stage implementation of an odometry
 * @author Jens Wawerla <jwawerla@sfu.ca>
*/
class CStageOdometry : public COdometry
{
  public:
    /** Default constructor
     * @param stgModel stage position model
     * @param devName name of device
     */
    CStageOdometry( Stg::ModelPosition* stgModel, std::string devName);
    /** Default destructor */
    ~CStageOdometry();

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     */
    virtual void updateData();
    /** We are friends with stage drivetrain */
    friend class CStageDrivetrain2dof;

  private:
    /** Stage model */
    Stg::ModelPosition* mStgPosition;
};

} // namespace

#endif
