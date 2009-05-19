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
#include "pose2dwidget.h"
#include "utilities.h"
#include "unicodechar.h"
#include <QHBoxLayout>

namespace Rapi
{

//-----------------------------------------------------------------------------
CPose2dWidget::CPose2dWidget ( QWidget* parent )
    : QGroupBox ( parent )
{
  setCheckable ( true );
  setTitle ( "Pose" );
  setFlat ( false );
  QHBoxLayout* layout = new QHBoxLayout ( this );

  mXLine = new CDataLine ( this, "X [m]" );
  layout->addWidget ( mXLine );

  mYLine = new CDataLine ( this, "Y [m]" );
  layout->addWidget ( mYLine );

  mYawLine = new CDataLine ( this, Q_YAW + " ["+Q_DEGREE+"]" );
  layout->addWidget ( mYawLine );

  setLayout ( layout );
}
//-----------------------------------------------------------------------------
CPose2dWidget::~CPose2dWidget()
{
}
//-----------------------------------------------------------------------------
void CPose2dWidget::updateData ( CPose2d pose )
{
  if ( isChecked() ) {
    mXLine->setHidden ( false );
    mYLine->setHidden ( false );
    mYawLine->setHidden ( false );
    mXLine->setData ( pose.mX );
    mYLine->setData ( pose.mY );
    mYawLine->setData ( R2D ( pose.mYaw ) );
  }
  else {
    mXLine->setHidden ( true );
    mYLine->setHidden ( true );
    mYawLine->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------
} // namespace
