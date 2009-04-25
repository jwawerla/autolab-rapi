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

namespace Rapi
{

//-----------------------------------------------------------------------------
ARobot::ARobot()
{
}
//-----------------------------------------------------------------------------
ARobot::~ARobot()
{
}
//-----------------------------------------------------------------------------
void ARobot::registerRobotController(ARobotCtrl* ctrl)
{
  mRobotCtrlList.push_back( ctrl );
}
//-----------------------------------------------------------------------------
void ARobot::updateControllers()
{
  ARobotCtrl* ctrl;
  std::list<ARobotCtrl*>::iterator it;

  for (it = mRobotCtrlList.begin(); it != mRobotCtrlList.end(); it++) {
    ctrl = (ARobotCtrl*)*it;
    ctrl->update( mUpdateInterval );
  }
}
//-----------------------------------------------------------------------------

} // namespace

