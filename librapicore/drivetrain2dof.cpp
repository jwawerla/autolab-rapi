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
}
//-----------------------------------------------------------------------------
ADrivetrain2dof::~ADrivetrain2dof()
{
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::stop()
{
  setSpeedCmd ( CVelocity2d ( 0.0, 0.0 ) );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setRotationalSpeedCmd ( float turnrate )
{
  setSpeedCmd ( CVelocity2d ( mVelocityCmd.mVX, 0.0, turnrate ) );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::setTranslationalSpeedCmd ( float velocity )
{
  setSpeedCmd ( CVelocity2d ( velocity, 0.0,  mVelocityCmd.mYawDot ) );
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
  mVelocityCmd.mVX = LIMIT ( mVelocityCmd.mVX,
                             mLowerVelocityLimit.mVX,
                             mUpperVelocityLimit.mVX );

  mVelocityCmd.mVY = LIMIT ( mVelocityCmd.mVY,
                             mLowerVelocityLimit.mVY,
                             mUpperVelocityLimit.mVY );

  mVelocityCmd.mYawDot = LIMIT ( mVelocityCmd.mYawDot,
                             mLowerVelocityLimit.mYawDot,
                             mUpperVelocityLimit.mYawDot );
}
//-----------------------------------------------------------------------------
void ADrivetrain2dof::print()
{
  printf ( "ADriveTrain2dof:v=%01.2f w=%02.1f ", mVelocityCmd.mVX,
           R2D ( mVelocityCmd.mYawDot ) );
  mOdometry->print();
}
//-----------------------------------------------------------------------------
bool ADrivetrain2dof::isStopped()
{
  if ( ( mVelocityCmd.mVX == 0.0 ) &&
       ( mVelocityCmd.mYawDot == 0.0 ) )
    return true; // stopped

  return false;
}
//-----------------------------------------------------------------------------
bool ADrivetrain2dof::isStalled()
{
  return mFgStalled;
}
//-----------------------------------------------------------------------------
CVelocity2d ADrivetrain2dof::getVelocityCmd()
{
  return mVelocityCmd;
}
//-----------------------------------------------------------------------------
COdometry* ADrivetrain2dof::getOdometry()
{
  return mOdometry;
}
//-----------------------------------------------------------------------------

} // namespace
