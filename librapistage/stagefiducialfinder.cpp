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
#include "stagefiducialfinder.h"

namespace Rapi
{

// Callback for stage, stage calls this function if the corresponding model
// is updated
int fiducialUpdate ( Stg::ModelFiducial* mod, CStageFiducialFinder* fiducal )
{
  fiducal->updateData();
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageFiducialFinder::CStageFiducialFinder ( Stg::ModelFiducial* stgModel,
    std::string devName )
    : AFiducialFinder ( devName )
{
  assert( stgModel );
  mStgFiducial = stgModel;
  mFgEnabled = false;

  mStgFiducial->AddUpdateCallback ( ( Stg::stg_model_callback_t )
                                    fiducialUpdate,
                                    this );
  setEnabled( true );
}
//-----------------------------------------------------------------------------
CStageFiducialFinder::~CStageFiducialFinder()
{
}
//-----------------------------------------------------------------------------
int CStageFiducialFinder::init()
{
  mFov = mStgFiducial->fov;
  mMinRange = mStgFiducial->min_range;
  mMaxRange = MAX ( mStgFiducial->max_range_anon, mStgFiducial->max_range_id );

  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageFiducialFinder::setEnabled ( bool enable )
{
  if ( mFgEnabled == enable )
    return;  // we are really have the correct status

  if ( enable )
    mStgFiducial->Subscribe();
  else
    mStgFiducial->Unsubscribe();

  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void CStageFiducialFinder::setFiducialSignal(int id)
{
  mStgFiducial->SetFiducialReturn(id);
}
//-----------------------------------------------------------------------------
void CStageFiducialFinder::updateData()
{

  if ( mFgEnabled ) {
    // clear old data
    if ( mFiducialData ) {
      delete[] mFiducialData;
      mFiducialData = NULL;
    }

    mOwnFiducialId = mStgFiducial->GetFiducialReturn();

	 std::vector<Stg::ModelFiducial::Fiducial>& fids = mStgFiducial->GetFiducials();
	 
    mNumReadings = fids.size();

    // do we have fiducial data ?
    if ( mNumReadings == 0 ) {
      return; // no data
    }
    // allocate memory for data
    mFiducialData = new tFiducialData[mNumReadings];
    // copy data
    for ( unsigned int i = 0; i < mNumReadings; i++ ) {
      mFiducialData[i].id = fids[i].id;
      mFiducialData[i].range = fids[i].range;
      mFiducialData[i].bearing = fids[i].bearing;
    }
    mTimeStamp = mStgFiducial->GetWorld()->SimTimeNow() / 1e6;
    notifyDataUpdateObservers();
  }
}
//-----------------------------------------------------------------------------

} // namespace
