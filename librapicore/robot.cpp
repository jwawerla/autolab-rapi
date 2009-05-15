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
  mUpdateInterval = 0.0;
  mRobotCtrl = NULL;
}
//-----------------------------------------------------------------------------
ARobot::~ARobot()
{
}
//-----------------------------------------------------------------------------
std::string ARobot::getName()
{
  return mName;
}
//-----------------------------------------------------------------------------
ADevice* ARobot::getDeviceByIndex(unsigned int index)
{
  if ( index >= mDeviceList.size() )
    return NULL;

  return mDeviceList[index];
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
} // namespace

