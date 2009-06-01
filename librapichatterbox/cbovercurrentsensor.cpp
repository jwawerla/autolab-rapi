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
#include "cbovercurrentsensor.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBOverCurrentSensor::CCBOverCurrentSensor ( CCBDriver* driver, std::string devName )
    : ABinarySensorArray ( devName )
{
  assert ( driver );
  mCBDriver = driver;

  mNumSamples = 5;
  mBitData = new bool[mNumSamples];
  mBitPose = new CPose2d[mNumSamples];
}
//-----------------------------------------------------------------------------
CCBOverCurrentSensor::~CCBOverCurrentSensor()
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
void CCBOverCurrentSensor::updateData()
{
  mFgAnyTriggered = false;

  // fill data structures
  mBitData[0] = mCBDriver->leftWheelOverCurrent();
  mBitData[1] = mCBDriver->rightWheelOverCurrent();
  mBitData[2] = mCBDriver->lowSideDriverOverCurrent(0);
  mBitData[3] = mCBDriver->lowSideDriverOverCurrent(1);
  mBitData[4] = mCBDriver->lowSideDriverOverCurrent(2);

  // any triggers ?
  for (unsigned int i = 0; i < mNumSamples; i++) {
    if (mBitData[i] )
     mFgAnyTriggered = true;
  }
}
//-----------------------------------------------------------------------------
void CCBOverCurrentSensor::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBOverCurrentSensor::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace
