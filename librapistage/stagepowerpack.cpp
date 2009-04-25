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
#include "stagepowerpack.h"
#include "rapierror.h"
#include "conversions.h"

namespace Rapi
{
//---------------------------------------------------------------------------
// Callback for stage, stage calls this function if the corresponding model
// is updated
int powerUpdate ( Stg::ModelPosition* model, CStagePowerPack* power )
{
  power->updateData();
  return 0; // ok
}
//-----------------------------------------------------------------------------
CStagePowerPack::CStagePowerPack ( Stg::ModelPosition* stgModel,
                                   std::string devName )
    : APowerPack ( devName )
{
  mStgPowerPack = stgModel->FindPowerPack();
  mSimInterval = stgModel->GetWorld()->GetSimInterval() / 1e6;

  stgModel->AddUpdateCallback ( ( Stg::stg_model_callback_t )
                                powerUpdate,
                                this );
  if ( mStgPowerPack == NULL ) {
    ERROR1 ("The stage model %s does not have a power pack",
               stgModel->Token() );
  }

  // Stage does not provide all data fiels, so we just make something up
  mVoltage = 12.0;
  mBatteryTemperature = 25.0;
  mMaxCurrent = 5.0;
}
//-----------------------------------------------------------------------------
CStagePowerPack::~CStagePowerPack()
{
}
//-----------------------------------------------------------------------------
int CStagePowerPack::init()
{
  return 1; // success
}
//----------------------------------------------------------------------------
void CStagePowerPack::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//----------------------------------------------------------------------------
void CStagePowerPack::updateData()
{
  double prevBatteryCapacity;

  if ( ( mFgEnabled ) && ( mStgPowerPack ) ) {

    prevBatteryCapacity = mBatteryCapacity;
    mBatteryCapacity =  JOULES_TO_AMPHOURS ( mStgPowerPack->GetStored(),
                        mVoltage );
    mMaxBatteryCapacity = JOULES_TO_AMPHOURS ( mStgPowerPack->GetCapacity(),
                          mVoltage );
    mTotalEnergyDissipated = JOULES_TO_WATTHOURS (
                               mStgPowerPack->GetDissipated() );

    // take the change in battery capacity (in Ah)
    // devide by simulution step duration (converted to hours)
    mCurrent = ( prevBatteryCapacity - mBatteryCapacity ) /
               SECONDS_TO_HOURS ( mSimInterval );
    notifyDataUpdateObservers();
  }
}
//----------------------------------------------------------------------------
tChargeState CStagePowerPack::getChargingState()
{
  if ( mStgPowerPack->GetCharging() == true ) {
    return FULL_CHARGING;
  }

  return NOT_CHARGING;
}
//----------------------------------------------------------------------------
} // namespace

