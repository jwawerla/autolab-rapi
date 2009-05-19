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

#include "velocity2d.h"
#include <stdio.h>
#include "utilities.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CVelocity2d::CVelocity2d ( double vX, double vY, double yawDot )
{
  mVX = vX;
  mVY = vY;
  mYawDot = yawDot;
}
//-----------------------------------------------------------------------------

CVelocity2d::~CVelocity2d()
{
}
//-----------------------------------------------------------------------------
void CVelocity2d::print() const
{
  printf ( "CVelocity2d: vx=%f vy=%f yawDot=%f\n",
           mVX, mVY, R2D ( mYawDot ) );
}
//-----------------------------------------------------------------------------
void CVelocity2d::operator = ( CVelocity2d const &vel )
{
  mVX = vel.mVX;
  mVY = vel.mVY;
  mYawDot = vel.mYawDot;
}
//-----------------------------------------------------------------------------
void CVelocity2d::setZero()
{
  mVX = 0.0;
  mVY = 0.0;
  mYawDot = 0.0;
}
//-----------------------------------------------------------------------------

} // namespace
