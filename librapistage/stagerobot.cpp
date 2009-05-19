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

#include "stagerobot.h"
#include "stagelaser.h"
#include "stagedrivetrain2dof.h"
#include "stagepowerpack.h"
#include "stagefiducialfinder.h"
#include "stagetextdisplay.h"
#include "stagesonar.h"
#include "rapierror.h"

namespace Rapi
{

// Callback for stage, stage calls this function if the corresponding model
// is updated
int ctrlUpdate ( Stg::World* world, CStageRobot* controller )
{
  controller->updateControllers();
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageRobot::CStageRobot ( Stg::Model* mod )
    : ARobot()
{
  assert ( mod );
  mStageModel = mod;
  mName = mStageModel->Token();
  // update interval [s]
  mUpdateInterval = mStageModel->GetWorld()->GetSimInterval() * 1e-6;

  mStageModel->GetWorld()->AddUpdateCallback ( ( Stg::stg_world_callback_t )
      ctrlUpdate,
      this );
}
//-----------------------------------------------------------------------------
CStageRobot::~CStageRobot()
{
}
//-----------------------------------------------------------------------------
int CStageRobot::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
double CStageRobot::getCurrentTime() const
{
  return mStageModel->GetWorld()->SimTimeNow() * 1e-6;
}
//-----------------------------------------------------------------------------
ADevice* CStageRobot::findDeviceByName ( std::string devName )
{
  ADevice* device;
  std::vector<ADevice*>::iterator it;

  for ( it = mDeviceList.begin(); it != mDeviceList.end(); it++ ) {
    device = *it;
    if ( device->getName() == devName )
      return device;
  }

  return NULL;
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ARangeFinder* &device, std::string devName )
{
  CStageLaser* laser = NULL;
  CStageSonar* sonar = NULL;
  Stg::ModelLaser* modLaser;
  Stg::ModelRanger* modRanger;

  // check if we already created such a device
  device = ( ARangeFinder* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  //***************************
  // Handle Stage's Laser Models
  if ( devName.find ( LASER_MODEL_NAME ) != std::string::npos ) {
    // no device created yet, so do it now
    modLaser = ( Stg::ModelLaser* ) mStageModel->GetModel ( devName.c_str() );
    if ( modLaser == NULL ) {
      ERROR2 ( "Stage model %s has no device named %s",
               mStageModel->Token(), devName.c_str() );
      return 0; // error
    }
    laser = new CStageLaser ( modLaser, devName );
    device = laser;
    // add device to the list
    mDeviceList.push_back ( device );

    // now initialize the device
    if ( device->init() != 1 ) {
      ERROR1 ( "Failed to initialize device %s", devName.c_str() );
      return 0; // error
    }
    device->setEnabled ( true );
    // enforce one update step, to fill the data structures with valid data
    laser->updateData();
    return 1; // success
  }

  //***************************
  // Handle Stage's Ranger Models
  if ( devName.find ( RANGER_MODEL_NAME ) != std::string::npos ) {
    // no device created yet, so do it now
    modRanger = ( Stg::ModelRanger* ) mStageModel->GetModel ( devName.c_str() );
    if ( modRanger == NULL ) {
      ERROR2 ( "Stage model %s has no device named %s",
               mStageModel->Token(), devName.c_str() );
      return 0; // error
    }
    sonar = new CStageSonar ( modRanger, devName );
    device = sonar;

    // add device to the list
    mDeviceList.push_back ( device );
    // now initialize the device
    if ( device->init() != 1 ) {
      ERROR1 ( "Failed to initialize device %s", devName.c_str() );
      return 0; // error
    }
    device->setEnabled ( true );
    // enforce one update step, to fill the data structures with valid data
    sonar->updateData();
    return 1; // success
  }

  ERROR2 ( "Stage model %s has no device named %s",
           mStageModel->Token(), devName.c_str() );
  return 0; // error
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ADrivetrain2dof* &device, std::string devName )
{
  CStageDrivetrain2dof* drivetrain2of;
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( ADrivetrain2dof* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  drivetrain2of = new CStageDrivetrain2dof ( modPosition, devName );
  device = ( ADrivetrain2dof* ) drivetrain2of;
  // add device to the list
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0; // error
  }
  device->setEnabled ( true );
  // enforce one update step, to fill the data structures with valid data
  drivetrain2of->updateData();
  return 1; // success
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( APowerPack* &device, std::string devName )
{
  CStagePowerPack* powerPack;
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( APowerPack* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;
  powerPack = new CStagePowerPack ( modPosition, devName );
  device = ( APowerPack* ) powerPack;
  // add device to the list
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0; // error
  }
  device->setEnabled ( true );
  // enforce one update step, to fill the data structures with valid data
  powerPack->updateData();

  return 1; // success
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( AFiducialFinder* &device, std::string devName )
{
  CStageFiducialFinder* fiducialFinder;
  Stg::ModelFiducial* modFiducial;

  // check if we already created such a device
  device = ( AFiducialFinder* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modFiducial = ( Stg::ModelFiducial* ) mStageModel->GetModel ( devName.c_str() );
  if ( modFiducial == NULL ) {
    ERROR2 ( "Stage model %s has no device named %s",
             mStageModel->Token(), devName.c_str() );
    return 0; // error
  }

  fiducialFinder = new CStageFiducialFinder ( modFiducial, devName );
  device = fiducialFinder;
  // add device to the list
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0; // error
  }
  device->setEnabled ( true );
  // enforce one update step, to fill the data structures with valid data
  fiducialFinder->updateData();

  return 1; // success
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ALights* &device, std::string devName )
{
  device = NULL;
  ERROR0 ( "Device not implementated for stage" );
  return 0;
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ABinarySensorArray* &device, std::string devName )
{
  device = NULL;
  ERROR0 ( "Device not implementated for stage" );
  return 0;
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ATextDisplay* &device, std::string devName )
{
  CStageTextDisplay* textDisplay;
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( ATextDisplay* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  textDisplay = new CStageTextDisplay ( modPosition, devName );
  device = ( ATextDisplay* ) textDisplay;

  // add device to the list
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0; // error
  }
  device->setEnabled ( true );
  // enforce one update step, to fill the data structures with valid data
  textDisplay->updateData();

  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace

