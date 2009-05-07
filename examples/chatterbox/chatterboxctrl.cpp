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

//-----------------------------------------------------------------------------
CChatterboxCtrl::CChatterboxCtrl ( ARobot* robot )
    : Rapi::ARobotCtrl ( robot )
{

  mRobot->findDevice ( mPowerPack, "CB:powerpack" );
  mRobot->findDevice ( mDrivetrain, "CB:drivetrain" );
  mRobot->findDevice ( mIr, "CB:ir" );
  mRobot->findDevice ( mTextDisplay, "CB:textdisplay" );
  mRobot->findDevice ( mBumper, "CB:bumper");
  mRobot->findDevice( mLights, "CB:lights");

  if ( rapiError->hasError() ) {
    rapiError->print();
    exit(-1);
  }

  // show some text, just for fun
  mTextDisplay->setText("1");

  // set up a heart beat with 1Hz
  mLights->setBlink(5, true, 1.0);
}
//-----------------------------------------------------------------------------
CChatterboxCtrl::~CChatterboxCtrl()
{
}
//-----------------------------------------------------------------------------
void CChatterboxCtrl::updateData ( float dt )
{
  static unsigned char c = 0;


  ((CCBDrivetrain2dof*)mDrivetrain)->setDefaultOIMode(CB_MODE_FULL);

   c++;
   mLights->setLight(-1, CRgbColor( c, c, c) );
   mLights->setBlink(5, true, 1.0);
   mTextDisplay->setText("1");

  //mDrivetrain->setSpeedCmd(0.3, 0.0);
  //obstacleAvoid();

  if ( rapiError->hasError() ) {
    rapiError->print();
  }
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
  if ( (mIr->mRangeData[1].range < 0.8) ||
       (mIr->mRangeData[5].range < 0.8) ) {
    diff = mIr->mRangeData[1].range - mIr->mRangeData[5].range;
  }
  else {
    diff = 0;
  }
  diffFilt = diffFilt + 0.7 * (diff - diffFilt);


  if (mBumper->isAnyTriggered() ) {
    velocity = 0.0;
    if (mBumper->mBitData[1] )
      turnRate = D2R(-10.0);
    if (mBumper->mBitData[0] )
      turnRate = D2R(10.0);
  }
  else if (mIr->mRangeData[0].range < 0.4) {
    velocity = 0.0;
    turnRate = D2R(10.0);

  }
  else {
    turnRate = LIMIT( diffFilt, D2R(-30.0), D2R(30.0));
    velocity = (1.0 -fabs(turnRate) / D2R(30.0) ) * 0.3;
    //printf("speed %f turnrate %f \n", velocity, turnRate);
  }
  mDrivetrain->setSpeedCmd(velocity, turnRate);
}
//-----------------------------------------------------------------------------
