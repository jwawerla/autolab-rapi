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

#include "stagelaser.h"
#include "utilities.h"
#include "rapierror.h"
#include "pose2d.h"

namespace Rapi
{

// Callback for stage, stage calls this function if the corresponding model
// is updated
int laserUpdate ( Stg::ModelLaser* mod, CStageLaser* laser )
{
  laser->updateData();
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageLaser::CStageLaser ( Stg::ModelLaser* stgLaser, std::string devName )
    : ARangeFinder ( devName )
{
  assert ( stgLaser );
  mStgLaser = stgLaser;
  mFgEnabled = false;
  mStgLaser->AddUpdateCallback ( ( Stg::stg_model_callback_t )
                                 laserUpdate,
                                 this );
  setEnabled ( true );
}
//-----------------------------------------------------------------------------
CStageLaser::~CStageLaser()
{
  if ( mRelativeBeamPose )
    delete[] mRelativeBeamPose;

  if ( mRangeData )
    delete[] mRangeData;
}
//-----------------------------------------------------------------------------
void CStageLaser::setEnabled ( bool enable )
{
  if ( mFgEnabled == enable )
    return;  // we are really have the correct status

  if ( enable )
    mStgLaser->Subscribe();
  else
    mStgLaser->Unsubscribe();

  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CStageLaser::init()
{
  float beamBearing;
  float beamIncrement;
  Stg::ModelLaser::Config cfg;

  cfg = mStgLaser->GetConfig();
  mMaxRange = cfg.range_bounds.max;
  mMinRange = cfg.range_bounds.min;
  mNumSamples = cfg.sample_count;
  mRangeResolution = cfg.resolution;
  mFov = cfg.fov;

  // initialize data structures
  mRelativeBeamPose = new CPose2d[mNumSamples];
  mRangeData = new tRangeData[mNumSamples];

  mBeamConeAngle = 0; // set to zero to indicate it is a laser

  beamBearing = -mFov / 2.0f;
  beamIncrement = mFov / float ( mNumSamples - 1 );

  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    mRelativeBeamPose[i].mX = 0.0;
    mRelativeBeamPose[i].mY = 0.0;
    mRelativeBeamPose[i].mYaw =  beamBearing;
    beamBearing += beamIncrement;

    mRangeData[i].range = 0.0;
    mRangeData[i].reflectance = 0.0;
  }

  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageLaser::updateData()
{
  tRangeData* temp;
  uint32_t sampleCount;

  if ( mFgEnabled ) {
    // get range data from stage, if the simulation is paused at start up
    // stage returns NULL, so we store the result in a temporal variable
    // and store is only if it is any good
    temp = ( tRangeData* ) mStgLaser->GetSamples ( &sampleCount );
    if ( temp != NULL ) {
      mRangeData = temp;
    }

    mTimeStamp = mStgLaser->GetWorld()->SimTimeNow() / 1e6;
    notifyDataUpdateObservers();

    if ( sampleCount != mNumSamples )
      ERROR2 ( "Got wrong number of laser readings from Stage, "\
               "expected %d but got %d", mNumSamples, sampleCount );
  }
}
//-----------------------------------------------------------------------------

} // namespace
