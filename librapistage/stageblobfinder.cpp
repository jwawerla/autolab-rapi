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
#include "utilities.h"
#include "stageblobfinder.h"

namespace Rapi
{

// Callback for stage, stage calls this function if the corresponding model
// is updated
  int blobUpdate ( Stg::ModelBlobfinder* mod, CStageBlobFinder* blobfinder )
  {
    blobfinder->updateData ( mod->GetUpdateInterval() / 1e6 );
    return 0; // ok
  }

//-----------------------------------------------------------------------------
  CStageBlobFinder::CStageBlobFinder ( Stg::ModelBlobfinder* stgModel,
                                       std::string devName )
      : ABlobFinder ( devName )
  {
    assert ( stgModel );
    mStgBlobFinder = stgModel;
    mFgEnabled = false;

    mStgBlobFinder->GetWorld()->AddUpdateCallback ( ( Stg::stg_world_callback_t )
                                        blobUpdate,
                                        this );
    setEnabled ( true );
  }
//-----------------------------------------------------------------------------
  CStageBlobFinder::~CStageBlobFinder()
  {
  }
//-----------------------------------------------------------------------------
  int CStageBlobFinder::init()
  {
    mFov = mStgBlobFinder->fov;
    mPanAngle = mStgBlobFinder->pan;
    mMaxRange = mStgBlobFinder->range;
    mScanHeight = mStgBlobFinder->scan_height;
    mScanWidth = mStgBlobFinder->scan_width;

    return 1; // success
  }
//-----------------------------------------------------------------------------
  void CStageBlobFinder::setEnabled ( bool enable )
  {
    if ( mFgEnabled == enable )
      return;  // we are really have the correct status

    if ( enable )
      mStgBlobFinder->Subscribe();
    else
      mStgBlobFinder->Unsubscribe();

    mFgEnabled = enable;
  }
//-----------------------------------------------------------------------------
  void CStageBlobFinder::updateData ( const double dt )
  {
    Stg::ModelBlobfinder::Blob* blobs;

    if ( mFgEnabled )
    {
      // clear old data
      if ( mBlobData )
      {
        delete[] mBlobData;
        mBlobData = NULL;
      }


      blobs = mStgBlobFinder->GetBlobs ( &mNumReadings );




      // do we have blob data ?
      if ( mNumReadings == 0 )
      {
        return; // no data
      }
      // allocate memory for data
      mBlobData = new tBlobData[mNumReadings];
      // copy data
      for ( unsigned int i = 0; i < mNumReadings; i++ )
      {
        mBlobData[i].left = blobs[i].left;
        mBlobData[i].right = blobs[i].right;
        mBlobData[i].top = blobs[i].top;
        mBlobData[i].bottom = blobs[i].bottom;
        mBlobData[i].range = blobs[i].range;
        mBlobData[i].color = CRgbColor ( blobs[i].color.r * 255,
                                         blobs[i].color.g * 255,
                                         blobs[i].color.b * 255);
      }
      mTimeStamp = mStgBlobFinder->GetWorld()->SimTimeNow() / 1e6;
      notifyDataUpdateObservers();
    }
  }
//-----------------------------------------------------------------------------

} // namespace
