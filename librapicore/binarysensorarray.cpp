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
#include "binarysensorarray.h"

namespace Rapi {

//-----------------------------------------------------------------------------
ABinarySensorArray::ABinarySensorArray(std::string devName)
 : ADevice(devName)
{
  mBitData = NULL;
  mBitPose = NULL;
  mFgAnyTriggered = false;
}
//-----------------------------------------------------------------------------
ABinarySensorArray::~ABinarySensorArray()
{
  if (mBitData)
    delete[] mBitData;

  if (mBitPose)
    delete[] mBitPose;
}
//-----------------------------------------------------------------------------
bool ABinarySensorArray::isAnyTriggered()
{
  return mFgAnyTriggered;
}
//-----------------------------------------------------------------------------
void ABinarySensorArray::print()
{
  printf("ABinarySensorArray: ");

  for (unsigned int i = 0; i < mNumSamples; i++) {
    if (mBitData[i] )
      printf("%d:on ", i);
    else
      printf("%d:off ", i);
  }
  printf("\n");

}
//-----------------------------------------------------------------------------
} // namespace
