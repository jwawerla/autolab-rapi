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
#include "chatterboxctrl.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
CChatterboxCtrl::CChatterboxCtrl ( ARobot* robot )
    : ARobotCtrl ( robot )
{
  ADrivetrain2dof* drivetrain;
  mTime = 0.0;
  mMinPhoto = INFINITY;
  mRedLedId = 0;
  mText = 0;
  mState = RUN;
  mFgRunDemo = false;
  mFgMotor = false;

  mRobot->findDevice ( mPowerPack, "CB:powerpack" );
  mRobot->findDevice ( drivetrain, "CB:drivetrain" );
  mRobot->findDevice ( mIr, "CB:ir" );
  mRobot->findDevice ( mTextDisplay, "CB:textdisplay" );
  mRobot->findDevice ( mBumper, "CB:bumper" );
  mRobot->findDevice ( mLights, "CB:lights" );
  mRobot->findDevice ( mWheelDrop, "CB:wheeldrop" );
  mRobot->findDevice ( mLowSideDriver, "CB:lowsidedriver" );
  mRobot->findDevice ( mButton, "CB:button" );
  mRobot->findDevice ( mFrontFiducial, "CB:front_fiducial");
  //mRobot->findDevice ( mTopFiducial, "CB:top_fiducial");
  mRobot->findDevice ( mPhoto, "CB:photosensor" );
  //mRobot->findDevice ( mLaser, "CB:laser" );

  mDrivetrain = ( CCBDrivetrain2dof* ) drivetrain;

  if ( rapiError->hasError() ) {
    rapiError->print();
    exit ( -1 );
  }

  ( ( CCBDrivetrain2dof* ) mDrivetrain )->setDefaultOIMode ( CB_MODE_FULL );
  // show some text, just for fun
  mTextDisplay->setText ( "1" );

  // set up a heart beat with 1Hz
  mLights->setBlink ( DOT, true, 1.0 );

  mLimit.setLimit ( 0.0, 0.5 );
}
//-----------------------------------------------------------------------------
CChatterboxCtrl::~CChatterboxCtrl()
{
}
//-----------------------------------------------------------------------------
bool CChatterboxCtrl::homeBaseDetected()
{
  unsigned char ir;

  ir = mFrontFiducial->getFiducialSignal();

  if ( ( ir == CB_RED_BUOY ) ||
       ( ir == CB_GREEN_BUOY ) ||
       ( ir == CB_FORCE_FIELD ) ||
       ( ir == CB_RED_GREEN_BUOY ) ||
       ( ir == CB_RED_BUOY_FORCE_FIELD ) ||
       ( ir == CB_GREEN_BUOY_FORCE_FIELD ) ||
       ( ir == CB_RED_GREEN_BUOY_FORCE_FIELD ) ) {
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
void CChatterboxCtrl::updateData ( float dt )
{

  char c[1];

  mTime += dt;
  mTimer += dt;

  if ( not mFgRunDemo ) {
    if ( mTime < 10.0 ) {
      c[0] = 57 - ( int ) mTime;
      mTextDisplay->setText ( c );
      if ( mButton->mBitData[0] )
        mFgRunDemo = true;
    }
    else {
      mRobot->quit();
    }
  }
  else {
    demo();
  }

  if ( mWheelDrop->isAnyTriggered() ) {
    mDrivetrain->stop();
    mLights->setLight ( ALL_LIGHTS, RED );
    mFgMotor = false;
  }


  if ( rapiError->hasError() ) {
    rapiError->print();
  }
}
//-----------------------------------------------------------------------------
void CChatterboxCtrl::demo()
{
  char blue = 0;
  char red = 0;

  //***************************************************
  // Enable motors ?
  if ( mButton->mBitData[1] )
    mFgMotor = true;


  //***************************************************
  // Demo for photo sensor
  mMinPhoto = min ( mMinPhoto, mPhoto->mData[0] );

  if ( mPhoto->mData[0] > 12.0 * mMinPhoto ) {
    if ( mRedLedId < 0 )
      mRedLedId = 0;

    if ( fmod ( mTime, 0.5 ) < 0.1 ) {
      mRedLedId ++;
      if ( mRedLedId > 4 )
        mRedLedId = 0;
    }
    mLights->setLight ( mRedLedId, GREEN );
  }
  else {
    mRedLedId = -1;
  }

  //***************************************************
  // Demo for RGB leds
  for ( unsigned int i = 0; i < mIr->getNumSamples(); i++ ) {
    if ( i != mRedLedId ) {
      if ( mIr->mRangeData[i].range >= 1.5 ) {
        mLights->setLight ( i, BLACK );
      }
      else {
        red  = ( int ) ( 255 * ( 1.0 - ( mIr->mRangeData[i].range / 1.5 ) ) );
        blue = ( int ) ( 255 * ( mIr->mRangeData[i].range / 1.5 ) );

        mLights->setLight ( i, red, 0, blue );
      }
    }
  }

  //****************************************************
  // Demo for 7Seg display
  if ( fmod ( mTime, 3.0 ) < 0.1 ) {
    mText ++;
    if ( mText > 15 )
      mText = 0;

    switch ( mText ) {
      case  0: mTextDisplay->setText ( "0" ); break;
      case  1: mTextDisplay->setText ( "1" ); break;
      case  2: mTextDisplay->setText ( "2" ); break;
      case  3: mTextDisplay->setText ( "3" ); break;
      case  4: mTextDisplay->setText ( "4" ); break;
      case  5: mTextDisplay->setText ( "5" ); break;
      case  6: mTextDisplay->setText ( "6" ); break;
      case  7: mTextDisplay->setText ( "7" ); break;
      case  8: mTextDisplay->setText ( "8" ); break;
      case  9: mTextDisplay->setText ( "9" ); break;
      case 10: mTextDisplay->setText ( "A" ); break;
      case 11: mTextDisplay->setText ( "B" ); break;
      case 12: mTextDisplay->setText ( "C" ); break;
      case 13: mTextDisplay->setText ( "D" ); break;
      case 14: mTextDisplay->setText ( "E" ); break;
      case 15: mTextDisplay->setText ( "F" ); break;
    } // switch
  }

  switch ( mState ) {
    case DOCKING:
      if ( mDrivetrain->getOIMode() != CB_MODE_PASSIVE ) {
        mDrivetrain->activateDemo ( CB_DEMO_DOCK );
      }
      if ( mTimer > 120 ) {
        mState = UNDOCKING;
        mTimer = 0;
      }
      break;

    case CHARGING:
      mLights->setLight ( ALL_LIGHTS, GREEN );
      if ( mTimer > 60 ) {
        mState = UNDOCKING;
        mTimer = 0;
      }
      break;

    case UNDOCKING:
      if ( mDrivetrain->getOIMode() != CB_MODE_FULL ) {
        mDrivetrain->setDefaultOIMode ( CB_MODE_FULL );
      }
      mDrivetrain->setVelocityCmd ( -0.1, 0.0 );
      if ( mTimer > 10 )
        mState = RUN;
      break;

    case RUN:
      if ( homeBaseDetected() ) {
        mState = DOCKING;
        mTimer = 0;
      }

      if ( mFgMotor )
        obstacleAvoid();
      else
        mDrivetrain->stop();
      break;
  } // switch
}
//-----------------------------------------------------------------------------
void CChatterboxCtrl::obstacleAvoid()
{
  float diff;
  float turnRate = 0.0;
  float velocity = 0.0;
  static float diffFilt = 0.0;

  //********************************************************
  // Obstacle avoidance

  // check left and right
  if ( ( mIr->mRangeData[1].range < 0.8 ) ||
       ( mIr->mRangeData[5].range < 0.8 ) ) {
    diff = mIr->mRangeData[1].range - mIr->mRangeData[5].range;
  }
  else {
    diff = 0;
  }
  diffFilt = diffFilt + 0.7 * ( diff - diffFilt );


  if ( mBumper->isAnyTriggered() ) {
    velocity = 0.0;
    if ( mBumper->mBitData[1] )
      turnRate = D2R ( -10.0 );
    if ( mBumper->mBitData[0] )
      turnRate = D2R ( 10.0 );
  }
  else if ( mIr->mRangeData[0].range < 0.55 ) {
    velocity = 0.0;
    turnRate = D2R ( 10.0 );

  }
  else {
    turnRate = limit ( diffFilt, D2R ( -30.0 ), D2R ( 30.0 ) );
    velocity = ( 1.0 - fabs ( turnRate ) / D2R ( 30.0 ) ) * 0.3;
    //printf("speed %f turnrate %f \n", velocity, turnRate);
  }
  velocity = mLimit.limit ( velocity );
  mDrivetrain->setVelocityCmd ( velocity, turnRate );
}
//-----------------------------------------------------------------------------
