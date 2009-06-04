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
#include "drivetrain2dof.h"
#include "utilities.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
ADrivetrain2dof::ADrivetrain2dof ( std::string devName )
    : ADevice ( devName )
{
  mFgStalled = false;
  mStalledTimer = 0.0;
}
//-----------------------------------------------------------------------------
ADrivetrain2dof::~ADrivetrain2dof()
{
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::stop()
{
  setVelocityCmd ( CVelocity2d ( 0.0, 0.0 ) );
}
//-----------------------------------------------------------------------------
CVelocity2d ADrivetrain2dof::getVelocity() const
{
  return mVelocityMeas;
}
//-----------------------------------------------------------------------------
float ADrivetrain2dof::stalledSince() const
{
  return mStalledTimer;
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setRotationalVelocityCmd ( float turnrate )
{
  setVelocityCmd ( CVelocity2d ( mVelocityCmd.mXDot, 0.0, turnrate ) );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setTranslationalVelocityCmd ( float velocity )
{
  setVelocityCmd ( CVelocity2d ( velocity, 0.0,  mVelocityCmd.mYawDot ) );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setUppererVelocityLimit ( CVelocity2d limit )
{
  mUpperVelocityLimit = limit;
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setLowerVelocityLimit ( CVelocity2d limit )
{
  mLowerVelocityLimit = limit;
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::applyVelocityLimits()
{
  mVelocityCmd.mXDot = limit ( mVelocityCmd.mXDot,
                             mLowerVelocityLimit.mXDot,
                             mUpperVelocityLimit.mXDot );

  mVelocityCmd.mYDot = limit ( mVelocityCmd.mYDot,
                             mLowerVelocityLimit.mYDot,
                             mUpperVelocityLimit.mYDot );

  mVelocityCmd.mYawDot = limit ( mVelocityCmd.mYawDot,
                             mLowerVelocityLimit.mYawDot,
                             mUpperVelocityLimit.mYawDot );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::print() const
{
  printf ( "ADriveTrain2dof:v=%01.2f w=%02.1f ", mVelocityCmd.mXDot,
           R2D ( mVelocityCmd.mYawDot ) );
  mOdometry->print();
}
//-----------------------------------------------------------------------------
bool ADrivetrain2dof::isStopped() const
{
  if ( isAboutZero(mVelocityCmd.mXDot ) &&
       isAboutZero(mVelocityCmd.mYawDot ) )
    return true; // stopped

  return false;
}
//-----------------------------------------------------------------------------
bool ADrivetrain2dof::isStalled() const
{
  return mFgStalled;
}
//-----------------------------------------------------------------------------
CVelocity2d ADrivetrain2dof::getVelocityCmd() const
{
  return mVelocityCmd;
}
//-----------------------------------------------------------------------------
COdometry* ADrivetrain2dof::getOdometry() const
{
  return mOdometry;
}
//-----------------------------------------------------------------------------

} // namespace
