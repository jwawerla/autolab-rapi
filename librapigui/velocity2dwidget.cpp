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
#include "velocity2dwidget.h"
#include "utilities.h"
#include "unicodechar.h"

#include <QHBoxLayout>

namespace Rapi
{

//-----------------------------------------------------------------------------
CVelocity2dWidget::CVelocity2dWidget ( QWidget* parent )
    : QGroupBox ( parent )
{
  setTitle ( "Velocity" );
  setFlat ( false );
  setCheckable ( true );

  QHBoxLayout* layout = new QHBoxLayout ( this );

  mXDotLine = new CDataLine ( this, "Vx [m/s]" );
  mXDotLine->setFloatPrecision(3);
  layout->addWidget ( mXDotLine );
  mXDotLine->setColorEnable( true );

  mYDotLine = new CDataLine ( this, "Vy [m/s]" );
  mYDotLine->setFloatPrecision(3);
  layout->addWidget ( mYDotLine );
  mYDotLine->setColorEnable( true );

  mYawDotLine = new CDataLine ( this, "V"+Q_YAW+" ["+Q_DEGREE+"/s]" );
  layout->addWidget ( mYawDotLine );

  setLayout ( layout );
}
//-----------------------------------------------------------------------------
CVelocity2dWidget::~CVelocity2dWidget()
{
}
//-----------------------------------------------------------------------------
void CVelocity2dWidget::updateData(CVelocity2d velocity)
{
  if ( isChecked() ) {
    mXDotLine->setHidden ( false );
    mYDotLine->setHidden ( false );
    mYawDotLine->setHidden ( false );

    mXDotLine->setData ( velocity.mXDot );
    mYDotLine->setData ( velocity.mYDot );
    mYawDotLine->setData ( R2D ( velocity.mYawDot ) );
  }
  else {
    mXDotLine->setHidden ( true );
    mYDotLine->setHidden ( true );
    mYawDotLine->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------
} // namespace
