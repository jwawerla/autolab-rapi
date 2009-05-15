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
#include "fiducialfinderwidget.h"
#include "assert.h"
#include "unicodechar.h"
#include "utilities.h"
#include <QString>

namespace Rapi
{

//-----------------------------------------------------------------------------
CFiducialFinderWidget::CFiducialFinderWidget ( AFiducialFinder* fiducial,
    QWidget* parent ) : ADeviceWidget ( parent )
{
  assert ( fiducial );
  mFiducialFinder = fiducial;

  setTitle ( "Fiducial Finder" );

  mTableWidget = new QTableWidget ( 0, 3, this );
  mMainLayout->addWidget ( mTableWidget );

  mTableWidget->setSelectionMode ( QAbstractItemView::NoSelection );
  mTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  mTableWidget->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( "Id" ) );
  mTableWidget->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( "Range [m]" ) );
  mTableWidget->setHorizontalHeaderItem ( 2, new QTableWidgetItem (
                                            "Bearing ["+Q_DEGREE+"]" ) );

  mOwnFiducialId = new CDataLine ( mGeneralInfoBox, "My Id" );
  mGeneralInfoBoxLayout->addWidget ( mOwnFiducialId );
  mOwnFiducialId->setHidden ( true );

  mMinRange = new CDataLine ( mGeneralInfoBox, "Min. Range [m]" );
  mGeneralInfoBoxLayout->addWidget ( mMinRange );
  mMinRange->setHidden ( true );

  mMaxRange = new CDataLine ( mGeneralInfoBox, "Max. Range [m]" );
  mGeneralInfoBoxLayout->addWidget ( mMaxRange );
  mMaxRange->setHidden ( true );

  mFov = new CDataLine ( mGeneralInfoBox, "FOV ["+Q_DEGREE+"]" );
  mGeneralInfoBoxLayout->addWidget ( mFov );
  mFov->setHidden ( true );
}
//-----------------------------------------------------------------------------
CFiducialFinderWidget::~CFiducialFinderWidget()
{
}
//-----------------------------------------------------------------------------
void CFiducialFinderWidget::toggled ( bool on )
{
  if ( on ) {
    mEnabledLed->setHidden ( false );
    mMinRange->setHidden ( false );
    mMaxRange->setHidden ( false );
    mFov->setHidden ( false );
    mOwnFiducialId->setHidden ( false );
  }
  else {
    mEnabledLed->setHidden ( true );
    mMinRange->setHidden ( true );
    mMaxRange->setHidden ( true );
    mFov->setHidden ( true );
    mOwnFiducialId->setHidden ( true );
  }
}
//-----------------------------------------------------------------------------
void CFiducialFinderWidget::updateData ()
{
  QTableWidgetItem* tableItem;
  QString str;

  if ( isChecked() ) {
    mOwnFiducialId->setData( mFiducialFinder->getFiducialSignal() );
    mFov->setData ( R2D ( mFiducialFinder->getFov() ) );
    mMinRange->setData ( mFiducialFinder->getMinRange() );
    mMaxRange->setData ( mFiducialFinder->getMaxRange() );
    mTableWidget->setHidden ( false );
    mTableWidget->setRowCount ( mFiducialFinder->getNumReadings() );

    for ( unsigned int i = 0; i < mFiducialFinder->getNumReadings(); i++ ) {
      // Fiducial ID
      tableItem = mTableWidget->item ( i, 0 );
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        mTableWidget->setItem ( i, 0, tableItem );
      }
      str.setNum ( mFiducialFinder->mFiducialData[i].id );
      tableItem->setText ( str );

      // Fiducial Range
      tableItem = mTableWidget->item ( i, 1 );
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        mTableWidget->setItem ( i, 1, tableItem );
      }
      str.setNum ( mFiducialFinder->mFiducialData[i].range );
      tableItem->setText ( str );

      // Fiducial Bearing
      tableItem = mTableWidget->item ( i, 2 );
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        mTableWidget->setItem ( i, 2, tableItem );
      }
      str.setNum ( R2D ( mFiducialFinder->mFiducialData[i].bearing ) );
      tableItem->setText ( str );
    }
  }
  else {
    mTableWidget->setHidden ( true );
  }
  ADeviceWidget::updateData ( mFiducialFinder );
}
//-----------------------------------------------------------------------------
} // namespace
