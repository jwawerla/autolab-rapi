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
#include "statevariable.h"
#include <assert.h>

namespace Rapi
{


//-----------------------------------------------------------------------------
template<class T>
CStateVariable<T>::CStateVariable() : IRobotUpdate()
{
  mUpdateTimestamp = 0.0;
  mModificationTimestamp = 0.0;
  mFgModified = false;
}
//-----------------------------------------------------------------------------
template<class T>
CStateVariable<T>::~CStateVariable()
{
}
//-----------------------------------------------------------------------------
template<class T>
void CStateVariable<T>::setRobot ( ARobot* robot )
{
  assert ( robot );
  robot->registerStateVariable ( this );

  mUpdateTimestamp = robot->getCurrentTime();
  mModificationTimestamp = robot->getCurrentTime();
}
//-----------------------------------------------------------------------------
template<class T>
bool CStateVariable<T>::operator== ( const T value )
{
  if (mValue == value)
    return true;

  return false;
}
//-----------------------------------------------------------------------------
template<class T>
bool CStateVariable<T>::operator!= ( const T value )
{
  if (mValue == value)
    return false;

  return true;
}
//-----------------------------------------------------------------------------
template<class T>
T CStateVariable<T>::operator= ( const T value )
{
  if ( mValue != value) {
    mModificationTimestamp = mUpdateTimestamp;
    mFgModified = true;
    mValue = value;
  }

  return mValue;
}
//-----------------------------------------------------------------------------
template<class T>
T CStateVariable<T>::operator*(T value)
{
  return mValue;
}
//-----------------------------------------------------------------------------
template<class T>
void CStateVariable<T>::updateData(float dt)
{
  if (mUpdateTimestamp - mModificationTimestamp > dt)
    mFgModified = false;

  mUpdateTimestamp += dt;
}
//-----------------------------------------------------------------------------

} // namespace
