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
#include "booleanstatevariable.h"

namespace Rapi {

//-----------------------------------------------------------------------------
CBooleanStateVariable::CBooleanStateVariable()
 : AStateVariable()
{
}
//-----------------------------------------------------------------------------
CBooleanStateVariable::~CBooleanStateVariable()
{
}
//-----------------------------------------------------------------------------
bool CBooleanStateVariable::operator= ( const bool b )
{
  if ( mValue != b) {
    mModificationTimestamp = mUpdateTimestamp;
    mFgModified = true;
    mValue = b;
  }

  return mValue;
}
//-----------------------------------------------------------------------------
void CBooleanStateVariable::update(float dt)
{
  if (mUpdateTimestamp - mModificationTimestamp > dt)
    mFgModified = false;

  mUpdateTimestamp += dt;
}
//-----------------------------------------------------------------------------
bool CBooleanStateVariable::operator== ( const bool b )
{
  if ( mValue == b)
    return true;

  return false;
}
//-----------------------------------------------------------------------------
bool CBooleanStateVariable::operator!= ( const bool b )
{
  if ( mValue != b)
    return true;

  return false;
}
//-----------------------------------------------------------------------------


} // namespace
