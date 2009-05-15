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
#ifndef RAPIDEVICEWIDGETLIST_H
#define RAPIDEVICEWIDGETLIST_H

#include <list>
#include <QtGui>
#include "devicewidget.h"

namespace Rapi
{

/**
 * @author Jens Wawerla
 */
class CDeviceWidgetList : public QObject
{
Q_OBJECT

  public:
    /** Default constructor */
    CDeviceWidgetList ( QMenu* menu, QString name, QObject* parent );
    /** Default destructor */
    ~CDeviceWidgetList();
    /**
     * Add a device widget
     * @param widget to add
     */
    void addWidget ( ADeviceWidget* widget );
    /**
     * Writes the current settings to file
     */
    void writeSettings();

  public slots:
    void checkWidgets(bool checked);

  protected:
    /** List of device widgets */
    std::list<ADeviceWidget*> mWidgetList;
    /** Menu */
    QMenu* mMenu;
    /** Action */
    QAction* mAction;
    /** Name of device widgets */
    QString mName;
};

} // namespace

#endif
