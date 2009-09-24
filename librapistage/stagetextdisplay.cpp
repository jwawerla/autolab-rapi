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

#include "stagetextdisplay.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CStageTextDisplay::CStageTextDisplay ( Stg::ModelPosition* stgModel,
                                       std::string devName )
    : ATextDisplay ( devName )
{
  mStgPosition = stgModel;
  mFgEnabled = false;

  setEnabled( true );
}
//-----------------------------------------------------------------------------
CStageTextDisplay::~CStageTextDisplay()
{
}
//-----------------------------------------------------------------------------
int CStageTextDisplay::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageTextDisplay::setEnabled ( bool enable )
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void CStageTextDisplay::updateData( const double dt )
{
  mTimeStamp = mStgPosition->GetWorld()->SimTimeNow() / 1e6;
}
//-----------------------------------------------------------------------------
int CStageTextDisplay::setText ( std::string text )
{
  mText = text;

  if ( mStgPosition ) {
    mTimeStamp = mStgPosition->GetWorld()->SimTimeNow() / 1e6;
    mStgPosition->Say ( mText.c_str() );
    return 1; // success;
  }

  return 0; // failure;
}
//-----------------------------------------------------------------------------
int CStageTextDisplay::getSize() const
{
  return 256;
}
//-----------------------------------------------------------------------------

} // namespace
