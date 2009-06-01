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
#include "switcharray.h"
#include "printerror.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
ASwitchArray::ASwitchArray ( std::string devName )
    : ADevice ( devName )
{
  mNumSwitches = 0;
  mSwitch = NULL;
  mFgEnabled = false;
}
//-----------------------------------------------------------------------------
ASwitchArray::~ASwitchArray()
{
  if ( mSwitch ) {
    delete[] mSwitch;
    mSwitch = NULL;
  }
}
//-----------------------------------------------------------------------------
void ASwitchArray::setSwitch ( unsigned int id, bool on )
{
  if (id < mNumSwitches)
    mSwitch[id] = on;
  else
    PRT_WARN2("Switch %d requested but only have %d switches", id, mNumSwitches);
}
//-----------------------------------------------------------------------------
bool ASwitchArray::getSwitch ( unsigned int id ) const
{
  if (id < mNumSwitches)
    return mSwitch[id];
  else
    PRT_WARN2("Switch %d requested but only have %d switches", id, mNumSwitches);

  return false;
}
//-----------------------------------------------------------------------------
unsigned int ASwitchArray::getNumSwitches() const
{
  return mNumSwitches;
}
//-----------------------------------------------------------------------------
void ASwitchArray::print() const
{
  printf ( "ASwitchArray: " );
  for ( unsigned int i =0; i < mNumSwitches; i++ ) {
    if ( mSwitch[i] )
      printf ( "sw %d ON ", i );
    else
      printf ( "sw %d OFF ", i );
  }
  printf ( "\n" );
}
//-----------------------------------------------------------------------------
} // namespace

