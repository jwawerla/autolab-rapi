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
#include "limit.h"

namespace Rapi {

//-----------------------------------------------------------------------------
CLimit::CLimit(const double lower, const double upper)
{
  mLower = lower;
  mUpper = upper;
}
//-----------------------------------------------------------------------------
CLimit::~CLimit()
{
}
//-----------------------------------------------------------------------------
double CLimit::limit(double var) const
{
  if ( var > mUpper)
    return mUpper;
  if ( var < mLower )
    return mLower;

  return var;
}
//-----------------------------------------------------------------------------
void CLimit::setLimit( const double lower, const double upper)
{
  mLower = lower;
  mUpper = upper;
}
//-----------------------------------------------------------------------------
double CLimit::getLowerLimit() const
{
  return mLower;
}
//-----------------------------------------------------------------------------
double CLimit::getUpperLimit() const
{
  return mUpper;
}
//-----------------------------------------------------------------------------
} // namespace
