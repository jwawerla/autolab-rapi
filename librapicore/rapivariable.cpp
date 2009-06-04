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
#include "rapivariable.h"
#include <assert.h>
#include "printerror.h"
#include <sstream>
#include <typeinfo>

namespace Rapi
{


//-----------------------------------------------------------------------------
template<class T>
CRapiVariable<T>::CRapiVariable() //: IRapiVar(), IRobotUpdate()
{
  mRobot = NULL;

  mModificationTimestamp = 0.0;
  mFgModified = false;
}
//-----------------------------------------------------------------------------
template<class T>
CRapiVariable<T>::~CRapiVariable()
{
}
//-----------------------------------------------------------------------------
template<class T>
void CRapiVariable<T>::setRobot ( ARobot* robot )
{
  assert ( robot );
  mRobot = robot;

  mRobot->registerStateVariable ( this );

  mModificationTimestamp = robot->getCurrentTime();
}
//-----------------------------------------------------------------------------
template<class T>
std::string CRapiVariable<T>::toStr() const
{
  std::ostringstream strOut;

  strOut << mValue;
  return strOut.str();
}
//-----------------------------------------------------------------------------
template<class T>
std::string CRapiVariable<T>::getTypeStr() const
{
  std::string str;

  str = typeid(T).name;
  return "CRapiVariable<"+ str + ">";
}
//-----------------------------------------------------------------------------
template<class T>
double CRapiVariable<T>::getElapsedTimeSinceChange() const
{
  return mRobot->getCurrentTime() - mModificationTimestamp;
}
//-----------------------------------------------------------------------------
template<class T>
T CRapiVariable<T>::operator! ()
{
  return !mValue;
}
//-----------------------------------------------------------------------------
template<class T>
T CRapiVariable<T>::value()
{
  return mValue;
}
//-----------------------------------------------------------------------------
template<class T>
bool CRapiVariable<T>::operator== ( const T value )
{
  if (mValue == value)
    return true;

  return false;
}
//-----------------------------------------------------------------------------
template<class T>
bool CRapiVariable<T>::operator!= ( const T value )
{
  if (mValue == value)
    return false;

  return true;
}
//-----------------------------------------------------------------------------
template<class T>
T CRapiVariable<T>::operator= ( const T value )
{
  if (not mRobot )
    PRT_ERR0("No robot available, use setRobot()");
  assert(mRobot);

  if ( mValue != value) {
    mModificationTimestamp = mRobot->getCurrentTime();
    mFgModified = true;
    mValue = value;
  }

  return mValue;
}
//-----------------------------------------------------------------------------
template<class T>
void CRapiVariable<T>::updateData(float dt)
{
  if (mRobot->getCurrentTime() - mModificationTimestamp > dt)
    mFgModified = false;
}
//-----------------------------------------------------------------------------

} // namespace
