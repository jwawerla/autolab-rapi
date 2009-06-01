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
#include <assert.h>

namespace Rapi {

//-----------------------------------------------------------------------------
CCBWallSensor::CCBWallSensor(CCBDriver* driver, std::string devName)
 : ARangeFinder(devName)
{
  assert(driver);
  mCBDriver = driver;
  mNumSamples = 1;

  mRangeData = new tRangeData[mNumSamples];
  mRelativeBeamPose = new CPose2d[mNumSamples];

  // TODO: get a realistic value
  mFov = D2R( 20.0 );
  mRelativeBeamPose[0] = CPose2d(0.2, 0.2, D2R( -45.0) );
  mMinRange = 0.0;
  mMaxRange = 4.0;

  setEnabled ( true );
}
//-----------------------------------------------------------------------------
CCBWallSensor::~CCBWallSensor()
{
  if (mRelativeBeamPose) {
    delete[] mRelativeBeamPose;
    mRelativeBeamPose = NULL;
  }
  if (mRangeData) {
    delete[] mRangeData;
    mRangeData = NULL;
  }
}
//-----------------------------------------------------------------------------
void CCBWallSensor::updateData()
{
  mRangeData[0].range = mCBDriver->mCreateSensorPackage.wallSignal;
}
//-----------------------------------------------------------------------------
void CCBWallSensor::setEnabled(bool enable)
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBWallSensor::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------

} // namespace
