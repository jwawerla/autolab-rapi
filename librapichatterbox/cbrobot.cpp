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

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBRobot::CCBRobot()
    : ARobot()
{
  mUpdateInterval = CB_T;
  mName = "Chatterbox";

  mCBDriver = new CCBDriver();

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

  mFgRunning = true;
}
//-----------------------------------------------------------------------------
CCBRobot::~CCBRobot()
{
  mFgRunning = false;
  sleep(2);

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

  if (mCBLowSideDriver);
    delete mCBLowSideDriver;

  if ( mCBDriver )
    delete mCBDriver;
}
//-----------------------------------------------------------------------------
int CCBRobot::init()
{
  if ( mFgInitialized ) {
    PRT_WARN0 ( "Robot already initialized" );
    return 1;
  }
  if ( mCBDriver->init() == 0 ) {
    return 0; // failure
  }
  if ( mCBDriver->openPort ( "/dev/ttyS2" ) == 0 ) {
    ERROR0 ( "Failed to open create port: /dev/ttyS2" );
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
  gettimeofday ( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  return timeNow;
}
//-----------------------------------------------------------------------------
void CCBRobot::terminate()
{
  printf(" CCBRobot::terminate() \n");
  mFgRunning = false;
}
//-----------------------------------------------------------------------------
void CCBRobot::run ()
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    return;
  }

  while ( mFgRunning ) {
    // get data from ICreate
    if ( mCBDriver->readSensorData() == 1 ) {

      // update all devices
      if ( mCBDrivetrain )
        mCBDrivetrain->updateData();
      if ( mCBPowerPack )
        mCBPowerPack->updateData();
      if ( mCBLaser )
        mCBLaser->updateData();
      if ( mCBLights )
        mCBLights->updateData();
      if ( mCBIrSensor )
        mCBIrSensor->updateData();
      if ( mCBTextDisplay )
        mCBTextDisplay->updateData();
      if ( mCBBumper )
        mCBBumper->updateData();
      if ( mCBWallSensor )
        mCBWallSensor->updateData();
      if ( mCBWheelDropSensor )
        mCBWheelDropSensor->updateData();
      if ( mCBCliffSensor )
        mCBCliffSensor->updateData();
      if ( mCBOverCurrentSensor )
        mCBOverCurrentSensor->updateData();

      // Low side drivers updateData() is empty, no need to call it here
      //if ( mCBLowSideDriver )
      //  mCBLowSideDriver->updateData();
    }
    // update all registered constrollers
    updateControllers();

    //******************************************************
    // last step - keep everything in a 100 ms loop
    synchronize ( CB_T );
  } // while
 printf("DONE\n");
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ARangeFinder* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( ( devName != "CB:laser" ) &&
       ( devName != "CB:wall" ) &&
       ( devName != "CB:ir" ) ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  //************************************
  // Laser

  if ( devName == "CB:laser" ) {
    // check if device already exists
    if ( mCBLaser == NULL ) {
      mCBLaser = new CCBLaser ( mCBDriver, "CB:laser" );
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
      mCBIrSensor = new CCBIrSensor ( mCBDriver, "CB:ir" );
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
      mCBWallSensor = new CCBWallSensor ( mCBDriver, "CB:wall" );
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
int CCBRobot::findDevice ( ADrivetrain2dof* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:drivetrain" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBDrivetrain == NULL ) {
    mCBDrivetrain = new CCBDrivetrain2dof ( mCBDriver, "CB:drivetrain" );
    device = mCBDrivetrain;
    return mCBDrivetrain->init();
  }

  // return already existing device
  device = mCBDrivetrain;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( APowerPack* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:powerpack" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBPowerPack == NULL ) {
    mCBPowerPack = new CCBPowerPack ( mCBDriver, "CB:powerpack" );
    device = mCBPowerPack;
    return mCBPowerPack->init();
  }

  // return already existing device
  device = mCBPowerPack;
  return 1;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( AFiducialFinder* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  ERROR0 ( "Fiducial finder not available for Chatterbox" );
  device = NULL;
  return 0; // error
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ALights* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:lights" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBLights == NULL ) {
    mCBLights = new CCBLights ( mCBDriver, "CB:lights" );
    device = mCBLights;
    return mCBLights->init();
  }

  // return already existing device
  device = mCBLights;
  return 1;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ATextDisplay* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:textdisplay" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBTextDisplay == NULL ) {
    mCBTextDisplay = new CCBTextDisplay ( mCBDriver, "CB:textdisplay" );
    device = mCBTextDisplay;
    return mCBTextDisplay->init();
  }

  // return already existing device
  device = mCBTextDisplay;
  return 1;
}
//-----------------------------------------------------------------------------
int  CCBRobot::findDevice ( ASwitchArray* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( devName != "CB:lowsidedriver" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBLowSideDriver == NULL ) {
    mCBLowSideDriver = new CCBLowSideDriver ( mCBDriver, "CB:lowsidedriver" );
    device = mCBLowSideDriver;
    return mCBLowSideDriver->init();
  }

  // return already existing device
  device = mCBLowSideDriver;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ABinarySensorArray* &device, std::string devName )
{
  if ( not mFgInitialized ) {
    PRT_WARN0 ( "Robot is not initialized, call init() first" );
    device = NULL;
    return 0; // error
  }

  if ( ( devName != "CB:bumper" ) &&
       ( devName != "CB:cliff" ) &&
       ( devName != "CB:overcurrent" ) &&
       ( devName != "CB:wheeldrop" ) ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  //************************************
  // Bumper
  if ( devName == "CB:bumper" ) {
    // check if device already exists
    if ( mCBBumper == NULL ) {
      mCBBumper = new CCBBumper ( mCBDriver, "CB:bumper" );
      device = mCBBumper;
      return mCBBumper->init();
    }

    // return already existing device
    device = mCBBumper;
    return 1;
  }

  //************************************
  // Wheel drop sensor
  if ( devName == "CB:wheeldrop" ) {
    // check if device already exists
    if ( mCBWheelDropSensor == NULL ) {
      mCBWheelDropSensor =
        new CCBWheelDropSensor ( mCBDriver, "CB:wheeldrop" );
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
      mCBCliffSensor = new CCBCliffSensor ( mCBDriver, "CB:cliff" );
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
      mCBOverCurrentSensor = new CCBOverCurrentSensor ( mCBDriver, "CB:overcurrent" );
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
void CCBRobot::synchronize ( double interval )
{
  double timeNow;
  double duration;

  struct timeval tv;
  gettimeofday ( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  duration = timeNow - mLastSynchronizeTime;
  if ( duration < interval ) {
    usleep ( ( int ) ( duration * 1e6 ) );
  }

  mLastSynchronizeTime = tv.tv_sec + tv.tv_usec * 1e-6;
}
//-----------------------------------------------------------------------------

} // namespace
