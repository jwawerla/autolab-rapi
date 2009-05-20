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
#ifndef RAPIDRIVETRAINWIDGET_H
#define RAPIDRIVETRAINWIDGET_H

#include "devicewidget.h"
#include "drivetrain2dof.h"
#include "velocity2dwidget.h"
#include "pose2dwidget.h"
#include "dataled.h"


namespace Rapi {

/**
 * A Qt widget to visualize a drivetrain
 * @author Jens Wawerla
 */
class CDrivetrain2dofWidget : public ADeviceWidget
{
  public:
   /**
    * Default constructor
    * @param drivetrain
    * @param parent
    */
    CDrivetrain2dofWidget(ADrivetrain2dof* drivetrain, QWidget* parent);
    /** Default destructor */
    ~CDrivetrain2dofWidget();
    /** Update the data of the display */
    void updateData ();

  protected:
    /** Drivetrain we visualize */
    ADrivetrain2dof* mDrivetrain;
    /** Display for velocity command */
    CVelocity2dWidget* mVelocityCmdWidget;
    /** Display measured velocity */
    CVelocity2dWidget* mVelocityMeasWidget;
    /** Display for odometry */
    CPose2dWidget* mOdometryWidget;
    /** Stalled led */
    CDataLed* mStalledLed;
};

} // namespace

#endif
