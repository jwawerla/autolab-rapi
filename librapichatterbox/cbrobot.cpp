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
#include "cbrobot.h"
#include <sys/time.h>
#include "rapierror.h"
#include "printerror.h"
#include <sys/utsname.h>
#include <assert.h>
namespace Rapi
{

//-----------------------------------------------------------------------------
CCBRobot::CCBRobot()
    : ARobot()
{
  struct utsname un;

  mUpdateInterval = 0.1; // default update frequency is 10Hz
  mLastLoopDuration = mUpdateInterval;

  // figure out name of robot, uname returns the full name, including
  // the domain name, which we don't need, so lets remove it
  uname(&un);
  mName = un.nodename;
  mName = mName.substr(0, mName.find(".") );

  mCBDriver = new CCBDriver();
  assert(mCBDriver);

  mCBDrivetrain = NULL;
  mCBPowerPack = NULL;
  mCBLaser = NULL;
  mCBIrSensor = NULL;
  mCBLights = NULL;
  mCBTextDisplay = NULL;
  mCBBumper = NULL;
  mCBWallSensor = NULL;
  mCBWheelDropSensor = NULL;
  mCBCliffSensor = NULL;
  mCBOverCurrentSensor = NULL;
  mCBLowSideDriver = NULL;
  mCBTopFiducial = NULL;
  mCBFrontFiducial = NULL;
  mCBPhotoSensor = NULL;
  mCBCreateButton = NULL;
  mCBVirtualWall = NULL;

  mSlowRunCount = 0;
  mSlowRunThreshold = 10;
  mFgRunning = true;
}
//-----------------------------------------------------------------------------
CCBRobot::~CCBRobot()
{
  mFgRunning = false;
  sleep( 2 );

  if ( mCBDrivetrain )
    delete mCBDrivetrain;

  if ( mCBPowerPack )
    delete mCBPowerPack;

  if ( mCBLaser )
    delete mCBLaser;

  if ( mCBLights )
    delete mCBLights;

  if ( mCBTextDisplay )
    delete mCBTextDisplay;

  if ( mCBBumper )
    delete mCBBumper;

  if ( mCBWallSensor )
    delete mCBWallSensor;

  if ( mCBWheelDropSensor )
    delete mCBWheelDropSensor;

  if ( mCBCliffSensor )
    delete mCBCliffSensor;

  if ( mCBOverCurrentSensor )
    delete mCBOverCurrentSensor;

  if ( mCBLowSideDriver );
  delete mCBLowSideDriver;

  if ( mCBFrontFiducial )
    delete mCBFrontFiducial;

  if ( mCBTopFiducial )
    delete mCBTopFiducial;

  if ( mCBPhotoSensor )
    delete mCBPhotoSensor;

  if ( mCBDriver )
    delete mCBDriver;

  if ( mCBCreateButton )
    delete mCBCreateButton;

  if ( mCBVirtualWall )
    delete mCBVirtualWall;
}
//-----------------------------------------------------------------------------
int CCBRobot::init()
{
  if ( mFgInitialized ) {
    PRT_WARN0( "Robot already initialized" );
    return 1;
  }
  if ( mCBDriver->init() == 0 ) {
    return 0; // failure
  }
  if ( mCBDriver->openPort( "/dev/ttyS2" ) == 0 ) {
    ERROR0( "Failed to open create port: /dev/ttyS2" );
    return 0; // failure
  }

  mFgInitialized = true;
  return 1; // success
}
//-----------------------------------------------------------------------------
double CCBRobot::getCurrentTime() const
{
  double timeNow;

  struct timeval tv;
  gettimeofday( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  return timeNow;
}
//-----------------------------------------------------------------------------
void CCBRobot::quit()
{
  if ( mCBDrivetrain )
    mCBDrivetrain->stop();

  mFgRunning = false;
}
//-----------------------------------------------------------------------------
void CCBRobot::run()
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    return;
  }

