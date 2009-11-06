#include "rpcpowerpack.h"

namespace Rapi
{
//------------------------------------------------------------------------------
RpcPowerPack::RpcPowerPack ( RobotRpcClient * client, std::string devName )
    : APowerPack ( devName )
{
  mClient = client;
  if( mClient->getPowerPackDev( mMaxBatteryCapacity ) == false ) {
    std::cerr << "Can't initialize power pack" << std::endl;
    return;
  }
  mChargeSource = NO_CHARGER;
  mChargeState = NOT_CHARGING;
  mMaxCurrent = 4.0; // TODO: find a reasonable value
  setEnabled ( true );
}
//------------------------------------------------------------------------------
RpcPowerPack::~RpcPowerPack()
{
}
//-----------------------------------------------------------------------------
int RpcPowerPack::init()
{
  return 1;
}
//------------------------------------------------------------------------------
void RpcPowerPack::setEnabled( bool enable)
{
  mFgEnabled = enable;
}
//------------------------------------------------------------------------------
bool RpcPowerPack::isCharging()
{
  if ( getChargingSource() != NO_CHARGER )
    return true;

  return false;
}
//------------------------------------------------------------------------------
int RpcPowerPack::getChargingSource()
{
  // this assumes we have a chatterbox
  switch ( mChargeSource ) {
    case 0x01: return INTEGRAL_CHARGER;
    case 0x02: return HOMEBASE;
  }
  ; // switch

  return NO_CHARGER;
}
//------------------------------------------------------------------------------
tChargeState RpcPowerPack::getChargingState()
{
  switch ( mChargeState ) {
    case 0x00: return NOT_CHARGING;
    case 0x01: return RECONDITIONING;
    case 0x02: return FULL_CHARGING;
    case 0x03: return TRICKLE_CHARGING;
    case 0x04: return WAITING;
    default: return CHARGING_FAULT;
  } // switch
}
//------------------------------------------------------------------------------
void RpcPowerPack::updateData( const double dt )
{
  mClient->getPowerPack( mBatteryCapacity, mCurrent, mVoltage,
                         mBatteryTemperature, mBatteryLevel,
                         mTotalEnergyDissipated, mChargeState,
                         mChargeSource );
}
//------------------------------------------------------------------------------

} // namespace
