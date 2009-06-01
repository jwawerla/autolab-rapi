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
#include "cbbumper.h"
#include "utilities.h"

namespace Rapi {

//-----------------------------------------------------------------------------
CCBBumper::CCBBumper(CCBDriver* cbDriver, std::string devName)
 : ABinarySensorArray(devName)
{
  mCBDriver = cbDriver;

  mNumSamples = 2;
  mBitData = new bool[mNumSamples];
  mBitPose = new CPose2d[mNumSamples];

  mFgEnabled = false;

  setEnabled(true);
}
//-----------------------------------------------------------------------------
CCBBumper::~CCBBumper()
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
void CCBBumper::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBBumper::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBBumper::updateData()
{
  mFgAnyTriggered = false;

  if ( mFgEnabled == true ) {
     mBitData[0] = mCBDriver->rightBumper();
     if (mBitData[0])
       mFgAnyTriggered = true;

     mBitData[1] = mCBDriver->leftBumper();
     if (mBitData[1])
       mFgAnyTriggered = true;

    // update time stamp of this measurement
    mTimeStamp = timeStamp();
  }
}
//-----------------------------------------------------------------------------

} // namespace
