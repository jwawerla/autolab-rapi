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
#include "cbwallsensor.h"
#include "utilities.h"
#include "cbfrontfiducialfinder.h"
#include <assert.h>
#include <math.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBFrontFiducialFinder::CCBFrontFiducialFinder( CCBDriver* driver, std::string devName )
    : AFiducialFinder( devName )
{
  assert( driver );
  mCBDriver = driver;
  mNumReadings = 1;
  mOwnFiducialId = 0;

  mFiducialData = new tFiducialData[1];
  mFiducialData->range = -1;
  mFiducialData->bearing = 0;
  mFiducialData->id = 255;

  mMinRange = 0.0;
  mMaxRange = 4.0;
  mFov = 2.0 * M_PI;

  setEnabled( true );
}
//-----------------------------------------------------------------------------
CCBFrontFiducialFinder::~CCBFrontFiducialFinder()
{
  mCBDriver->sendIr(255);

  if ( mFiducialData )
    delete[] mFiducialData;
  mFiducialData = NULL;
}
//-----------------------------------------------------------------------------
void CCBFrontFiducialFinder::updateData()
{
  mFiducialData->id = mCBDriver->mCreateSensorPackage.ir;
  if (mCBDriver->mCreateSensorPackage.virtualWall )
    mFiducialData->id |= 0x0100;
  mCBDriver->sendIr( mOwnFiducialId );
}
//-----------------------------------------------------------------------------
void CCBFrontFiducialFinder::setEnabled( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBFrontFiducialFinder::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBFrontFiducialFinder::setFiducialSignal( int id )
{
  if ( ( id < 0) || (id > 255 ) ) {
    fprintf( stderr, "Invalid fiducial ID %d; must be in [0..255].\n", id );
    return;
  }

  mOwnFiducialId = id;
}
//-----------------------------------------------------------------------------
void CCBFrontFiducialFinder::print( void ) const
{
  printf( "CBFiducialFinder: reading %u, I am %u.  ",
          mFiducialData->id, mOwnFiducialId );

  /* Remote control bytes */
  switch ( mFiducialData->id ) {
    case CB_FIDUCIAL_LEFT: printf( "LEFT" ); break;
    case CB_FIDUCIAL_FORWARD: printf( "FORWARD" ); break;
    case CB_FIDUCIAL_RIGHT: printf( "RIGHT" ); break;
    case CB_FIDUCIAL_SPOT: printf( "SPOT" ); break;
    case CB_FIDUCIAL_MAX: printf( "MAX" ); break;
    case CB_FIDUCIAL_SMALL: printf( "SMALL" ); break;
    case CB_FIDUCIAL_MEDIUM: printf( "MEDIUM" ); break;
    case CB_FIDUCIAL_LARGE_CLEAN: printf( "LARGE_CLEAN" ); break;
    case CB_FIDUCIAL_PAUSE: printf( "PAUSE" ); break;
    case CB_FIDUCIAL_POWER: printf( "POWER" ); break;
    case CB_FIDUCIAL_ARC_FORWARD_LEFT: printf( "ARC_FORWARD_LEFT" ); break;
    case CB_FIDUCIAL_ARC_FORWARD_RIGHT: printf( "ARC_FORWARD_RIGHT" ); break;
    case CB_FIDUCIAL_DRIVE_STOP: printf( "DRIVE_STOP" ); break;

      /* Scheduling remote bytes */
    case CB_FIDUCIAL_SEND_ALL: printf( "SEND_ALL" ); break;
    case CB_FIDUCIAL_SEEK_DOCK: printf( "SEEK_DOCK" ); break;

      /* Home base bytes */
    case CB_FIDUCIAL_RESERVED: printf( "RESERVED" ); break;
    case CB_FIDUCIAL_RED_BUOY: printf( "RED_BUOY" ); break;
    case CB_FIDUCIAL_GREEN_BUOY: printf( "GREEN_BUOY" ); break;
    case CB_FIDUCIAL_FORCE_FIELD: printf( "FORCE_FIELD" ); break;
    case CB_FIDUCIAL_RED_AND_GREEN_BUOYS: printf( "RED_AND_GREEN_BUOYS" ); break;
    case CB_FIDUCIAL_RED_AND_FORCE_FIELD: printf( "RED_AND_FORCE_FIELD" ); break;
    case CB_FIDUCIAL_GREEN_AND_FORCE_FIELD: printf( "GREEN_AND_FORCE_FIELD" ); break;
    case CB_FIDUCIAL_RED_GREEN_AND_FORCE_FIELD: printf( "RED_GREEN_AND_FORCE_FIELD" ); break;

      /* something else */
    default: printf( "(unknown)\n" ); break;
  }

  putchar( '\n' );
}
//-----------------------------------------------------------------------------

} // namespace
