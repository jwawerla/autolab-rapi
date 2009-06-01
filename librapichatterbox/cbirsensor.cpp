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

#include "cbirsensor.h"
#include "utilities.h"
#include "rapierror.h"

namespace Rapi
{

//---------------------------------------------------------------------------
CCBIrSensor::CCBIrSensor ( CCBDriver* cbDriver, std::string devName )
    : ARangeFinder ( devName )
{
  mCBDriver = cbDriver;
  mNumSamples = 6;
  mMaxRange = 1.5;
  mMinRange = 0.0;
  mBeamConeAngle = D2R ( 20.0 );

  mRangeData = new tRangeData[mNumSamples];
  mRelativeBeamPose = new CPose2d[mNumSamples];

  mRelativeBeamPose[0].mX = 0.0;
  mRelativeBeamPose[0].mY = 0.0;
  mRelativeBeamPose[0].mYaw = D2R ( 0.0 );
  mRangeData[0].range = 0.0;

  mRelativeBeamPose[1].mX = 0.0;
  mRelativeBeamPose[1].mY = 0.0;
  mRelativeBeamPose[1].mYaw = D2R ( 30.0 );
  mRangeData[1].range = 0.0;

  mRelativeBeamPose[2].mX = 0.0;
  mRelativeBeamPose[2].mY = 0.0;
  mRelativeBeamPose[2].mYaw = D2R ( 90.0 );
  mRangeData[2].range = 0.0;

  mRelativeBeamPose[3].mX = 0.0;
  mRelativeBeamPose[3].mY = 0.0;
  mRelativeBeamPose[3].mYaw = D2R ( 180.0 );
  mRangeData[3].range = 0.0;

  mRelativeBeamPose[4].mX = 0.0;
  mRelativeBeamPose[4].mY = 0.0;
  mRelativeBeamPose[4].mYaw = D2R ( -90.0 );
  mRangeData[4].range = 0.0;

  mRelativeBeamPose[5].mX = 0.0;
  mRelativeBeamPose[5].mY = 0.0;
  mRelativeBeamPose[5].mYaw = D2R ( -30.0 );
  mRangeData[5].range = 0.0;

  setEnabled ( true );
}
//---------------------------------------------------------------------------
CCBIrSensor::~CCBIrSensor()
{
  // turn off IR sensors
  mCBDriver->enableIr ( false );

  delete[] mRelativeBeamPose;
  delete[] mRangeData;
}
//---------------------------------------------------------------------------
void CCBIrSensor::setEnabled ( bool enable )
{
  if ( enable == true ) {
    if ( mCBDriver->enableIr ( true ) == 0 ) {
      ERROR0 ( "Failed to enable IR sensors" );
      return;
    }
    mFgEnabled = true;
  } else {
    if ( mCBDriver->enableIr ( false ) == 0 ) {
      ERROR0 ( "Failed to disable IR sensors" );
      return;
    }
    mFgEnabled = false;
  }
}
//---------------------------------------------------------------------------
int CCBIrSensor::init()
{
  return 1; // nothing to do right now
}
//---------------------------------------------------------------------------
void CCBIrSensor::updateData()
{
  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    mRangeData[i].range = mCBDriver->readDistance ( i );
    mRangeData[i].reflectance = 0.0;
  }
}
//---------------------------------------------------------------------------
void CCBIrSensor::print() const
{
  printf ( "CCBIrSensor: " );
  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    printf ( " %01.3f ", mRangeData[i].range );
  }
  printf ( "\n" );
}
//---------------------------------------------------------------------------

} // namespace
