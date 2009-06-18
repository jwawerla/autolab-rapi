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
#include "cbtopfiducialfinder.h"
#include <assert.h>
#include <math.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBTopFiducialFinder::CCBTopFiducialFinder( CCBDriver* driver, std::string devName )
    : AFiducialFinder( devName )
{
  assert( driver );
  mCBDriver = driver;
  mNumReadings = 1;
  mOwnFiducialId = 0;

  mFiducialData = new tFiducialData();
  mFiducialData->range = -1;
  mFiducialData->bearing = 0;
  mFiducialData->id = 255;

  mMinRange = 0.0;
  mMaxRange = 4.0;
  mFov = 2.0 * M_PI;

  setEnabled( true );
}
//-----------------------------------------------------------------------------
CCBTopFiducialFinder::~CCBTopFiducialFinder()
{
  mCBDriver->setIrLed(0);
  if ( mFiducialData )
    delete mFiducialData;
  mFiducialData = NULL;
}
//-----------------------------------------------------------------------------
void CCBTopFiducialFinder::updateData()
{
  mFiducialData->id = 0;
  mCBDriver->setIrLed( mOwnFiducialId );
}
//-----------------------------------------------------------------------------
void CCBTopFiducialFinder::setEnabled( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBTopFiducialFinder::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBTopFiducialFinder::setFiducialSignal( int id )
{
  if ( ( id < 0 ) ||  (id > 255 ) ) {
    fprintf( stderr, "Invalid fiducial ID %d; must be in [0..255].\n", id );
    return;
  }

  mOwnFiducialId = id;
}
//-----------------------------------------------------------------------------
void CCBTopFiducialFinder::print( void ) const
{
  printf( "CBFiducialFinder: reading %u, I am %u. \n ",
          mFiducialData->id, mOwnFiducialId );
}
//-----------------------------------------------------------------------------

} // namespace
