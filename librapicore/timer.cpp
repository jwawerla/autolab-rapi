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

#include "timer.h"
#include <assert.h>
namespace Rapi
{


//-----------------------------------------------------------------------------
CTimer::CTimer(ARobot* robot)
    : IRobotUpdate()
{
  assert(robot);
  robot->registerStateVariable( this );
  mTime = 0.0f;
  mFgExpired = false;
  mFgRunning = false;
}
//-----------------------------------------------------------------------------
CTimer::~CTimer()
{
}
//-----------------------------------------------------------------------------
void CTimer::updateData ( float dt )
{
  if ( mFgRunning ) {
    mTime = mTime - dt;
    if ( mTime <= 0.0f ) {
      mTime = 0.0f;
      mFgExpired = true;
      mFgRunning = false;
    }
  }
}
//-----------------------------------------------------------------------------
float CTimer::getTimeRemaining()
{
  return mTime;
}
//-----------------------------------------------------------------------------
bool CTimer::isExpired()
{
  return mFgExpired;
}
//-----------------------------------------------------------------------------
bool CTimer::isRunning()
{
  return mFgRunning;
}
//-----------------------------------------------------------------------------
void CTimer::start ( float timeout )
{
  mTime = timeout;
  mFgRunning = true;
  mFgExpired = false;
}
//-----------------------------------------------------------------------------
void CTimer::stop()
{
  mFgRunning = false;
}
//-----------------------------------------------------------------------------
} // namespace
