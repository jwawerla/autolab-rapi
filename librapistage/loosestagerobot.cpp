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

#include "loosestagerobot.h"
#include "rapierror.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CLooseStageRobot::CLooseStageRobot( Stg::Model* mod )
    : CStageRobot( mod )
{
}
//-----------------------------------------------------------------------------
CLooseStageRobot::~CLooseStageRobot()
{
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageLaser* &device,
                                  std::string devName )
{
  Stg::ModelLaser* modLaser;

  // check if we already created such a device
  device = ( CLooseStageLaser* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  //modLaser = ( Stg::ModelLaser* ) mStageModel->GetModel( devName.c_str() );
  modLaser = ( Stg::ModelLaser* ) mStageModel->GetChild( devName.c_str() );
  if ( modLaser == NULL ) {
    ERROR2( "Stage model %s has no device named %s",
            mStageModel->Token(), devName.c_str() );
    return 0; // error
  }

  device = new CLooseStageLaser( modLaser, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modLaser->GetUpdateInterval() );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageSonar* &device,
                                  std::string devName )
{
  Stg::ModelRanger* modRanger;

  // check if we already created such a device
  device = ( CLooseStageSonar* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  //modRanger = ( Stg::ModelRanger* ) mStageModel->GetModel( devName.c_str() );
  modRanger = ( Stg::ModelRanger* ) mStageModel->GetChild( devName.c_str() );
  if ( modRanger == NULL )  {
    ERROR2( "Stage model %s has no device named %s",
            mStageModel->Token(), devName.c_str() );
    return 0; // error
  }
  device = new CLooseStageSonar( modRanger, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modRanger->GetUpdateInterval() );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageDrivetrain2dof* &device,
                                  std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStageDrivetrain2dof* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;
  device = new CLooseStageDrivetrain2dof( modPosition, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back(( ADevice* ) device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modPosition->GetUpdateInterval() );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStagePowerPack* &device,
                                  std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStagePowerPack* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;
  device = new CLooseStagePowerPack( modPosition, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back(( ADevice* ) device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modPosition->GetUpdateInterval() );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageFiducialFinder* &device,
                                  std::string devName )
{
  Stg::ModelFiducial* modFiducial;

  // check if we already created such a device
  device = ( CLooseStageFiducialFinder* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  //modFiducial = ( Stg::ModelFiducial* ) mStageModel->GetModel( devName.c_str() );
  modFiducial = ( Stg::ModelFiducial* ) mStageModel->GetChild( devName.c_str() );
  if ( modFiducial == NULL ) {
    ERROR2( "Stage model %s has no device named %s",
            mStageModel->Token(), devName.c_str() );
    return 0; // error
  }
  device = new CLooseStageFiducialFinder( modFiducial, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back(( ADevice* ) device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modFiducial->GetUpdateInterval() );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageLights* &device,
                                  std::string devName )
{
  device = NULL;
  ERROR0( "Device not implementated for stage" );
  return 0; // failure
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice( CLooseStageTextDisplay* &device,
                                  std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStageTextDisplay* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;
  device =  new CLooseStageTextDisplay( modPosition, devName );
  assert( device );
  // add device to the list
  mDeviceList.push_back(( ADevice* ) device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // failure
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modPosition->GetUpdateInterval() );
  return 1;  // success
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageBlobFinder* &device, std::string devName )
{
  Stg::ModelBlobfinder* modBlobFinder;

  // check if we already created such a device
  device = ( CLooseStageBlobFinder* ) findDeviceByName( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modBlobFinder = ( Stg::ModelBlobfinder* ) mStageModel->GetChild( devName.c_str() );

  device = new CLooseStageBlobFinder( modBlobFinder, devName );

  // add device to the list
  mDeviceList.push_back( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1( "Failed to initialize device %s", devName.c_str() );
    return 0; // error
  }
  device->setEnabled( true );
  // enforce one update step, to fill the data structures with valid data
  device->updateData( modBlobFinder->GetUpdateInterval() / 1e6 );

  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace

