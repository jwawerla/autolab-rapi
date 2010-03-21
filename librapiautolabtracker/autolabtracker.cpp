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
#include "autolabtracker.h"
#include "printerror.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CAutolabTracker::CAutolabTracker(std::string devName, std::string robotName,
  std::string hostName, int port ) : ALocalizer2d(devName)
{
  mPose.setName("tracker");
  mRobotName = robotName;
  mRedisClient = CRedisClient::getInstance(hostName, port);
  assert( mRedisClient );
  mFgEnabled = true;
  mCameraId = -1;
  mFgNewData = false;
}
//-----------------------------------------------------------------------------
CAutolabTracker::~CAutolabTracker()
{
}
//----------------------------------------------------------------------------
void CAutolabTracker::setEnabled( bool enable )
{
  mFgEnabled = enable;
}
//----------------------------------------------------------------------------
int CAutolabTracker::init()
{
  return 1;
}
//----------------------------------------------------------------------------
void CAutolabTracker::updateData( const double dt )
{
  static double lastTimeStamp;
  double ts, x, y, a;
  std::string value;

  if (mFgEnabled) {
    lastTimeStamp = mTimeStamp;
    if (mRedisClient->get( mRobotName+".pose2d" , value ) == 0) {
      PRT_WARN0("Failed to read position information from tracker");
      return;
    }
    //printf("REDIS %s\n", value.c_str());
    if (sscanf(value.c_str(), "x:%lf y:%lf a:%lf t:%lf c:%d\n",
      &x, &y, &a, &ts, &mCameraId )
       != 5) {
      PRT_WARN0("Failed to parse data from Tracker");
    }
    if (mCameraId >= 0) {
      mPose.mX = x;
      mPose.mY = y;
      mPose.mYaw = a;
      mTimeStamp = ts;
      if (mTimeStamp != lastTimeStamp)
        mFgNewData = true;
      else
        mFgNewData = false;
      }
      else {
        mFgNewData = false;
    }
  }
}
//-----------------------------------------------------------------------------
bool CAutolabTracker::isNew()
{
  return mFgNewData;
}
//-----------------------------------------------------------------------------
bool CAutolabTracker::isValid()
{
  if (mCameraId < 0)
    return false; // data not valid

  return true; // data valid
}
//-----------------------------------------------------------------------------
void CAutolabTracker::print() const
{
  printf("AutolabTracker: camera %d, time %f (%f %f %f)\n",
    mCameraId, mTimeStamp, mPose.mX, mPose.mY, R2D(mPose.mYaw) );
}
//-----------------------------------------------------------------------------
void CAutolabTracker::startLogging(std::string filename)
{
  mDataLogger = CDataLogger::getInstance(filename);
  mDataLogger->addVar( &mPose, "tracker pose");
  mDataLogger->addVar( &mCameraId, "cam id");
  mDataLogger->addVar( &mTimeStamp, "timestamp");
}
//-----------------------------------------------------------------------------
} // namespace
