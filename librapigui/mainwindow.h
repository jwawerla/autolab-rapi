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
#ifndef RAPIMAINWINDOW_H
#define RAPIMAINWINDOW_H

#include <QtGui>
#include <list>
#include "robot.h"
#include "devicewidgetlist.h"

namespace Rapi
{

/**
 * Main window of the gui
 * @author Jens Wawerla
*/
class CMainWindow : public QMainWindow
{

  Q_OBJECT

  public:
    /** Default constructor */
    CMainWindow();
    /** Default destructor */
    ~CMainWindow();
    /**
     * Adds a robot to be displayed
     * @param robot to be added
     */
    void addRobot ( ARobot* robot );
    /** Rangefinder list */
    CDeviceWidgetList* mRangeFinderWidgetList;
    /** Drivetrain list */
    CDeviceWidgetList* mDrivetrainWidgetList;
    /** Fiducialfinder list */
    CDeviceWidgetList* mFiducialFinderWidgetList;
    /** Powerpack list */
    CDeviceWidgetList* mPowerPackWidgetList;
    /** Textdisplay list */
    CDeviceWidgetList* mTextDisplayWidgetList;
    /** Variable monitor list */
    CDeviceWidgetList* mVariableMonitorWidgetList;
    /** Console list */
    CDeviceWidgetList* mConsoleWidgetList;

  protected slots:
    void update();

  protected:
    /** Qt close event */
    void closeEvent(QCloseEvent* event);
    /** List of robots to be displayed */
    std::vector<ARobot*> mRobotVector;
    /** Tab widget for robot */
    QTabWidget* mTabWidget;
    /** Timer to update the gui */
    QTimer* mTimer;
    /** Number of robots */
    unsigned int mNumRobots;
    /** Flags if we are initialized or not */
    bool mFgInit;
    /** Show device menu */
    QMenu* mDeviceMenu;

};

} // namespace

#endif
