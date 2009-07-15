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
#include "powerpack.h"
#include "utilities.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
APowerPack::APowerPack ( std::string devName )
    : ADevice ( devName )
{
  mMaxBatteryCapacity = 0.0;
  mBatteryCapacity = 0.0;
  mCurrent = 0.0;
  mMaxCurrent = 0.0;
  mVoltage = 0.0;
  mBatteryTemperature = 0.0;
}
//-----------------------------------------------------------------------------
APowerPack::~APowerPack()
{
}
//-----------------------------------------------------------------------------
double APowerPack::getCurrent() const
{
  return mCurrent;
}
//-----------------------------------------------------------------------------
double APowerPack::getBatteryCapacity() const
{
  return mBatteryCapacity;
}
//-----------------------------------------------------------------------------
double APowerPack::getMaxBatteryCapacity() const
{
  return mMaxBatteryCapacity;
}
//-----------------------------------------------------------------------------
double APowerPack::getVoltage() const
{
  return mVoltage;
}
//-----------------------------------------------------------------------------
double APowerPack::getBatteryTemperature() const
{
  return mBatteryTemperature;
}
//-----------------------------------------------------------------------------
int APowerPack::getChargingSource() const
{
  return 0; // default charging source
}
//-----------------------------------------------------------------------------
double APowerPack::getBatteryLevel() const
{
  if ( isInf(mBatteryCapacity) )
    return 1.0;

  return mBatteryCapacity / mMaxBatteryCapacity;
}
//-----------------------------------------------------------------------------
double APowerPack::getTotalEnergyDissipated() const
{
  return mTotalEnergyDissipated;
}
//-----------------------------------------------------------------------------
void APowerPack::print() const
{
  printf ( "APowerPack: voltage %02.2fV, current %02.3fA, cap. %02.3fWh, "\
           "max cap. %02.3fWh, %02.1f C\n",
           mVoltage, mCurrent, mBatteryCapacity,
           mMaxBatteryCapacity, mBatteryTemperature );
}
//--------------------------------------------------------------------------
} // namespace
