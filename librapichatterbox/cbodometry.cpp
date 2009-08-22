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
#include "cbodometry.h"
#include "utilities.h"

namespace Rapi {

//-----------------------------------------------------------------------------
CCBOdometry::CCBOdometry(CCBDriver* driver, std::string devName)
 : COdometry( devName )
{
  mCBDriver = driver;
}
//-----------------------------------------------------------------------------
CCBOdometry::~CCBOdometry()
{
}
//-----------------------------------------------------------------------------
void CCBOdometry::updateData()
{
  float angle;
  float distance;

  angle = D2R ( mCBDriver->mCreateSensorPackage.angle );
  distance = ( float ) ((float) mCBDriver->mCreateSensorPackage.distance) / 1e3;

  mPose.mYaw = normalizeAngle ( mPose.mYaw + angle );
  mPose.mX += distance * cos ( mPose.mYaw );
  mPose.mY += distance * sin ( mPose.mYaw );

  // update time stamp of this measurement
  mTimeStamp = timeStamp();
}
//-----------------------------------------------------------------------------

} // namespace
