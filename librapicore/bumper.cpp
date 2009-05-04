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
#include "bumper.h"

namespace Rapi {

//-----------------------------------------------------------------------------
ABumper::ABumper(std::string devName)
 : ADevice(devName)
{
  mBumper = NULL;
  mBumperPose = NULL;
  mFgAnyTriggered = false;
}
//-----------------------------------------------------------------------------
ABumper::~ABumper()
{
  if (mBumper)
    delete[] mBumper;

  if (mBumperPose)
    delete[] mBumperPose;
}
//-----------------------------------------------------------------------------
bool ABumper::isAnyTriggered()
{
  return mFgAnyTriggered;
}
//-----------------------------------------------------------------------------
void ABumper::print()
{
  printf("ABumper: ");

  for (unsigned int i = 0; i < mNumSamples; i++) {
    if (mBumper[i] )
      printf("%d:on ", i);
    else
      printf("%d:off ", i);
  }
  printf("\n");

}
//-----------------------------------------------------------------------------
} // namespace
