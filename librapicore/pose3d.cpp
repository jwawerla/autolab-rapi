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

#include "pose3d.h"
#include "utilities.h"
#include <stdio.h>

namespace Rapi
{
//-----------------------------------------------------------------------------
CPose3d::CPose3d ( double x, double y, double z,
                   double roll, double pitch, double yaw )
{
  mX = x;
  mY = y;
  mZ = z;
  mRoll = normalizeAngle ( roll );
  mPitch = normalizeAngle ( pitch );
  mYaw = normalizeAngle ( yaw );
}
//-----------------------------------------------------------------------------
CPose3d::~CPose3d()
{
  // nothing to do
}
//-----------------------------------------------------------------------------
CPose3d::CPose3d ( CPose3d const& pose )
{
  mX = pose.mX;
  mY = pose.mY;
  mZ = pose.mZ;
  mRoll = normalizeAngle ( pose.mRoll );
  mPitch = normalizeAngle ( pose.mPitch );
  mYaw = normalizeAngle ( pose.mYaw );
}
//-----------------------------------------------------------------------------
void CPose3d::print()
{
  printf ( "CPose3d: x=%f y=%f z=%f roll=%f pitch=%f yaw=%f\n",
           mX, mY, mZ, R2D ( mRoll ), R2D ( mPitch ), R2D ( mYaw ) );
}
//-----------------------------------------------------------------------------
void CPose3d::operator= ( const CPose3d pose )
{
  mX = pose.mX;
  mY = pose.mY;
  mZ = pose.mZ;
  mRoll = normalizeAngle ( pose.mRoll );
  mPitch = normalizeAngle ( pose.mPitch );
  mYaw = normalizeAngle ( pose.mYaw );
}
//-----------------------------------------------------------------------------
CPose3d CPose3d::operator+ ( const CPose3d pose )
{
  CPose3d newPose;

  newPose.mX = mX + pose.mX;
  newPose.mY = mY + pose.mY;
  newPose.mZ = mZ + pose.mZ;
  newPose.mRoll = normalizeAngle ( mRoll + pose.mRoll );
  newPose.mPitch = normalizeAngle ( mPitch + pose.mPitch );
  newPose.mYaw = normalizeAngle ( mYaw + pose.mYaw );
  return newPose;
}
//-----------------------------------------------------------------------------
bool CPose3d::operator!= ( const CPose3d pose )
{
  if ( mX != pose.mX )
    return true;
  if ( mY != pose.mY )
    return true;
  if ( mZ != pose.mZ )
    return true;
  if ( mRoll != pose.mRoll )
    return true;
  if ( mPitch != pose.mPitch )
    return true;
  if ( mYaw != pose.mYaw )
    return true;

  return false;
}
//-----------------------------------------------------------------------------
double CPose3d::distance ( const CPose3d pose )
{
  return sqrt ( pow2 ( pose.mX - mX ) + pow2 ( pose.mY - mY )
                + pow2 ( pose.mZ - mZ ) );
}
//-----------------------------------------------------------------------------

} // namespace
