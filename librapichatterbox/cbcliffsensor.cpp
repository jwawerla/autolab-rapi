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
#include "cbcliffsensor.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBCliffSensor::CCBCliffSensor ( CCBDriver* driver, std::string devName )
    : ABinarySensorArray ( devName )
{
  assert ( driver );
  mCBDriver = driver;

  mNumSamples = 4;
  mBitData = new bool[mNumSamples];
  mBitPose = new CPose2d[mNumSamples];
}
//-----------------------------------------------------------------------------
CCBCliffSensor::~CCBCliffSensor()
{
  if ( mBitData ) {
    delete[] mBitData;
    mBitData = NULL;
  }
  if ( mBitPose ) {
    delete[] mBitPose;
    mBitPose = NULL;
  }
}
//-----------------------------------------------------------------------------
void CCBCliffSensor::updateData()
{
  mFgAnyTriggered = false;

  // fill data structures
  mBitData[0] = mCBDriver->getCliffSensor ( CB_LEFT_CLIFF );
  mBitData[1] = mCBDriver->getCliffSensor ( CB_FRONT_LEFT_CLIFF );
  mBitData[2] = mCBDriver->getCliffSensor ( CB_FRONT_RIGHT_CLIFF );
  mBitData[3] = mCBDriver->getCliffSensor ( CB_RIGHT_CLIFF );

  // any triggers ?
  for ( unsigned int i =0; i < mNumSamples; i++ ) {
    if ( mBitData[0] )
      mFgAnyTriggered = true;
  }
}
//-----------------------------------------------------------------------------
void CCBCliffSensor::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBCliffSensor::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace
