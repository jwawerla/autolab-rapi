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
#include "mainwindow.h"
#include "robotwidget.h"
#include "assert.h"
#include <QScrollArea>

namespace Rapi
{

//-----------------------------------------------------------------------------
CMainWindow::CMainWindow()
    : QMainWindow()
{
  QVBoxLayout* layout;
  mNumRobots = 0;
  mFgInit = false;

  setWindowTitle ( "RapiGui" );

  mDeviceMenu = menuBar()->addMenu("&Devices");

  mTabWidget = new QTabWidget ( this );
  layout = new QVBoxLayout( mTabWidget );
  mTabWidget->setLayout(layout);
  setCentralWidget ( mTabWidget );

  mTimer = new QTimer ( this );
  connect ( mTimer, SIGNAL ( timeout() ), this, SLOT ( update() ) );
  mTimer->setSingleShot ( false );
  mTimer->start ( 100 ); // msec
}
//-----------------------------------------------------------------------------
CMainWindow::~CMainWindow()
{
  printf("CMainWindow::~CMainWindow()\n");
}
//-----------------------------------------------------------------------------
void CMainWindow::closeEvent(QCloseEvent* event)
{
  printf("CMainWindow::closeEvent()\n");
  event->accept();

  QSettings settings(this);


printf("applicationName %s\n", settings.applicationName().toLatin1().constData () );
  settings.setValue("mainWindow/size", size());
}
//-----------------------------------------------------------------------------
void CMainWindow::update()
{
  ARobot* robot;
  QScrollArea* scrollArea = NULL;
  CRobotWidget* robotWidget;

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  if ( mFgInit == false ) {
    mFgInit = true;
    mTimer->start ( 100 ); // msec
    return;
  }
  // add additional robots if necessary
  while ( mNumRobots != mRobotVector.size() ) {
    robot = mRobotVector[mNumRobots];
    scrollArea = new QScrollArea(mTabWidget);
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    robotWidget = new CRobotWidget(robot, scrollArea);
    mTabWidget->addTab ( scrollArea, robot->getName().c_str() );
    scrollArea->setWidget(robotWidget);
    scrollArea-> setWidgetResizable(true);
    resize(scrollArea->sizeHint());
    mNumRobots ++;
  }

  scrollArea = ( QScrollArea* ) mTabWidget->currentWidget();
  if ( scrollArea ) {
    robotWidget = ( CRobotWidget* ) scrollArea->widget();
    if ( robotWidget )
      robotWidget->update();
  }
  QMainWindow::update();
}
//-----------------------------------------------------------------------------
void CMainWindow::addRobot ( ARobot* robot )
{

  assert ( robot );
  mRobotVector.push_back ( robot );
}
//-----------------------------------------------------------------------------
} // namespace
