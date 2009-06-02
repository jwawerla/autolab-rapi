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

#include "RapiChatterbox"
#include "chatterboxctrl.h"
#include <signal.h>
#include <string.h>

Rapi::CCBRobot* robot = NULL;
CChatterboxCtrl* robotCtrl = NULL;

//-----------------------------------------------------------------------------
void quitSig(int signum)
{
  PRT_MSG0(4,"User requested ctrl-c");

  // set default signal handler
  if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
     PRT_ERR1("Error resetting signal handler %s", strerror(errno));
  }

  // terminate main thread
  robot->terminate();

  // clean up robot controller
  if (robotCtrl) {
    delete robotCtrl;
    robotCtrl = NULL;
  }

  // clean up robot
  if (robot)
    delete (robot);
}
//------------------------------------------------------------------------------

int main( int argc, char* argv[] )
{
  // init general stuff
  ErrorInit ( 4, false );
  initRandomNumberGenerator();

  if (signal(SIGINT, quitSig) == SIG_ERR) {
     PRT_ERR1("Error resetting signal handler %s", strerror(errno));
  }

  // create robot and its controller
  robot = new Rapi::CCBRobot ();
  if ( robot->init() == 0) {
    Rapi::rapiError->print();
    exit(-1);
  }
  robotCtrl = new CChatterboxCtrl ( robot );

  // blocking call
  robot->run();
  return 1;
}
