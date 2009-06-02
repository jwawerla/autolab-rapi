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
#include "cblowsidedriver.h"
#include "printerror.h"
#include <assert.h>

namespace Rapi
{

//-----------------------------------------------------------------------------
CCBLowSideDriver::CCBLowSideDriver ( CCBDriver* driver, std::string devName )
    : ASwitchArray ( devName )
{
  assert ( driver );
  mCBDriver = driver;
  mNumSwitches = 3;

  mSwitch = new bool[mNumSwitches];
  for (unsigned int i = 0; i < mNumSwitches; i++)
    mSwitch[i] = false;

  setEnabled ( true );
}
//-----------------------------------------------------------------------------
CCBLowSideDriver::~CCBLowSideDriver()
{
  mCBDriver->setLowSideDriver ( 0, false );
  mCBDriver->setLowSideDriver ( 1, false );
  mCBDriver->setLowSideDriver ( 2, false );

  if ( mSwitch ) {
    delete[] mSwitch;
    mSwitch = NULL;
  }
}
//-----------------------------------------------------------------------------
int CCBLowSideDriver::init()
{
  return 1; // nothing to do
}
//-----------------------------------------------------------------------------
void CCBLowSideDriver::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void CCBLowSideDriver::updateData()
{
  // nothing to do
}
//-----------------------------------------------------------------------------
void CCBLowSideDriver::setSwitch ( unsigned int id, bool on )
{
  if ( mFgEnabled ) {
    if ( id < mNumSwitches ) {
      if (mSwitch[id] != on) {
        mSwitch[id] = on;
        mCBDriver->setLowSideDriver ( id, on );
      }
    }
    else
      PRT_WARN2 ( "Switch %d requested but only have %d switches", id, mNumSwitches );
  }
}
//-----------------------------------------------------------------------------
} // namespace
