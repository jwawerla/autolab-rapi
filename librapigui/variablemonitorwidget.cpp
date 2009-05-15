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
#include "variablemonitorwidget.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CVariableMonitorWidget::CVariableMonitorWidget ( CVariableMonitor* monitor,
    QWidget* parent ) : ADeviceWidget ( parent )
{
  setTitle ( "Monitor" );

  mVariableMonitor = monitor;
  mTableWidget = new QTableWidget ( mVariableMonitor->getNumOfVariables(), 3,
                                    this );
  mMainLayout->addWidget ( mTableWidget );

  mTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
  mTableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
  mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  mTableWidget->setSortingEnabled(true);
  mTableWidget->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( "Type" ) );
  mTableWidget->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( "Variable" ) );
  mTableWidget->setHorizontalHeaderItem ( 2, new QTableWidgetItem ( "Value" ) );
}
//-----------------------------------------------------------------------------
CVariableMonitorWidget::~CVariableMonitorWidget()
{
}
//-----------------------------------------------------------------------------
void CVariableMonitorWidget::updateData()
{
  QFont font;
  std::string value;
  std::string name;
  std::string varType;
  QTableWidgetItem* tableItem;

  if ( isChecked() ) {
    mTableWidget->setHidden ( false );

    mTableWidget->setSortingEnabled(false);
    for ( unsigned int i = 0; i < mVariableMonitor->getNumOfVariables(); i++ ) {
      mVariableMonitor->getVariableString ( i, varType, name, value );
      tableItem = mTableWidget->item ( i, 0 );
      // variable type
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        //tableItem->setFlags(0);
        tableItem->setForeground ( QBrush ( QColor ( 128,0,0 ) ) );
        font = tableItem->font();
        font.setBold ( true );
        tableItem->setFont ( font );
        mTableWidget->setItem ( i, 0, tableItem );
      }
      tableItem->setText ( varType.c_str() );

      // variable name
      tableItem = mTableWidget->item ( i, 1 );
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        //tableItem->setFlags(0);
        mTableWidget->setItem ( i, 1, tableItem );
      }
      tableItem->setText ( name.c_str() );

      // variable value
      tableItem = mTableWidget->item ( i, 2 );
      if ( tableItem == NULL ) {
        tableItem = new QTableWidgetItem();
        //tableItem->setFlags(0);
        mTableWidget->setItem ( i, 2, tableItem );
      }
      tableItem->setText ( value.c_str() );
    } // for
    mTableWidget->setSortingEnabled(true);
  }
  else {
    mTableWidget->setHidden ( true );
  }

  // always hide general info box, because we don't need it
  mGeneralInfoBox->setHidden ( true );
}
//-----------------------------------------------------------------------------
} // namespace
