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
#include "drivetrain2dofwidget.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CDrivetrain2dofWidget::CDrivetrain2dofWidget ( ADrivetrain2dof* drivetrain,
    QWidget* parent ) : ADeviceWidget ( parent )
{
  mDrivetrain = drivetrain;
  setTitle ( "Drivetrain" );

  mVelocityCmdWidget = new CVelocity2dWidget ( this );
  mMainLayout->addWidget ( mVelocityCmdWidget );

  mOdometryWidget = new CPose2dWidget ( this );
  mOdometryWidget->setTitle ( "Odometry" );
  mMainLayout->addWidget ( mOdometryWidget );
}
//-----------------------------------------------------------------------------
CDrivetrain2dofWidget::~CDrivetrain2dofWidget()
{
}
//-----------------------------------------------------------------------------
void CDrivetrain2dofWidget::updateData ()
{
  if ( mUpdateCheckBox->isChecked() ) {
    mOdometryWidget->setHidden(false);
    mVelocityCmdWidget->setHidden(false);
    ADeviceWidget::updateData ( mDrivetrain );
    mVelocityCmdWidget->setData ( mDrivetrain->getVelocityCmd() );
    mOdometryWidget->setData ( mDrivetrain->getOdometry()->getPose() );
  }
  else {
    mOdometryWidget->setHidden(true);
    mVelocityCmdWidget->setHidden(true);
  }
}
//-----------------------------------------------------------------------------

} // namespace
