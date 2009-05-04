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

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBRobot::CCBRobot()
    : ARobot()
{
  mUpdateInterval = CB_T;

  mCBDrivetrain = NULL;
  mCBPowerPack = NULL;
  mCBLaser = NULL;
  mCBIrSensor = NULL;
  mCBLights = NULL;
  mCBTextDisplay = NULL;
  mCBBumper = NULL;

  mFgRunning = true;
}
//-----------------------------------------------------------------------------
CCBRobot::~CCBRobot()
{
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
}
//-----------------------------------------------------------------------------
int CCBRobot::init()
{
  if ( mCBDriver.init() == 0 ) {
    return 0; // failure
  }
  if ( mCBDriver.openPort ( "/dev/ttyS2" ) == 0 ) {
    ERROR0 ( "Failed to open create port: /dev/ttyS2" );
    return 0; // failure
  }

  return 1; // success
}
//-----------------------------------------------------------------------------
double CCBRobot::getCurrentTime()
{
  double timeNow;

  struct timeval tv;
  gettimeofday ( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  return timeNow;
}
//-----------------------------------------------------------------------------
void CCBRobot::run ()
{
  while ( mFgRunning ) {

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

    // update all registered constrollers
    updateControllers();

    //******************************************************
    // last step - keep everything in a 100 ms loop
    synchronize ( CB_T );
  } // while
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ARangeFinder* &device, std::string devName )
{
  if ( ( devName != "CB:laser" ) &&
       ( devName != "CB:ir" ) ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }


  if ( devName == "CB:laser" ) {
    // check if device already exists
    if ( mCBLaser == NULL ) {
      mCBLaser = new CCBLaser ( &mCBDriver, "CB:laser" );
      device = mCBLaser;
      return mCBLaser->init();
    }

    // return already existing device
    device = mCBLaser;
    return 1;
  }

  if ( devName == "CB:ir" ) {
    // check if device already exists
    if ( mCBIrSensor == NULL ) {
      mCBIrSensor = new CCBIrSensor ( &mCBDriver, "CB:ir" );
      device = mCBIrSensor;
      return mCBIrSensor->init();
    }

    // return already existing device
    device = mCBIrSensor;
    return 1;
  }

  return 0; // should not be able to reach this, but silences compiler
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ADrivetrain2dof* &device, std::string devName )
{
  if ( devName != "CB:drivetrain" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBDrivetrain == NULL ) {
    mCBDrivetrain = new CCBDrivetrain2dof ( &mCBDriver, "CB:drivetrain" );
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
  if ( devName != "CB:powerpack" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBPowerPack == NULL ) {
    mCBPowerPack = new CCBPowerPack ( &mCBDriver, "CB:powerpack" );
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
  ERROR0 ( "Fiducial finder not available for Chatterbox" );
  device = NULL;
  return 0; // error
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ALights* &device, std::string devName )
{
  if ( devName != "CB:lights" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBLights == NULL ) {
    mCBLights = new CCBLights ( &mCBDriver, "CB:lights" );
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
  if ( devName != "CB:textdisplay" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBTextDisplay == NULL ) {
    mCBTextDisplay = new CCBTextDisplay ( &mCBDriver, "CB:textdisplay" );
    device = mCBTextDisplay;
    return mCBTextDisplay->init();
  }

  // return already existing device
  device = mCBTextDisplay;
  return 1;
}
//-----------------------------------------------------------------------------
int CCBRobot::findDevice ( ABumper* &device, std::string devName )
{
  if ( devName != "CB:bumper" ) {
    ERROR1 ( "No such device: %s", devName.c_str() );
    device = NULL;
    return 0; // error
  }

  // check if device already exists
  if ( mCBBumper == NULL ) {
    mCBBumper = new CCBBumper ( &mCBDriver, "CB:textdisplay" );
    device = mCBBumper;
    return mCBBumper->init();
  }

  // return already existing device
  device = mCBBumper;
  return 1;
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
