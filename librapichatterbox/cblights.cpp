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

#include "cblights.h"
#include "rapierror.h"
#include "printerror.h"
#include "utilities.h"

namespace Rapi
{



//---------------------------------------------------------------------------
CCBLights::CCBLights ( CCBDriver* cbDriver, std::string devName )
  : ALights ( devName )
{
  mCBDriver = cbDriver;

  setEnabled ( true );
  mNumLights = 6;

  for ( unsigned int i =0; i < mNumLights; i++ ) {
    mBlink[i].on = true;
    mBlink[i].enabled = false;
    mBlink[i].freq = 1.0f;
    mBlink[i].tick = 0;
    mBlink[i].color = CRgbColor ( 0, 0, 0 );
  }

}
//---------------------------------------------------------------------------
CCBLights::~CCBLights()
{
  // when the music is over - turn off the lights
  for ( unsigned char i = 0; i < mNumLights-1; i++ )
    mCBDriver->setRgbLed ( i, CRgbColor ( 0, 0, 0 ) );
}
//---------------------------------------------------------------------------
void CCBLights::setEnabled ( bool enable )
{
  if ( enable == true ) {
    mFgEnabled = true;
  }
  else {
    mFgEnabled = false;
    for ( unsigned char i = 0; i < mNumLights-1; i++ )
      mCBDriver->setRgbLed ( i, CRgbColor ( 0, 0, 0 ) );
  }
}
//----------------------------------------------------------------------------
int CCBLights::init()
{
  return 1;
}
//----------------------------------------------------------------------------
void CCBLights::updateData( const double dt)
{
  mUpdateInterval = dt;

  if ( mFgEnabled ) {
    for ( unsigned int i = 0; i < mNumLights-1; i++ ) {
      if ( mBlink[i].enabled ) {
        mBlink[i].tick = mBlink[i].tick - 1;
        if ( mBlink[i].tick <= 0 ) {

          mBlink[i].tick = ( int ) ( 0.5/ ( mBlink[i].freq * dt ) );
          if ( mBlink[i].on == true ) {
            mBlink[i].on = false;
            mCBDriver->setRgbLed ( i, mBlink[i].color );
          }
          else {
            mBlink[i].on = true;
            mCBDriver->setRgbLed ( i, CRgbColor ( 0, 0, 0 ) );
          }
        }
      }
    } // for

    mBlink[DOT].tick = mBlink[DOT].tick - 1;
    if ( mBlink[DOT].tick <= 0 ) {

      mBlink[DOT].tick = ( int ) ( 0.5/ ( mBlink[DOT].freq * dt ) );
      if ( mBlink[DOT].on == true ) {
        mBlink[DOT].on = false;
        mCBDriver->set7SegDot ( false );
      }
      else {
        mBlink[DOT].on = true;
        mCBDriver->set7SegDot ( true );
      }
    }

    // update time stamp of this measurement
    mTimeStamp = timeStamp();
  } // enabled
}
//----------------------------------------------------------------------------
int CCBLights::setBlink ( int id, bool enabled, float freq )
{

  if ( id == -1 ) {
    for ( unsigned int i = 0; i < mNumLights; i++ ) {
      setBlink ( i, enabled, freq );
    }
    return 1;
  }

  if ( ( ( unsigned int ) id >= mNumLights ) || ( id < 0 ) ) {
    ERROR2 ( "Led id out of bounds %d [0..%d]",id, mNumLights-1 );
    return 0;
  }
  if ( freq == 0 ) {
    PRT_WARN1 ( "Blink frequency for led %d cannot be 0",id );
    return 0; // failure
  }

  mBlink[id].enabled = enabled;
  if ( mBlink[id].freq != freq ) {
    mBlink[id].freq = freq;
    mBlink[id].tick = ( int ) ( 1.0 / ( mBlink[id].freq * mUpdateInterval ) );
  }

  return 1;
}
//----------------------------------------------------------------------------
int CCBLights::setLight ( int id, CRgbColor color )
{
  if ( id == ALL_LIGHTS ) {
    for ( unsigned int i = 0; i < mNumLights-1; i++ ) {
      setLight ( i, color );
    }
    return 1;
  }

  if ( ( ( unsigned int ) id >= mNumLights ) || ( id < 0 ) ) {
    ERROR2 ( "Led id out of bounds %d [0..%d]",id, mNumLights-1 );
    return 0; // failure
  }

  mBlink[id].color = color;

  // set led rgb value only if not blinking, if blinking stuff is handled by
  // updateData()
  if ( mBlink[id].enabled == false ) {
    if ( ( unsigned int ) id < mNumLights-1 ) {
      return mCBDriver->setRgbLed ( id, color );
    }
    else {
      if ( color == CRgbColor ( 0,0,0 ) )
        return mCBDriver->set7SegDot ( false );
      else
        return mCBDriver->set7SegDot ( true );
    }
  }
  return 1;
}
//----------------------------------------------------------------------------
void CCBLights::print() const
{
  printf ( "CCBLights: " );
  for ( unsigned int i = 0; i < mNumLights; i ++ ) {
    printf ( " %d  ", i );
    mBlink[i].color.print();
  }
  printf ( "\n" );
}
//----------------------------------------------------------------------------


} // namespace
