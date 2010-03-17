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

#include "robot.h"
#include "printerror.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
ARobot::ARobot()
{
  mName = "ARobot";
  mUpdateInterval = 0.1; // 10 Hz
  mLastLoopDuration = 0.0;
  mRobotCtrl = NULL;
  mFgInitialized = false;
  mFgRunningSlowly = false;
}
//-----------------------------------------------------------------------------
ARobot::~ARobot()
{
}
//-----------------------------------------------------------------------------
std::string ARobot::getName() const
{
  return mName;
}
//-----------------------------------------------------------------------------
double ARobot::getDurationLastLoop() const
{
  return mLastLoopDuration;
}
//-----------------------------------------------------------------------------
void ARobot::updateDevices()
{
  // update additional devices
  for (unsigned int i = 0; i < mDeviceList.size(); i++)
    mDeviceList[i]->updateData( mLastLoopDuration );
}
//-----------------------------------------------------------------------------
ADevice* ARobot::getDeviceByIndex(unsigned int index) const
{
  if ( index >= mDeviceList.size() )
    return NULL;

  return mDeviceList[index];
}
//-----------------------------------------------------------------------------
void ARobot::addDevice(ADevice* dev)
{
  mDeviceList.push_back(dev);
}
//-----------------------------------------------------------------------------
void ARobot::registerRobotController(IRobotUpdate* ctrl)
{
  if (mRobotCtrl)
    PRT_WARN0("Already have a controller");

  mRobotCtrl = ctrl;
}
//-----------------------------------------------------------------------------
void ARobot::registerStateVariable( IRobotUpdate* var)
{
  mStateVariableList.push_back( var );
}
//-----------------------------------------------------------------------------
void ARobot::updateControllers()
{
  if (mRobotCtrl) {
    mRobotCtrl->updateData( mUpdateInterval );
  }
}
//-----------------------------------------------------------------------------
void ARobot::updateStateVariable()
{
  IRobotUpdate* var;
  std::list<IRobotUpdate*>::iterator it;

  for (it = mStateVariableList.begin(); it != mStateVariableList.end(); it++) {
    var = (IRobotUpdate*)*it;
    assert(var);
    var->updateData( mUpdateInterval );
  }
}
//-----------------------------------------------------------------------------
void ARobot::setUpdateInterval ( double interval )
{
  mUpdateInterval = interval;
}
//-----------------------------------------------------------------------------
double ARobot::getUpdateInterval() const
{
  return mUpdateInterval;
}
//-----------------------------------------------------------------------------
bool ARobot::isSlow()
{
  if( mFgRunningSlowly ) {
    mFgRunningSlowly = false;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
} // namespace

