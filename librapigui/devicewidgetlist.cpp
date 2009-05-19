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
#include "devicewidgetlist.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CDeviceWidgetList::CDeviceWidgetList ( QMenu* menu, QString name, QObject* parent )
    : QObject ( parent )
{
  QSettings settings;

  mMenu = menu;
  mName = name;

  mAction = new QAction ( name, this );
  mAction->setCheckable ( true );
  mMenu->addAction ( mAction );
  connect ( mAction, SIGNAL ( toggled ( bool ) ), this,
                     SLOT ( checkWidgets ( bool ) ) );

  mAction->setChecked( settings.value(mName+"/checked").toBool() );
}
//-----------------------------------------------------------------------------
CDeviceWidgetList::~CDeviceWidgetList()
{
}
//-----------------------------------------------------------------------------
void CDeviceWidgetList::checkWidgets ( bool checked )
{
  ADeviceWidget* widget;

  std::list<ADeviceWidget*>::iterator it;

  for ( it = mWidgetList.begin(); it != mWidgetList.end(); it++ ) {
    widget = *it;
    widget->setChecked ( checked );
  } // for
}
//-----------------------------------------------------------------------------
void CDeviceWidgetList::addWidget ( ADeviceWidget* widget )
{
  mWidgetList.push_back ( widget );
  widget->setChecked(mAction->isChecked());
}
//-----------------------------------------------------------------------------
void CDeviceWidgetList::writeSettings()
{
  QSettings settings;

  settings.setValue(mName+"/checked", mAction->isChecked());
}
//-----------------------------------------------------------------------------
void CDeviceWidgetList::updateData()
{
  ADeviceWidget* widget;

  std::list<ADeviceWidget*>::iterator it;

  for ( it = mWidgetList.begin(); it != mWidgetList.end(); it++ ) {
    widget = *it;
    widget->updateData();
  } // for
}
//-----------------------------------------------------------------------------
} // namespace
