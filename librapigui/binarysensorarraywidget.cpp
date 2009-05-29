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
#include "binarysensorarraywidget.h"
#include <assert.h>
#include <QGridLayout>
#include <math.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CBinarySensorArrayWidget::CBinarySensorArrayWidget (
  ABinarySensorArray* sensorArray, QWidget* parent )
    : ADeviceWidget ( parent )
{
  QString str;
  const int NUM_COL = 5;
  int row = 0;
  int col = 0;
  QGridLayout* layout;

  assert ( sensorArray );
  mSensorArray = sensorArray;
  setTitle ( "SensorArray" );

  layout = new QGridLayout ( this);
  mLed = new CDataLed*[mSensorArray->getNumSamples() ];

  for ( unsigned int i = 0; i < mSensorArray->getNumSamples(); i++ ) {
    str = "Sensor " + str.setNum ( i );
    mLed[i] = new CDataLed ( this, str );
    layout->addWidget ( mLed[i], row, col );
    col ++;
    if ( col >= NUM_COL ) {
      col = 0;
      row++;
    }
  }
}
//-----------------------------------------------------------------------------
CBinarySensorArrayWidget::~CBinarySensorArrayWidget()
{
}
//-----------------------------------------------------------------------------
void CBinarySensorArrayWidget::updateData ()
{
  if ( isChecked() ) {
    for ( unsigned int i = 0; i < mSensorArray->getNumSamples(); i++ ) {
      mLed[i]->setHidden ( false );
      if ( mSensorArray->mBitData[i] )
        mLed[i]->setData ( CDataLed::GREEN_ON );
      else
        mLed[i]->setData ( CDataLed::GREEN_OFF );
    }
  }
  else {
    for ( unsigned int i = 0; i < mSensorArray->getNumSamples(); i++ )
      mLed[i]->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------

} // namespace
