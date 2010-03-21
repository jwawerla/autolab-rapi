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

#include "cbpowerpack.h"
#include "utilities.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBPowerPack::CCBPowerPack ( CCBDriver* cbDriver, std::string devName )
    : APowerPack ( devName )
{
  mCBDriver = cbDriver;

  mMaxBatteryCapacity = 0.0;
  mBatteryCapacity = 0.0;
  mCurrent = 0.0;
  mMaxCurrent = 4.0;  // TODO: set this to the correct value, 4 is just a guess
  mVoltage = 0.0;
  setEnabled ( true );
  updateData(0.1);
}
//-----------------------------------------------------------------------------
CCBPowerPack::~CCBPowerPack()
{
}
//-----------------------------------------------------------------------------
void CCBPowerPack::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBPowerPack::init()
{
  return 1; // nothing to do !
}
//-----------------------------------------------------------------------------
bool CCBPowerPack::isCharging()
{
  if ( getChargingSource() != NO_CHARGER )
    return true;

  return false;
}
//-----------------------------------------------------------------------------
void CCBPowerPack::updateData( const double dt)
{
  if ( mFgEnabled == true ) {
    // convert from [mAh] to [Ah]
    mMaxBatteryCapacity = ( float ) ( mCBDriver->mCreateSensorPackage.batMaxCapacity )
                          /1000.0;
    // convert from [mAh] to [Ah]
    mBatteryCapacity = ( float ) ( mCBDriver->mCreateSensorPackage.batCapacity )
                       /1000.0;
    // convert from [mA] to [A]
    mCurrent = ( float ) ( mCBDriver->mCreateSensorPackage.current ) / 1000.0;
    // convert from [mV] to [V]
    mVoltage = ( float ) ( mCBDriver->mCreateSensorPackage.voltage ) / 1000.0;
    // comes in [C]
    mBatteryTemperature = ( float ) ( mCBDriver->mCreateSensorPackage.batTemp );

    // update time stamp of this measurement
    mTimeStamp = timeStamp();

  } // enabled
}
//-----------------------------------------------------------------------------
int CCBPowerPack::getChargingSource()
{
  switch ( mCBDriver->mCreateSensorPackage.chargingSource ) {
    case 0x01: return INTEGRAL_CHARGER;
    case 0x02: return HOMEBASE;
  }
  ; // switch

  return NO_CHARGER;
}
//-----------------------------------------------------------------------------
tChargeState CCBPowerPack::getChargingState()
{
  switch ( mCBDriver->mCreateSensorPackage.chargingState ) {
    case 0x00: return NOT_CHARGING;
    case 0x01: return RECONDITIONING;
    case 0x02: return FULL_CHARGING;
    case 0x03: return TRICKLE_CHARGING;
    case 0x04: return WAITING;
  } // switch

  return CHARGING_FAULT;

}
//-----------------------------------------------------------------------------
void CCBPowerPack::print()
{
  tChargeState state;

  state = getChargingState();
  printf ( "Power: %02.2fV, %02.3fA, bat %02.3fAh,  maxBat %02.3fAh, %02.1f ",
           mVoltage, mCurrent,
           mBatteryCapacity, mMaxBatteryCapacity,
           mBatteryTemperature );

  switch ( state ) {
    case NOT_CHARGING:
      printf ( "not charging " );
      break;
    case RECONDITIONING:
      printf ( "reconditioning " );
      break;
    case FULL_CHARGING:
      printf ( "full charging " );
      break;
    case TRICKLE_CHARGING:
      printf ( "trickel charging " );
      break;
    case WAITING:
      printf ( "waiting " );
      break;
    case CHARGING_FAULT:
      printf ( "charging fault " );
      break;
  };

  if ( getChargingSource() == 0 )
    printf ( "No Charger\n" );

  if ( getChargingSource() == 1 )
    printf ( "Internal Charger\n" );

  if ( getChargingSource() == 2 )
    printf ( "Home Base\n" );
}
//-----------------------------------------------------------------------------
} // namespace


