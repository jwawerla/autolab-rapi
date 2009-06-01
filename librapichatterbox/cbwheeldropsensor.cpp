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
#include "cbwheeldropsensor.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBWheelDropSensor::CCBWheelDropSensor ( CCBDriver* driver, std::string devName )
    : ABinarySensorArray ( devName )
{
  assert ( driver );
  mCBDriver = driver;

  mNumSamples = 3;
  mBitData = new bool[mNumSamples];
  mBitPose = new CPose2d[mNumSamples];
}
//-----------------------------------------------------------------------------
CCBWheelDropSensor::~CCBWheelDropSensor()
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
void CCBWheelDropSensor::updateData()
{
  mFgAnyTriggered = false;

  // fill data structures
  mBitData[0] = mCBDriver->leftWheelDrop();
  mBitData[1] = mCBDriver->rightWheelDrop();
  mBitData[2] = mCBDriver->casterWheelDrop();

  // any triggers ?
  for (unsigned int i =0; i < mNumSamples; i++) {
    if (mBitData[i] )
     mFgAnyTriggered = true;
  }
}
//-----------------------------------------------------------------------------
void CCBWheelDropSensor::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBWheelDropSensor::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace
