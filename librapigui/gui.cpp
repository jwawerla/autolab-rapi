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
#include "gui.h"
#include "mainwindow.h"
#include <stdio.h>


namespace Rapi
{

/** Only instance of this class */
CGui* cguiInstance = NULL;
/** Qt Application */
QApplication* mQtApp = NULL;
/** Main window */
CMainWindow* mMainWindow = NULL;

//-----------------------------------------------------------------------------
CGui::CGui ( int argc, char* argv[] )
{
  mArgc = argc;
  mArgv = mArgv;
  pthread_create ( &mPThread, NULL, &threadMain, this );

  sleep(2);  // need to wait for thread to finish creating the Qt App
}
//-----------------------------------------------------------------------------
CGui::~CGui()
{
}
//-----------------------------------------------------------------------------
CGui* CGui::getInstance ( int argc, char* argv[] )
{
  if ( cguiInstance == NULL )
    cguiInstance = new CGui ( argc, argv );

  return cguiInstance;
}
//-----------------------------------------------------------------------------
void* CGui::threadMain ( void* arg )
{
  CGui* gui = (CGui*)arg;
  mQtApp =  new QApplication ( gui->mArgc, gui->mArgv );
  mQtApp->setStyle("plastique");
  mQtApp->setOrganizationName("Autolab");
  mQtApp->setOrganizationDomain("autolab.cmpt.sfu.ca");
  mQtApp->setApplicationName("RapiGui");
  mMainWindow = new CMainWindow();
  mMainWindow->show();
  mQtApp->exec();

  return NULL;
}
//-----------------------------------------------------------------------------
void CGui::registerRobot(ARobot* robot)
{
  mMainWindow->addRobot(robot);
}
//-----------------------------------------------------------------------------

} // namespace
