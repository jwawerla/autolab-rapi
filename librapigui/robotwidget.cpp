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
#include "robotwidget.h"
#include "drivetrain2dofwidget.h"
#include "rangefinderwidget.h"
#include "fiducialfinderwidget.h"
#include "powerpackwidget.h"
#include "textdisplaywidget.h"
#include "printerror.h"
#include "robotctrl.h"
#include <QVBoxLayout>

namespace Rapi {

//-----------------------------------------------------------------------------
CRobotWidget::CRobotWidget(ARobot* robot, CMainWindow* mw, QWidget* parent)
 : QWidget( parent)
{
  ARobotCtrl* ctrl;
  ADeviceWidget* widget;
  ADevice* device;
  mRobot = robot;

  mMainWindow = mw;
  QVBoxLayout* layout = new QVBoxLayout (this );

  for ( unsigned int i = 0; i < mRobot->getNumOfDevices(); i++) {
    device = mRobot->getDeviceByIndex(i);
    if ( device->getGuiName() == "nogui") {

    }
    else if ( device->getGuiName() == "DrivetrainWidget" ) {
      widget = new CDrivetrain2dofWidget((ADrivetrain2dof*)device, this);
      mMainWindow->mDrivetrainWidgetList->addWidget(widget);
      layout->addWidget( widget);
      mWidgetList.push_back(widget);
    }
    else if ( device->getGuiName() == "RangeFinderWidget" ) {
      widget = new CRangeFinderWidget((ARangeFinder*)device, this);
      mMainWindow->mRangeFinderWidgetList->addWidget(widget);
      layout->addWidget( widget);
      mWidgetList.push_back(widget);
    }
    else if ( device->getGuiName() == "PowerPackWidget" ) {
      widget = new CPowerPackWidget((APowerPack*)device, this);
      mMainWindow->mPowerPackWidgetList->addWidget(widget);
      layout->addWidget( widget);
      mWidgetList.push_back(widget);
    }
    else if ( device->getGuiName() == "FiducialFinderWidget" ) {
      widget = new CFiducialFinderWidget((AFiducialFinder*)device, this);
      mMainWindow->mFiducialFinderWidgetList->addWidget(widget);
      layout->addWidget( widget);
      mWidgetList.push_back(widget);
    }
    else if ( device->getGuiName() == "TextDisplayWidget" ) {
      widget = new CTextDisplayWidget((ATextDisplay*)device, this);
      mMainWindow->mTextDisplayWidgetList->addWidget(widget);
      layout->addWidget( widget);
      mWidgetList.push_back(widget);
    }
    else {
      PRT_WARN1("Unkown gui name %s", device->getGuiName().c_str() );
    }
  }

  // Now add variable monitor
  mVariableMonitor = new CVariableMonitorWidget(&(mRobot->mVariableMonitor), this);
  layout->addWidget(mVariableMonitor);
  mMainWindow->mVariableMonitorWidgetList->addWidget(mVariableMonitor);

  // Add console widget
  ctrl = ((ARobotCtrl*)mRobot->getRobotController());
  mConsoleWidget = new CConsoleWidget(&(ctrl->mRPrintfString), this );
  layout->addWidget(mConsoleWidget);
  mMainWindow->mConsoleWidgetList->addWidget(mConsoleWidget);

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setLayout( layout );
}
//-----------------------------------------------------------------------------
CRobotWidget::~CRobotWidget()
{
  printf("CRobotWidget::~CRobotWidget()\n");
}
//-----------------------------------------------------------------------------
void CRobotWidget::update()
{
  ADeviceWidget* widget;
  std::list<ADeviceWidget*>::iterator it;

  for (it = mWidgetList.begin(); it != mWidgetList.end(); it++) {
    widget = (ADeviceWidget*) (*it);
    widget->updateData();
  }

  mVariableMonitor->updateData();
  mConsoleWidget->updateData();
  QWidget::update();
}
//-----------------------------------------------------------------------------
} // namespace
