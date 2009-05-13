/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: cpowermanagementview.cpp,v 1.1 2008/12/04 02:01:05 jwawerla Exp $
 ***************************************************************************
 *                                                                         *
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
 ***************************************************************************
 * $Log: cpowermanagementview.cpp,v $
 * Revision 1.1  2008/12/04 02:01:05  jwawerla
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2008/02/02 22:19:54  jwawerla
 * new to cvs
 *
 * Revision 1.1  2008/01/17 23:41:11  jwawerla
 * fixed some bugs
 *
 * Revision 1.1.1.1  2008/01/10 19:43:54  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#include "powerpackwidget.h"
#include "unicodechar.h"
#include "assert.h"
#include <QHBoxLayout>

namespace Rapi
{
//---------------------------------------------------------------------------
CPowerPackWidget::CPowerPackWidget ( APowerPack* powerpack,
                                     QWidget* parent ) : ADeviceWidget ( parent )
{

  QHBoxLayout* batteryLayout;
  QHBoxLayout* chargingLayout;

  assert(powerpack);
  mPowerPack = powerpack;

  setTitle ( "Powerpack" );

  mBatteryBox = new QGroupBox ( "Battery", this );
  mChargingBox = new QGroupBox ( "Charger", this );
  batteryLayout = new QHBoxLayout();
  chargingLayout = new QHBoxLayout();

  mBatteryBox->setLayout ( batteryLayout );
  mChargingBox->setLayout ( chargingLayout );
  mMainLayout->addWidget ( mBatteryBox );
  mMainLayout->addWidget ( mChargingBox );

  mBatCapacity = new CDataLine ( mBatteryBox, "Capacity [Wh]" );
  mBatCapacity->setFloatPrecision ( 3 );
  batteryLayout->addWidget ( mBatCapacity );

  mMaxBatCapacity = new CDataLine ( mBatteryBox, "Max. Capacity [Wh]" );
  batteryLayout->addWidget ( mMaxBatCapacity );

  mCurrent = new CDataLine ( mBatteryBox, "Current [A]" );
  batteryLayout->addWidget ( mCurrent );

  mVoltage = new CDataLine ( mBatteryBox, "Voltage [V]" );
  batteryLayout->addWidget ( mVoltage );

  mBatTemp = new CDataLine( mBatteryBox, "Temp ["+Q_DEGREE+"]");
  batteryLayout->addWidget(mBatTemp);


  mChargingLed = new CDataLed( mChargingBox, "Charging");
  mChargingLed->setData( CDataLed::RED_OFF);
  chargingLayout->addWidget(mChargingLed);

  mChargingSource = new CDataLine( mChargingBox, "Source");
  chargingLayout->addWidget(mChargingSource);

  mChargingState =  new CDataLine( mChargingBox, "State");
  chargingLayout->addWidget(mChargingState);

  mDissipated = new CDataLine( mChargingBox, "Dissipated [Wh]");
  chargingLayout->addWidget(mDissipated);



}
//---------------------------------------------------------------------------
CPowerPackWidget::~CPowerPackWidget()
{
}
//---------------------------------------------------------------------------
void CPowerPackWidget::updateData()
{
  if ( isChecked() ) {
    mBatteryBox->setHidden(false);
    mChargingBox->setHidden(false);
    mBatCapacity->setData ( mPowerPack->getBatteryCapacity() );
    mMaxBatCapacity->setData ( mPowerPack->getMaxBatteryCapacity() );
    mCurrent->setData ( mPowerPack->getCurrent() );
    mVoltage->setData ( mPowerPack->getVoltage() );
    mBatTemp->setData ( mPowerPack->getBatteryTemperature() );
    mChargingSource->setData( mPowerPack->getChargingSource() );
    mChargingState->setData( mPowerPack->getChargingState() );
    mDissipated->setData( mPowerPack->getTotalEnergyDissipated() );

    if (mPowerPack->isCharging() )
      mChargingLed->setData( CDataLed::RED_ON);
    else
      mChargingLed->setData( CDataLed::RED_OFF);
  }
  else {
    mBatteryBox->setHidden(true);
    mChargingBox->setHidden(true);
  }
  ADeviceWidget::updateData ( mPowerPack );
}
//---------------------------------------------------------------------------

} // namespace
