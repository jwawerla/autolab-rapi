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
CLooseStageRobot::CLooseStageRobot ( Stg::Model* mod )
    : CStageRobot ( mod )
{
}
//-----------------------------------------------------------------------------
CLooseStageRobot::~CLooseStageRobot()
{
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
ADevice* CLooseStageRobot::findDeviceByName ( std::string devName )
{
  ADevice* device;
  std::list<ADevice*>::iterator it;

  for ( it = mDeviceList.begin(); it != mDeviceList.end(); it++ ) {
    device = *it;
    if ( device->getName() == devName )
      return device;
  }

  return NULL;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageLaser* &device,
                                   std::string devName )
{
  Stg::ModelLaser* modLaser;

  // check if we already created such a device
  device = ( CLooseStageLaser* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modLaser = ( Stg::ModelLaser* ) mStageModel->GetUnusedModelOfType (
               Stg::MODEL_TYPE_LASER );

  device = new CLooseStageLaser ( modLaser, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageSonar* &device,
                                   std::string devName )
{
  Stg::ModelRanger* modRanger;

  // check if we already created such a device
  device = ( CLooseStageSonar* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modRanger = ( Stg::ModelRanger* ) mStageModel->GetUnusedModelOfType (
                Stg::MODEL_TYPE_LASER );

  device = new CLooseStageSonar ( modRanger, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageDrivetrain2dof* &device,
                                   std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStageDrivetrain2dof* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device = ( CLooseStageDrivetrain2dof* ) new CLooseStageDrivetrain2dof (
             modPosition, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStagePowerPack* &device,
                                   std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStagePowerPack* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device = ( CLooseStagePowerPack* ) new CLooseStagePowerPack (
             modPosition, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageFiducialFinder* &device,
                                   std::string devName )
{
  Stg::ModelFiducial* modFiducial;

  // check if we already created such a device
  device = ( CLooseStageFiducialFinder* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modFiducial = ( Stg::ModelFiducial* ) mStageModel->GetUnusedModelOfType (
                  Stg::MODEL_TYPE_FIDUCIAL );

  device = new CLooseStageFiducialFinder ( modFiducial, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageLights* &device,
                                   std::string devName )
{
  device = NULL;
  ERROR0 ( "Device not implementated for stage" );
  return 0;
}
//-----------------------------------------------------------------------------
int CLooseStageRobot::findDevice ( CLooseStageTextDisplay* &device,
                                   std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( CLooseStageTextDisplay* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device =  new CLooseStageTextDisplay ( modPosition, devName );
  mDeviceList.push_back ( device );

  // now initialize the device
  if ( device->init() != 1 ) {
    ERROR1 ( "Failed to initialize device %s", devName.c_str() );
    return 0;
  }
  device->setEnabled ( true );
  return 1;
}
//-----------------------------------------------------------------------------

} // namespace

