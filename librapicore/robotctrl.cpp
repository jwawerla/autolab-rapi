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

#include "robotctrl.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>

namespace Rapi {

//-----------------------------------------------------------------------------
ARobotCtrl::ARobotCtrl( ARobot* robot ) : IRobotUpdate()
{
  assert( robot );
  mRobot = robot;
  mRobot->registerRobotController( this );
}
//-----------------------------------------------------------------------------
ARobotCtrl::~ARobotCtrl()
{
}
//-----------------------------------------------------------------------------
ARobot* ARobotCtrl::getRobot() const
{
  return mRobot;
}
//-----------------------------------------------------------------------------
void ARobotCtrl::rprintf ( const char* format, ... )
{
  //std::ostringstream timeStrOut;
  va_list args;
  char str[256];

  if (mRPrintfString.length() > 1024)
    mRPrintfString.clear();

  fprintf ( stdout, "\033[1;37;42m[%s]\033[0m: ", mRobot->getName().c_str() );
  va_start ( args, format );
  vfprintf ( stdout, format, args );
  va_end ( args );

  va_start ( args, format );
  vsprintf ( str, format, args );
  va_end ( args );
  //timeStrOut << mRobot->getCurrentTime();
  //mRPrintfString.append("("+timeStrOut.str()+") " );
  mRPrintfString.append(str);
}
//-----------------------------------------------------------------------------
} // namespace
