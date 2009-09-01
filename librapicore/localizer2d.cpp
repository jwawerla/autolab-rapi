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
#include "localizer2d.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
ALocalizer2d::ALocalizer2d ( std::string devName )
    : ADevice ( devName )
{
}
//-----------------------------------------------------------------------------
ALocalizer2d::~ALocalizer2d()
{
}
//-----------------------------------------------------------------------------
void ALocalizer2d::setCoordinateSystemOffset ( CPose2d offset )
{
  mCoordinateSystemOffset = offset;
}
//-----------------------------------------------------------------------------
CPose2d ALocalizer2d::getPose() const
{
  return mPose;
}
//-----------------------------------------------------------------------------
void ALocalizer2d::setPose( CPose2d pose)
{
  mPose = pose;
}
//-----------------------------------------------------------------------------
void ALocalizer2d::setToZero()
{
  mPose = CPose2d(0.0, 0.0, 0.0);
}
//-----------------------------------------------------------------------------

} // namespace

