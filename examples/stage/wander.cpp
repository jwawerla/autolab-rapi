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

#include "RapiLooseStage"

#ifdef RAPI_GUI
#include "RapiGui"
#endif

#include "wanderctrl.h"

using namespace Rapi;

extern "C" int Init ( Stg::Model* mod )
{
#ifdef RAPI_GUI
  CGui* gui = CGui::getInstance ( 0, NULL );
#endif

  printf ( "-----------------------------------\n" );
  printf ( "Wander RAPI demo \n" );
  printf ( "  build %s %s \n", __DATE__, __TIME__ );
  printf ( "  compiled against RAPI version %s (%s) build %s\n", RAPI_VERSION(),
           RAPI_GIT_VERSION(), RAPI_BUILD() );
  printf ( "\n" );

  CLooseStageRobot* robot;
  ARobotCtrl* robotCtrl;

  // init general stuff
  ErrorInit ( 1, false );
  initRandomNumberGenerator();

  // create robot and its controller
  robot = new CLooseStageRobot ( mod );
  robotCtrl = new CWanderCtrl ( robot );

#ifdef RAPI_GUI
  gui->registerRobot ( robot );
#endif
  return 0; // ok
}
