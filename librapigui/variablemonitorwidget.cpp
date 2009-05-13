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
#include <QHBoxLayout>

namespace Rapi
{

//-----------------------------------------------------------------------------
CVariableMonitorWidget::CVariableMonitorWidget ( CVariableMonitor* monitor,
    QWidget* parent ) : QGroupBox ( parent )
{
  QHBoxLayout* layout;

  setTitle ( "Monitor" );
  layout = new QHBoxLayout ( this );

  mVariableMonitor = monitor;
  mTableWidget = new QTableWidget ( mVariableMonitor->getNumOfVariables(), 2, this );
  layout->addWidget ( mTableWidget );

  mTableWidget->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( "Variable" ) );
  mTableWidget->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( "Value" ) );
  setLayout ( layout );
}
//-----------------------------------------------------------------------------
CVariableMonitorWidget::~CVariableMonitorWidget()
{
}
//-----------------------------------------------------------------------------
void CVariableMonitorWidget::updateData()
{
  std::string value;
  std::string name;
  QTableWidgetItem* tableItem;

  for ( unsigned int i = 0; i < mVariableMonitor->getNumOfVariables(); i++ ) {
    mVariableMonitor->getVariableString ( i, value, name );
    tableItem = mTableWidget->item ( i, 0 );
    if ( tableItem == NULL ) {
      tableItem = new QTableWidgetItem();
      mTableWidget->setItem ( i, 0, tableItem );
    }
    tableItem->setText ( name.c_str() );

    tableItem = mTableWidget->item ( i,1 );
    if ( tableItem == NULL ) {
      tableItem = new QTableWidgetItem();
      mTableWidget->setItem ( i, 1, tableItem );
    }
    tableItem->setText ( value.c_str() );
  }
}
//-----------------------------------------------------------------------------
} // namespace
