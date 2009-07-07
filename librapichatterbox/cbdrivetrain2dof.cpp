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
#include "cbdrivetrain2dof.h"
#include "utilities.h"
#include "rapierror.h"
#include "cbodometry.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBDrivetrain2dof::CCBDrivetrain2dof( CCBDriver* driver, std::string devName )
    : ADrivetrain2dof( devName )
{
  mCBDriver = driver;
  mOIMode = CB_MODE_SAFE;

  // maximal change in velocities
  mMaxTurnRateDelta = D2R( 5.0 );
  mMaxVelocityDelta = 0.1;

  // Create can drive max 0.5m/s
  mUpperVelocityLimit = CVelocity2d( 0.5, 0.0,  D2R( 30.0 ) );
  mLowerVelocityLimit = CVelocity2d( -0.5, 0.0, -D2R( 30.0 ) );

  // acceleration limits
  mTransAccelLimit.setLimit(-0.5, 0.1);
  mRotatAccelLimit.setLimit(-D2R(15.0), D2R(15.0));

  mOdometry = new CCBOdometry( mCBDriver, devName+":Odometry" );

  setEnabled( true );
}
//-----------------------------------------------------------------------------
CCBDrivetrain2dof::~CCBDrivetrain2dof()
{
  // make sure robot is stopped
  stop();
  updateData();

  if ( mOdometry )
    delete mOdometry;
}
//-----------------------------------------------------------------------------
int CCBDrivetrain2dof::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBDrivetrain2dof::setEnabled( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void CCBDrivetrain2dof::updateData()
{
  CPose2d pose;
  static CPose2d lastPose;
  static int count = 0;
  static int noProgressCount = 0;

  mOdometry->updateData();

  if ( mFgEnabled == true ) {
  /* FIXME: some problems measuring velocities mess up driving */
#if 0
    // read current velocities from chatterbox
    mVelocityMeas.mXDot = ( double )( mCBDriver->mCreateSensorPackage.velocity ) / 1e3;
    mVelocityMeas.mYDot = 0.0;
    if ( mCBDriver->mCreateSensorPackage.radius >= 0x7FFF )  // 0x7FFF or 0x8000
      mVelocityMeas.mYawDot = 0.0;
    else if ( mCBDriver->mCreateSensorPackage.radius == 0xFFFF )
      mVelocityMeas.mYawDot = D2R( -40.0 );   // TODO: find real maximum turnrate
    else if ( mCBDriver->mCreateSensorPackage.radius == 0x0001 )
      mVelocityMeas.mYawDot = D2R( 40.0 );    // TODO: find real maximum turnrate
    else if ( mCBDriver->mCreateSensorPackage.radius != 0 )
      mVelocityMeas.mYawDot = mCBDriver->mCreateSensorPackage.velocity /
                              mCBDriver->mCreateSensorPackage.radius;

    // limit acceleration
    if ( fabs( mPrevVelocityCmd.mXDot - mVelocityCmd.mXDot ) > mMaxVelocityDelta ) {
      mVelocityCmd.mXDot = mPrevVelocityCmd.mXDot +
                           sign( mVelocityCmd.mXDot - mPrevVelocityCmd.mXDot ) *
                           mMaxVelocityDelta;
    }

    if ( fabs( mVelocityMeas.mYawDot - mVelocityCmd.mYawDot ) > mMaxTurnRateDelta ) {
      mVelocityCmd.mYawDot = mPrevVelocityCmd.mYawDot +
                             sign( mVelocityCmd.mYawDot - mPrevVelocityCmd.mYawDot ) *
                             mMaxTurnRateDelta;
    }

    mPrevVelocityCmd = mVelocityCmd;
    // limit speeds
    applyVelocityLimits();
#endif

    // HACK: velocity measurement doesn't seem to work properly right now, instead we
    //       use the commanded velocity as our measurement
    mVelocityMeas = mVelocityLimitedCmd;

    // set OpenInterface Mode
    if ( mCBDriver->mCreateSensorPackage.oiMode != mOIMode )
      mCBDriver->setOIMode( mOIMode );

    // set speeds
    applyVelocityLimits();
    applyAccelerationLimits(CB_T);
    if ( mCBDriver->setSpeed( mVelocityLimitedCmd ) == 0 ) {
      ERROR2( "Failed to set speed command v=%f w=%f", mVelocityLimitedCmd.mXDot,
              mVelocityLimitedCmd.mYawDot );
    }

    // check if we are stuck
    pose = mOdometry->getPose();

    if ( count > 10 ) {
      count = 0;
      if ( fabs( pose.distance( lastPose ) ) < 0.01 )
        noProgressCount++;
      else
        noProgressCount = 0;

      if ( noProgressCount > 50 )
        mFgStalled = true;
      else
        mFgStalled = false;

      lastPose = pose;
    }
    count ++;
    mTimeStamp = timeStamp();
    if ( mFgStalled )
      mStalledTimer += CB_T;
    else
      mStalledTimer =0.0;
  } // enabled
}
//-----------------------------------------------------------------------------
void CCBDrivetrain2dof::print() const
{
  printf( "Drive: v=%01.3f m/s, w=%03.3f deg/s ",
          mVelocityLimitedCmd.mXDot,
          R2D( mVelocityLimitedCmd.mYawDot ) );
  mOdometry->print();

  switch ( mCBDriver->mCreateSensorPackage.oiMode ) {
    case CB_MODE_OFF:
      printf( " mode: OFF " );
      break;
    case CB_MODE_PASSIVE:
      printf( " mode: PASSIVE " );
      break;
    case CB_MODE_SAFE:
      printf( " mode: SAFE " );
      break;
    case CB_MODE_FULL:
      printf( " mode: FULL " );
      break;
  } // switch
  printf( "\n" );
}
//-----------------------------------------------------------------------------
void CCBDrivetrain2dof::setDefaultOIMode( tOIMode mode )
{
  mOIMode = mode;
}
//-----------------------------------------------------------------------------
tOIMode CCBDrivetrain2dof::getOIMode()
{
  return mCBDriver->getOIMode();
}
//-----------------------------------------------------------------------------
int CCBDrivetrain2dof::activateDemo( tDemo demo )
{
  if ( mCBDriver->activateDemo( demo ) == 1 ) {
    if ( demo == CB_DEMO_STOP )
      mOIMode = CB_MODE_SAFE;
    else
      mOIMode = CB_MODE_PASSIVE;

    return 1; // success
  }
  return 0; // failure
}
//-----------------------------------------------------------------------------

} // namespace
