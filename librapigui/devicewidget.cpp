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
  setFlat ( false );
  setTitle ( "Device" );
  setCheckable ( true );

  mMainLayout = new QVBoxLayout ( this );
  mMainLayout->setSizeConstraint(QLayout::SetNoConstraint);

  mGeneralInfoBox = new QGroupBox ( this );
  mGeneralInfoBox->setTitle("General");
  mMainLayout->addWidget ( mGeneralInfoBox );
  mGeneralInfoBox->setCheckable ( true );


  mGeneralInfoBoxLayout = new QHBoxLayout ( mGeneralInfoBox );
  mGeneralInfoBox->setLayout ( mGeneralInfoBoxLayout );
  mGeneralInfoBox->setChecked(false);

  mEnabledLed = new CDataLed ( mGeneralInfoBox, "Enabled" );
  mEnabledLed->setData ( CDataLed::GREEN_OFF );
  mGeneralInfoBoxLayout->addWidget ( mEnabledLed );

  setLayout ( mMainLayout );
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  connect(mGeneralInfoBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
  mEnabledLed->setHidden ( true );
}
//-----------------------------------------------------------------------------
ADeviceWidget::~ADeviceWidget()
{
}
//-----------------------------------------------------------------------------
void ADeviceWidget::toggled ( bool on )
{
  if ( on ) {
    mEnabledLed->setHidden ( false );
  }
  else {
    mEnabledLed->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------
void ADeviceWidget::updateData ( ADevice* device )
{
  if ( isChecked() ) {
    mGeneralInfoBox->setHidden ( false );
    if ( device->isEnabled() )
      mEnabledLed->setData ( CDataLed::GREEN_ON );
    else
      mEnabledLed->setData ( CDataLed::GREEN_OFF );
  }
  else {
    mGeneralInfoBox->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------

} // namespace
