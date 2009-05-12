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

namespace Rapi {

//-----------------------------------------------------------------------------
CMainWindow::CMainWindow()
 : QMainWindow()
{
  mNumRobots = 0;
  mFgInit = false;

  setWindowTitle("RapiGui");

  mTabWidget = new QTabWidget(this);
  setCentralWidget( mTabWidget );

  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
  mTimer->setSingleShot(false);
  mTimer->start(100); // msec
}
//-----------------------------------------------------------------------------
CMainWindow::~CMainWindow()
{
}
//-----------------------------------------------------------------------------
void CMainWindow::update()
{
  ARobot* robot;
  CRobotWidget* robotWidget;

  if (mFgInit == false) {
    mFgInit = true;
    mTimer->start(100); // msec
    return;
  }
  // add additional robots if necessary
  while (mNumRobots != mRobotVector.size() ) {
    robot = mRobotVector[mNumRobots];
    robotWidget = new CRobotWidget(robot, mTabWidget);
    mTabWidget->addTab(robotWidget, robot->getName().c_str() );
    mNumRobots ++;
  }

  robotWidget = (CRobotWidget*)mTabWidget->currentWidget();
  if (robotWidget)
    robotWidget->update();

  QMainWindow::update();
}
//-----------------------------------------------------------------------------
void CMainWindow::addRobot ( ARobot* robot )
{

  assert(robot);
  mRobotVector.push_back(robot);
}
//-----------------------------------------------------------------------------
} // namespace
