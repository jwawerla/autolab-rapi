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
#include "rapierror.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CStageRobot::CStageRobot ( Stg::Model* mod )
    : ARobot()
{
  mStageModel = mod;
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
ADevice* CStageRobot::findDeviceByName ( std::string devName )
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
int CStageRobot::findDevice ( ARangeFinder* &device, std::string devName )
{
  Stg::ModelLaser* modLaser;

  // check if we already created such a device
  device = ( ARangeFinder* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modLaser = ( Stg::ModelLaser* ) mStageModel->GetUnusedModelOfType ( Stg::MODEL_TYPE_LASER );

  device = new CStageLaser ( modLaser, devName );
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
int CStageRobot::findDevice ( ADrivetrain2dof* &device, std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( ADrivetrain2dof* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device = ( ADrivetrain2dof* ) new CStageDrivetrain2dof ( modPosition, devName );
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
int CStageRobot::findDevice ( APowerPack* &device, std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( APowerPack* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device = ( APowerPack* ) new CStagePowerPack ( modPosition, devName );
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
int CStageRobot::findDevice ( AFiducialFinder* &device, std::string devName )
{
  Stg::ModelFiducial* modFiducial;

  // check if we already created such a device
  device = ( AFiducialFinder* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modFiducial = ( Stg::ModelFiducial* ) mStageModel->GetUnusedModelOfType (
                  Stg::MODEL_TYPE_FIDUCIAL );

  device = new CStageFiducialFinder ( modFiducial, devName );
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
int CStageRobot::findDevice(ALights* &device, std::string devName )
{
  device = NULL;
  ERROR0("Device not implementated for stage");
  return 0;
}
//-----------------------------------------------------------------------------
int CStageRobot::findDevice ( ATextDisplay* &device, std::string devName )
{
  Stg::ModelPosition* modPosition;

  // check if we already created such a device
  device = ( ATextDisplay* ) findDeviceByName ( devName );
  if ( device ) {
    return 1; // success
  }

  // no device created yet, so do it now
  modPosition = ( Stg::ModelPosition* ) mStageModel;

  device = ( ATextDisplay* ) new CStageTextDisplay ( modPosition, devName );
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

