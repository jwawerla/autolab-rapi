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
#include "blobfinder.h"
#include "utilities.h"
#include "rapierror.h"
#include <stdio.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
ABlobFinder::ABlobFinder ( std::string devName )
    : ADevice ( devName )
{
  mFov = 0.0;
  mPanAngle = 0.0;
  mMaxRange = 0.0;
  mScanHeight = 0;
  mScanWidth = 0;
  mNumReadings = 0;
  mBlobData = NULL;
}
//-----------------------------------------------------------------------------
ABlobFinder::~ABlobFinder()
{
  if ( mBlobData )
    delete[] mBlobData;
}
//-----------------------------------------------------------------------------
double ABlobFinder::getPanAngle() const
{
  return mPanAngle;
}
//-----------------------------------------------------------------------------
double ABlobFinder::getMaxRange() const
{
  return mMaxRange;
}
//-----------------------------------------------------------------------------
double ABlobFinder::getFov() const
{
  return mFov;
}
//-----------------------------------------------------------------------------
unsigned int ABlobFinder::getNumReadings() const
{
  return mNumReadings;
}
//-----------------------------------------------------------------------------
void ABlobFinder::print() const
{
  printf ( "ABlobFinder: readings %d \n", mNumReadings );

  for ( unsigned int i = 0; i < mNumReadings; i++ ) {
    printf ( "  %d range %f color %s\n", i,mBlobData[i].range,
             mBlobData[i].color.toStr().c_str() );
  }
}
//-----------------------------------------------------------------------------
unsigned int ABlobFinder::getScanWidth() const
{
  return mScanWidth;
}
//-----------------------------------------------------------------------------
unsigned int ABlobFinder::getScanHeight() const
{
  return mScanHeight;
}
//-----------------------------------------------------------------------------
}  // namespace
