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
#include "stagedrivetrain2dof.h"
#include "stageodometry.h"
#include "utilities.h"

namespace Rapi
{
//-----------------------------------------------------------------------------
// Callback for stage, stage calls this function if the corresponding model
// is updated
int positionUpdate ( Stg::ModelPosition* pos, CStageDrivetrain2dof* drivetrain )
{
  drivetrain->updateData(pos->GetUpdateInterval() );
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageDrivetrain2dof::CStageDrivetrain2dof ( Stg::ModelPosition* stgModel,
    std::string devName )
    : ADrivetrain2dof ( devName )
{
  assert ( stgModel );
  mStgPosition = stgModel;
  mFgEnabled = false;
  mStgPosition->AddUpdateCallback ( ( Stg::stg_model_callback_t )
                                    positionUpdate,
                                    this );
  mOdometry = new CStageOdometry ( mStgPosition, devName + ":odometry" );
  assert ( mOdometry );

  // velocity limits
  mUpperVelocityLimit = CVelocity2d( 1.0, 0.0,  D2R(60.0) );
  mLowerVelocityLimit = CVelocity2d(-1.0, 0.0, -D2R(60.0) );

  // acceleration limits
  mTransAccelLimit.setLimit(-1.0, 1.0);
  mRotatAccelLimit.setLimit(-D2R(30.0), D2R(30.0));

  setEnabled ( true );
}
//-----------------------------------------------------------------------------
CStageDrivetrain2dof::~CStageDrivetrain2dof()
{
  if ( mOdometry )
    delete mOdometry;
}
//-----------------------------------------------------------------------------
int CStageDrivetrain2dof::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageDrivetrain2dof::setEnabled ( bool enable )
{
  if ( mFgEnabled == enable )
    return;  // we are really have the correct status

  if ( enable )
    mStgPosition->Subscribe();
  else
    mStgPosition->Unsubscribe();

  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void CStageDrivetrain2dof::updateData( const double dt)
{
  float time;
  if ( mFgEnabled ) {
    applyVelocityLimits();
    applyAccelerationLimits( mStgPosition->GetPoseInterval() / 1e6);
    mStgPosition->SetXSpeed ( mVelocityLimitedCmd.mXDot );
    mStgPosition->SetTurnSpeed ( mVelocityLimitedCmd.mYawDot );

    // update odometry
    ( ( CStageOdometry* ) mOdometry )->updateData(dt);

    // stage doesn't seem to allow us access to the sensed speed of the robot
    // so we simply assume it is the commanded velocity
    mVelocityMeas = mVelocityLimitedCmd;
    time = mTimeStamp;
    mTimeStamp = mStgPosition->GetWorld()->SimTimeNow() / 1e6;
    mFgStalled = mStgPosition->Stalled();

    if (mFgStalled)
      mStalledTimer += mTimeStamp - time;
    else
      mStalledTimer = 0.0;

    notifyDataUpdateObservers();
  }
}
//-----------------------------------------------------------------------------
} // namespace
