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
#include "rectangle.h"
#include <stdio.h>
#include <sstream>

namespace Rapi
{

//-----------------------------------------------------------------------------
CRectangle::CRectangle( float x, float y, float w, float h )
{
  mBottomLeft.mX = x;
  mBottomLeft.mY = y;
  mWidth = w;
  mHeight = h;
}
//-----------------------------------------------------------------------------
CRectangle::CRectangle( const CRectangle& rect )
{
  mBottomLeft = rect.mBottomLeft;
  mWidth = rect.mWidth;
  mHeight = rect.mHeight;
}
//-----------------------------------------------------------------------------
CRectangle::CRectangle( const CPoint2d bottomLeft, const CPoint2d topRight )
{
  mBottomLeft = bottomLeft;
  mWidth = topRight.mX - mBottomLeft.mX;
  mHeight = topRight.mY - mBottomLeft.mY;
}
//-----------------------------------------------------------------------------
CRectangle::~CRectangle()
{
}
//-----------------------------------------------------------------------------
CRectangle& CRectangle::operator= ( const CRectangle& rect )
{
  mBottomLeft = rect.mBottomLeft;
  mWidth = rect.mWidth;
  mHeight = rect.mHeight;

printf("CRectangle::operator= ");
mBottomLeft.print();
  return *this;
}
//-----------------------------------------------------------------------------
bool CRectangle::isValid()
{
  if (( mWidth >= 0 ) && ( mHeight >= 0 ) )
    return true; // valid

  return false; // not valid
}
//-----------------------------------------------------------------------------
double CRectangle::getWidth() const
{
  return mWidth;
}
//-----------------------------------------------------------------------------
double CRectangle::getHeight() const
{
  return mHeight;
}
//-----------------------------------------------------------------------------
CPoint2d CRectangle::getBottomLeft() const
{
  return mBottomLeft;
}
//-----------------------------------------------------------------------------
CPoint2d CRectangle::getTopRight() const
{
  CPoint2d topRight;

  topRight.mX = mBottomLeft.mX + mWidth;
  topRight.mY = mBottomLeft.mY + mHeight;
  return topRight;
}
//-----------------------------------------------------------------------------
void CRectangle::print() const
{
  printf( "CRectangle: x=%f y=%f w=%f h=%f\n", mBottomLeft.mX, mBottomLeft.mY,
          mWidth, mHeight );
}
//-----------------------------------------------------------------------------
std::string CRectangle::toStr() const
{
  std::ostringstream strOut;

  strOut << "x=" << mBottomLeft.mX << " y=" << mBottomLeft.mY
  << " w=" << mWidth << " h=" << mHeight;
  return strOut.str();
}
//-----------------------------------------------------------------------------
void CRectangle::setCoordinates( float x1, float y1, float x2, float y2 )
{
  mBottomLeft.mX = x1;
  mBottomLeft.mY = y1;

  mWidth = x2 - x1;
  mHeight = y2 - y1;
}
//-----------------------------------------------------------------------------
void CRectangle::setBottomLeft( CPoint2d point )
{
  mBottomLeft = point;
}
//-----------------------------------------------------------------------------
void CRectangle::setTopRight( CPoint2d point )
{
  mWidth = point.mX - mBottomLeft.mX;
  mHeight = point.mY - mBottomLeft.mY;
}
//-----------------------------------------------------------------------------
bool CRectangle::isInside( CPoint2d point )
{
  float dx;
  float dy;

  dx = point.mX - mBottomLeft.mX;
  dy = point.mY - mBottomLeft.mY;

  if (( dx >= 0.0 ) && ( dx <= mWidth ) &&
      ( dy >= 0.0 ) && ( dy <= mHeight ) )
    return true; // inside

  return false; // outside
}
//-----------------------------------------------------------------------------
double CRectangle::getLeft() const
{
  return mBottomLeft.mX;
}
//-----------------------------------------------------------------------------
double CRectangle::getRight() const
{
  return mBottomLeft.mX + mWidth;
}
//-----------------------------------------------------------------------------
double CRectangle::getTop() const
{
  return mBottomLeft.mY + mHeight;
}
//-----------------------------------------------------------------------------
double CRectangle::getBottom() const
{
  return mBottomLeft.mY;
}
//-----------------------------------------------------------------------------
} // namespace
