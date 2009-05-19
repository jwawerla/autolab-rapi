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

#include "point2d.h"
#include "utilities.h"
#include <stdio.h>
#include <math.h>

namespace Rapi {

//-----------------------------------------------------------------------------
CPoint2d::CPoint2d(double x, double y)
{
  mX = x;
  mY = y;
}
//-----------------------------------------------------------------------------
CPoint2d::~CPoint2d()
{
  // nothing to do
}
//-----------------------------------------------------------------------------
void CPoint2d::fromPolar(double length, double angle)
{
  mX = length * cos(angle);
  mY = length * sin(angle);
}
//-----------------------------------------------------------------------------
double CPoint2d::distance(const CPoint2d point) const
{
  return EUCLIDIAN(point.mX, point.mY, mX, mY);
}
//-----------------------------------------------------------------------------
void CPoint2d::print() const
{
  printf("CPoint2d: x %f y %f \n", mX, mY);
}
//-----------------------------------------------------------------------------
void CPoint2d::operator= ( const CPoint2d point )
{
  mX = point.mX;
  mY = point.mY;
}
//-----------------------------------------------------------------------------
} // namespace
