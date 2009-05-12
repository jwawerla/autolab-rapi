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
#include "devicewidget.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
ADeviceWidget::ADeviceWidget ( QWidget* parent )
    : QGroupBox ( parent )
{
  QGroupBox* generalBox;
  QHBoxLayout* boxLayout;

  setFlat ( false );
  setTitle ( "Device" );

  mMainLayout = new QVBoxLayout ( this );

  generalBox = new QGroupBox ( this );
  //generalBox->setTitle ( "General" );
  mMainLayout->addWidget ( generalBox );

  boxLayout = new QHBoxLayout ( generalBox );
  generalBox->setLayout ( boxLayout );

  mEnabledLed = new CDataLed ( generalBox, "Enabled" );
  mEnabledLed->setData ( CDataLed::GREEN_OFF );
  boxLayout->addWidget ( mEnabledLed );

  mUpdateCheckBox = new QCheckBox ("Update", generalBox );
  mUpdateCheckBox->setChecked(true);
  boxLayout->addWidget ( mUpdateCheckBox );
  setLayout ( mMainLayout );
}
//-----------------------------------------------------------------------------
ADeviceWidget::~ADeviceWidget()
{
}
//-----------------------------------------------------------------------------
void ADeviceWidget::updateData ( ADevice* device )
{
  if ( device->isEnabled() )
    mEnabledLed->setData ( CDataLed::GREEN_ON );
  else
    mEnabledLed->setData ( CDataLed::GREEN_OFF );
}
//-----------------------------------------------------------------------------

} // namespace
