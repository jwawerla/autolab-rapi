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
#include <sstream>

namespace Rapi
{


//-----------------------------------------------------------------------------
CTimer::CTimer ( ARobot* robot, std::string name )
    : IRobotUpdate()
{
  assert ( robot );
  mRobot = robot;
  mName = name;
  robot->registerStateVariable ( this );
  mStartTime = mRobot->getCurrentTime();
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
    if ( mRobot->getCurrentTime() - mStartTime <= 0.0f ) {
      mFgExpired = true;
      mFgRunning = false;
    }
  }
}
//-----------------------------------------------------------------------------
std::string CTimer::toStr() const
{
  std::ostringstream strOut;

  strOut << "start " << mStartTime << " timeout " << mTimeout << " now "
  << mRobot->getCurrentTime();
  return  strOut.str();
}
//---------------------------------------------------------------------------
std::string CTimer::toCSV() const
{
  std::ostringstream strOut;

  strOut << mStartTime << "," << mTimeout << "," << mRobot->getCurrentTime();
  return strOut.str();
}
//-----------------------------------------------------------------------------
std::string CTimer::getCVSHeader() const
{
  std::ostringstream strOut;
  strOut << mName << "::start, " << mName << "::timeout," << mName << "::now";
  return strOut.str();
}
//-----------------------------------------------------------------------------
float CTimer::getRemainingTime() const
{
  return mStartTime + mTimeout - mRobot->getCurrentTime() ;
}
//-----------------------------------------------------------------------------
float CTimer::getElapsedTime() const
{
  return mRobot->getCurrentTime() - mStartTime;
}
//-----------------------------------------------------------------------------
bool CTimer::isExpired() const
{
  return mFgExpired;
}
//-----------------------------------------------------------------------------
bool CTimer::isRunning() const
{
  return mFgRunning;
}
//-----------------------------------------------------------------------------
void CTimer::start ( float timeout )
{
  mStartTime = mRobot->getCurrentTime();
  mTimeout = timeout;
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