  while ( mFgRunning ) {
    // get data from ICreate
    printf("mLastLoopDuration %f \n",mLastLoopDuration);
    if ( mCBDriver->readSensorData( mLastLoopDuration ) == 1 ) {

      // update all devices
      if ( mCBDrivetrain )
        mCBDrivetrain->updateData( mUpdateInterval );
      if ( mCBPowerPack )
        mCBPowerPack->updateData( mLastLoopDuration );
      if ( mCBLaser )
        mCBLaser->updateData( mLastLoopDuration );
      if ( mCBLights )
        mCBLights->updateData( mLastLoopDuration );
      if ( mCBIrSensor )
        mCBIrSensor->updateData( mLastLoopDuration );
      if ( mCBTextDisplay )
        mCBTextDisplay->updateData( mLastLoopDuration );
      if ( mCBBumper )
        mCBBumper->updateData( mLastLoopDuration );
      if ( mCBWallSensor )
        mCBWallSensor->updateData( mLastLoopDuration );
      if ( mCBWheelDropSensor )
        mCBWheelDropSensor->updateData( mLastLoopDuration );
      if ( mCBCliffSensor )
        mCBCliffSensor->updateData( mLastLoopDuration );
      if ( mCBOverCurrentSensor )
        mCBOverCurrentSensor->updateData( mLastLoopDuration );
      if ( mCBFrontFiducial )
        mCBFrontFiducial->updateData( mLastLoopDuration );
      if ( mCBTopFiducial )
        mCBTopFiducial->updateData( mLastLoopDuration );
      if ( mCBPhotoSensor )
        mCBPhotoSensor->updateData( mLastLoopDuration );
      if ( mCBCreateButton )
        mCBCreateButton->updateData( mLastLoopDuration );
      if ( mCBVirtualWall )
        mCBVirtualWall->updateData( mLastLoopDuration );

      updateDevices();
      // Low side drivers updateData() is empty, no need to call it here
      //if ( mCBLowSideDriver )
      //  mCBLowSideDriver->updateData();
    }
    // update all registered constrollers
    updateControllers();

    //******************************************************
    // last step - keep everything in sync
    synchronize( mUpdateInterval );
  } // while
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ARangeFinder* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if (( devName != "CB:laser" ) &&
      ( devName != "CB:wall" ) &&
      ( devName != "CB:ir" ) ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  //************************************
  // Laser

  if ( devName == "CB:laser" ) {
    // check if device already exists
    if ( mCBLaser == NULL ) {
      mCBLaser = new CCBLaser( mCBDriver, "CB:laser" );
      device = mCBLaser;
      return mCBLaser->init();
    }

    // return already existing device
    device = mCBLaser;
    return 1; // success
  }

  //************************************
  // IR sensor
  if ( devName == "CB:ir" ) {
    // check if device already exists
    if ( mCBIrSensor == NULL ) {
      mCBIrSensor = new CCBIrSensor( mCBDriver, "CB:ir" );
      device = mCBIrSensor;
      return mCBIrSensor->init();
    }

    // return already existing device
    device = mCBIrSensor;
    return 1; // success
  }

  //************************************
  // Wall sensor
  if ( devName == "CB:wall" ) {
    // check if device already exists
    if ( mCBWallSensor == NULL ) {
      mCBWallSensor = new CCBWallSensor( mCBDriver, "CB:wall" );
      device = mCBWallSensor;
      return mCBWallSensor->init();
    }

    // return already existing device
    device = mCBWallSensor;
    return 1; // success
  }

  return 0; // should not be able to reach this, but silences compiler
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ADrivetrain2dof* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:drivetrain" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBDrivetrain == NULL ) {
    mCBDrivetrain = new CCBDrivetrain2dof( mCBDriver, "CB:drivetrain" );
    device = mCBDrivetrain;
    return mCBDrivetrain->init();
  }

  // return already existing device
  device = mCBDrivetrain;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( APowerPack* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:powerpack" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBPowerPack == NULL ) {
    mCBPowerPack = new CCBPowerPack( mCBDriver, "CB:powerpack" );
    device = mCBPowerPack;
    return mCBPowerPack->init();
  }

  // return already existing device
  device = mCBPowerPack;
  return 1;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( AFiducialFinder* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if (( devName != "CB:front_fiducial" ) &&
      ( devName != "CB:top_fiducial" ) ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  //************************************
  // Front fiducial finder
  if ( devName == "CB:front_fiducial" ) {
    // check if device already exists
    if ( mCBFrontFiducial == NULL ) {
      mCBFrontFiducial = new CCBFrontFiducialFinder( mCBDriver, "CB:front_fiducial" );
      device = mCBFrontFiducial;
      return mCBFrontFiducial->init();
    }

    // return already existing device
    device = mCBFrontFiducial;
    return 1; // success
  }

  //************************************
  // Top fiducial finder
  if ( devName == "CB:top_fiducial" ) {
    // check if device already exists
    if ( mCBTopFiducial == NULL ) {
      mCBTopFiducial = new CCBTopFiducialFinder( mCBDriver, "CB:top_fiducial" );
      device = mCBTopFiducial;
      return mCBTopFiducial->init();
    }

    // return already existing device
    device = mCBTopFiducial;
    return 1; // success
  }

  return 0; // error
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ALights* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:lights" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBLights == NULL ) {
    mCBLights = new CCBLights( mCBDriver, "CB:lights" );
    device = mCBLights;
    return mCBLights->init();
  }

  // return already existing device
  device = mCBLights;
  return 1;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ATextDisplay* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:textdisplay" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBTextDisplay == NULL ) {
    mCBTextDisplay = new CCBTextDisplay( mCBDriver, "CB:textdisplay" );
    device = mCBTextDisplay;
    return mCBTextDisplay->init();
  }

