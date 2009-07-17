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

/** Update interval [ms] */
const int UPDATE_INTERVAL = 100;

//-----------------------------------------------------------------------------
CMainWindow::CMainWindow()
    : QMainWindow()
{
  QSettings settings;
  QVBoxLayout* layout;
  mNumRobots = 0;
  mFgInit = false;

  setWindowTitle ( "RapiGui" );

  mStatusBarLabel = new QLabel ( this, Qt::FramelessWindowHint );
  mStatusBarLabel->setAutoFillBackground ( true );

  statusBar()->addWidget ( mStatusBarLabel, 2024 );
  mFileMenu = menuBar()->addMenu ( "&File" );
  mDeviceMenu = menuBar()->addMenu ( "&Devices" );

  mStartStopAction = new QAction ( "Start", this );
  mStartStopAction->setCheckable ( true );
  mStartStopAction->setChecked ( true );
  mFileMenu->addAction ( mStartStopAction );

  mQuitAction = new QAction ( "&Quit", this );
  mFileMenu->addAction ( mQuitAction );

  mRangeFinderWidgetList = new CDeviceWidgetList ( mDeviceMenu, "Rangefinder",
      this );
  mDrivetrainWidgetList = new CDeviceWidgetList ( mDeviceMenu,
      "Drivetrain", this );
  mFiducialFinderWidgetList = new CDeviceWidgetList ( mDeviceMenu,
      "Fiducialfinder", this );
  mPowerPackWidgetList = new CDeviceWidgetList ( mDeviceMenu, "Powerpack",
      this );
  mTextDisplayWidgetList = new CDeviceWidgetList ( mDeviceMenu, "Text Display",
      this );
  mVariableMonitorWidgetList = new CDeviceWidgetList ( mDeviceMenu,
      "Variable Monitor", this );
  mConsoleWidgetList = new CDeviceWidgetList ( mDeviceMenu,
      "Console", this );
  mBinarySensorArrayWidgetList = new CDeviceWidgetList ( mDeviceMenu,
      "Binary Sensor", this );

  mTabWidget = new QTabWidget ( this );
  layout = new QVBoxLayout ( mTabWidget );
  mTabWidget->setLayout ( layout );
  setCentralWidget ( mTabWidget );

  mTimer = new QTimer ( this );
  connect ( mTimer, SIGNAL ( timeout() ), this, SLOT ( update() ) );
  mTimer->setSingleShot ( false );

  connect ( mStartStopAction, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( startStop ( bool ) ) );
  connect ( mQuitAction, SIGNAL ( triggered() ), this, SLOT ( close() ) );

  resize ( settings.value ( "mainWindow/size" ).toSize() );
  move ( settings.value ( "mainWindow/pos" ).toPoint() );

  startStop ( true );
}
//-----------------------------------------------------------------------------
CMainWindow::~CMainWindow()
{
}
//-----------------------------------------------------------------------------
void CMainWindow::startStop ( bool checked )
{
  QPalette palette;

  if ( checked ) {
    palette.setColor ( QPalette::Background, Qt::green );
    mStatusBarLabel->setPalette ( palette );
    mStatusBarLabel->setText ( "Running..." );
    mTimer->start ( UPDATE_INTERVAL ); // msec
  }
  else {
    mTimer->stop();
    palette.setColor ( QPalette::Background, Qt::red );
    mStatusBarLabel->setPalette ( palette );
    mStatusBarLabel->setText ( "Paused" );
  }
}
//-----------------------------------------------------------------------------
void CMainWindow::closeEvent ( QCloseEvent* event )
{
  QSettings settings;

  event->accept();

  settings.setValue ( "mainWindow/size", size() );
  settings.setValue ( "mainWindow/pos", pos() );
  mRangeFinderWidgetList->writeSettings();
  mDrivetrainWidgetList->writeSettings();
  mVariableMonitorWidgetList->writeSettings();
  mConsoleWidgetList->writeSettings();
  mFiducialFinderWidgetList->writeSettings();
  mTextDisplayWidgetList->writeSettings();
  mPowerPackWidgetList->writeSettings();
  mBinarySensorArrayWidgetList->writeSettings();

  for ( unsigned int i = 0; i < mDialogList.size(); i++ ) {
    mDialogList[i]->close();
  }
}
//-----------------------------------------------------------------------------
void CMainWindow::update()
{
  ARobot* robot;
  QScrollArea* scrollArea = NULL;
  CRobotWidget* robotWidget;

  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );

  if ( mFgInit == false ) {
    mFgInit = true;
    mTimer->start ( 100 ); // msec
    for ( unsigned int i = 0; i < mDialogList.size(); i++ ) {
      mDialogList[i]->show();
      mDialogList[i]->setModal(false);
    }
    return;
  }

  // add additional robots if necessary
  while ( mNumRobots != mRobotVector.size() ) {
    robot = mRobotVector[mNumRobots];
    scrollArea = new QScrollArea ( mTabWidget );
    scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    scrollArea->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
    robotWidget = new CRobotWidget ( robot, this, scrollArea );
    mTabWidget->addTab ( scrollArea, robot->getName().c_str() );
    scrollArea->setWidget ( robotWidget );
    scrollArea-> setWidgetResizable ( true );
    //resize ( scrollArea->sizeHint() );
    mNumRobots ++;
  }

  scrollArea = ( QScrollArea* ) mTabWidget->currentWidget();
  if ( scrollArea ) {
    robotWidget = ( CRobotWidget* ) scrollArea->widget();
    if ( robotWidget )
      robotWidget->update();
  }

  mConsoleWidgetList->updateData();

  for ( unsigned int i = 0; i < mDialogList.size(); i++ )
    mDialogList[i]->update();

  QMainWindow::update();
}
//-----------------------------------------------------------------------------
void CMainWindow::addRobot ( ARobot* robot )
{
  assert ( robot );
  mRobotVector.push_back ( robot );
}
//-----------------------------------------------------------------------------
void CMainWindow::addCustomDialog ( CCustomDialog* dialog )
{
  if ( dialog ) {
    mDialogList.push_back ( dialog );
  }
}
//-----------------------------------------------------------------------------

} // namespace

