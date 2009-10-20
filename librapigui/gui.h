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
#ifndef RAPIGUI_H
#define RAPIGUI_H

#include "robot.h"
#include "customdialog.h"
#include <pthread.h>
#include <QtGui>
#include "mainwindow.h"

namespace Rapi
{

/**
 * Main entrance point for the gui
 * @author Jens Wawerla
 */
class CGui
{
  public:
    /** Default destructor */
    ~CGui();
    /**
     * Gets the only instance of this class
     * @return class instance
     */
    static CGui* getInstance( int argc, char* argv[] );
    /**
     * Registers a robot with this gui, so it can be visualized
     * @param robot to visualize
     */
    void registerRobot( ARobot* robot );
    /* GUI thread: needs to be public to shut down RPC controller correctly */
    pthread_t mPThread;

  protected:
    /** Default constructor */
    CGui( int argc=0, char* argv[]=NULL );
    /** Main for the thread */
    static void* threadMain( void* arg );
    /** Main window */
    CMainWindow* mMainWindow;

  private:
    /** Command line parameters for QT */
    int mArgc;
    char** mArgv;
};

}  // namespace

#endif
