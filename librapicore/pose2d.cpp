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

#include "pose2d.h"
#include "utilities.h"
#include <stdio.h>

namespace Rapi
{
//-----------------------------------------------------------------------------
CPose2d::CPose2d( double x, double y, double yaw )
{
  mX = x;
  mY = y;
  mYaw = NORMALIZE_ANGLE( yaw );
}
//-----------------------------------------------------------------------------
CPose2d::~CPose2d()
{
  // nothing to do
}
//-----------------------------------------------------------------------------
CPose2d::CPose2d( CPose2d const& pose )
{
  mX = pose.mX;
  mY = pose.mY;
  mYaw = NORMALIZE_ANGLE( pose.mYaw );
}
//-----------------------------------------------------------------------------
void CPose2d::print()
{
  printf("CPose2d: x=%f y=%f yaw=%f\n", mX, mY, R2D(mYaw));
}
//-----------------------------------------------------------------------------
void CPose2d::operator= ( const CPose2d pose )
{
  mX = pose.mX;
  mY = pose.mY;
  mYaw = NORMALIZE_ANGLE( pose.mYaw );
}
//-----------------------------------------------------------------------------
CPose2d CPose2d::operator+ ( const CPose2d pose )
{
  CPose2d newPose;

  newPose.mX = mX + pose.mX;
  newPose.mY = mY + pose.mY;
  newPose.mYaw = NORMALIZE_ANGLE( mYaw + pose.mYaw );

  return newPose;
}
//-----------------------------------------------------------------------------
CPose2d CPose2d::operator- ( const CPose2d pose )
{
  CPose2d newPose;

  newPose.mX = mX - pose.mX;
  newPose.mY = mY - pose.mY;
  newPose.mYaw = NORMALIZE_ANGLE( mYaw - pose.mYaw );

  return newPose;
}
//-----------------------------------------------------------------------------
bool CPose2d::operator!= ( const CPose2d pose )
{
  if ( mX != pose.mX )
    return true;
  if ( mY != pose.mY )
    return true;
  if ( mYaw != pose.mYaw )
    return true;

  return false;
}
//-----------------------------------------------------------------------------
double CPose2d::distance( const CPose2d pose )
{
  return sqrt( pow2( pose.mX - mX ) + pow2( pose.mY - mY ) );
}
//-----------------------------------------------------------------------------
double CPose2d::angleDifference( const CPose2d pose )
{
  return NORMALIZE_ANGLE( mYaw - pose.mYaw );
}
//-----------------------------------------------------------------------------

} // namespace