  // return already existing device
  device = mCBTextDisplay;
  return 1;
}
//-----------------------------------------------------------------------------
int  CCBRobot::findDevice( ASwitchArray* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:lowsidedriver" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBLowSideDriver == NULL ) {
    mCBLowSideDriver = new CCBLowSideDriver( mCBDriver, "CB:lowsidedriver" );
    device = mCBLowSideDriver;
    return mCBLowSideDriver->init();
  }

  // return already existing device
  device = mCBLowSideDriver;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ABlobFinder* &device, std::string devName )
{
  device = NULL;
  ERROR0( "Device not implementated for chatterbox" );
  return 0;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( AAnalogSensorArray* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:photosensor" ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBPhotoSensor == NULL ) {
    mCBPhotoSensor = new CCBPhotoSensor( mCBDriver, "CB:photosensor" );
    device = mCBPhotoSensor;
    return mCBPhotoSensor->init();
  }

  // return already existing device
  device = mCBPhotoSensor;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice( ABinarySensorArray* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if (( devName != "CB:bumper" ) &&
      ( devName != "CB:virtualwall" ) &&
      ( devName != "CB:cliff" ) &&
      ( devName != "CB:overcurrent" ) &&
      ( devName != "CB:button" ) &&
      ( devName != "CB:wheeldrop" ) ) {
    ERROR1( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  //************************************
  // Bumper
  if ( devName == "CB:bumper" ) {
    // check if device already exists
    if ( mCBBumper == NULL ) {
      mCBBumper = new CCBBumper( mCBDriver, "CB:bumper" );
      device = mCBBumper;
      return mCBBumper->init();
    }

    // return already existing device
    device = mCBBumper;
    return 1;
  }

  //************************************
  // Virtual wall
  if ( devName == "CB:virtualwall" ) {
    // check if device already exists
    if ( mCBVirtualWall == NULL ) {
      mCBVirtualWall = new CCBVirtualWallSensor( mCBDriver, "CB:virtualwall" );
      device = mCBVirtualWall;
      return mCBVirtualWall->init();
    }

    // return already existing device
    device = mCBVirtualWall;
    return 1;
  }

  //************************************
  // Button
  if ( devName == "CB:button" ) {
    // check if device already exists
    if ( mCBCreateButton == NULL ) {
      mCBCreateButton = new CCBCreateButton( mCBDriver, "CB:button" );
      device = mCBCreateButton;
      return mCBCreateButton->init();
    }

    // return already existing device
    device = mCBCreateButton;
    return 1;
  }

  //************************************
  // Wheel drop sensor
  if ( devName == "CB:wheeldrop" ) {
    // check if device already exists
    if ( mCBWheelDropSensor == NULL ) {
      mCBWheelDropSensor =
        new CCBWheelDropSensor( mCBDriver, "CB:wheeldrop" );
      device = mCBWheelDropSensor;
      return mCBWheelDropSensor->init();
    }

    // return already existing device
    device = mCBWheelDropSensor;
    return 1;
  }

  //************************************
  // Cliff sensor
  if ( devName == "CB:cliff" ) {
    // check if device already exists
    if ( mCBCliffSensor == NULL ) {
      mCBCliffSensor = new CCBCliffSensor( mCBDriver, "CB:cliff" );
      device = mCBCliffSensor;
      return mCBCliffSensor->init();
    }

    // return already existing device
    device = mCBCliffSensor;
    return 1;
  }

  //************************************
  // Overcurrent sensor
  if ( devName == "CB:overcurrent" ) {
    // check if device already exists
    if ( mCBOverCurrentSensor == NULL ) {
      mCBOverCurrentSensor = new CCBOverCurrentSensor( mCBDriver, "CB:overcurrent" );
      device = mCBOverCurrentSensor;
      return mCBOverCurrentSensor->init();
    }

    // return already existing device
    device = mCBOverCurrentSensor;
    return 1;
  }

  return 0; // should never get here
}
//-----------------------------------------------------------------------------
void CCBRobot::synchronize( double interval )
{
  double timeNow;
  double duration;
  double sleepDuration = 0.0;

  struct timeval tv;
  gettimeofday( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  duration = timeNow - mLastSynchronizeTime;
  if ( duration < interval ) {
    sleepDuration = interval - duration;
    mSlowRunCount = (mSlowRunCount > 0 ) ? mSlowRunCount - 1 : 0;
    usleep(( int )(( sleepDuration ) * 1e6 ) );
    mLastLoopDuration = interval;
  }
  else {
    mSlowRunCount = (mSlowRunCount < mSlowRunThreshold) ? mSlowRunCount + 1 :
                    mSlowRunCount;
    if( mSlowRunCount >= mSlowRunThreshold ) {
      mSlowRunCount = 0;
      mFgRunningSlowly = true;
      PRT_WARN0( "Control loop running consistently slowly\n" );
    }
    PRT_MSG0( 8, "Control loop ran slowly\n" );

    // the first time we often get outragously large values
    // which cause bad things to happen, so we limit this value
    // to the interval [0,5]
    mLastLoopDuration = limit(duration, 0.0, 5.0);
  }
  //gettimeofday( &tv, 0 );
  //mLastSynchronizeTime = tv.tv_sec + tv.tv_usec * 1e-6;
  mLastSynchronizeTime = timeNow + sleepDuration;
  //printf("timeNow  %f\n", timeNow);
}
//-----------------------------------------------------------------------------
} // namespace
