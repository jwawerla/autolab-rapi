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
#include "cbphotosensor.h"
#include "assert.h"
namespace Rapi
{

//-----------------------------------------------------------------------------
CCBPhotoSensor::CCBPhotoSensor( CCBDriver* cbDriver, std::string devName )
    : AAnalogSensorArray( devName )
{
  assert( cbDriver );
  mCBDriver = cbDriver;
  mNumSamples = 1;
  mData = new double[1];
  setEnabled( true );
}
//-----------------------------------------------------------------------------
CCBPhotoSensor::~CCBPhotoSensor()
{
  if ( mData ) {
    delete[] mData;
    mData = NULL;
  }
}
//-----------------------------------------------------------------------------
void CCBPhotoSensor::setLpfTau( float tau )
{
	mCBDriver->setLpfAdcTau( tau, 1 );
}
//-----------------------------------------------------------------------------
void CCBPhotoSensor::setEnabled( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBPhotoSensor::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBPhotoSensor::updateData()
{
  if ( mFgEnabled ) {
    mData[0] = mCBDriver->readPhotoSensor();
  }
}
//-----------------------------------------------------------------------------

} // namespace
