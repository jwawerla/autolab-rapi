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
  mNumLights = 5;

  mDot = DOT_OFF;

  for (unsigned int i =0; i < mNumLights; i++ ) {
    mBlink[i].on = true;
    mBlink[i].enabled = true;
    mBlink[i].freq = 1;
    mBlink[i].tick = 0;
    mBlink[i].color = CRgbColor(0, 0, 0);
  }

  mDotBlink.on = false;
  mDotBlink.enabled = false;
  mDotBlink.freq = 1;
  mDotBlink.tick = 0;
}
//---------------------------------------------------------------------------
CCBLights::~CCBLights()
{
}
//---------------------------------------------------------------------------
void CCBLights::setEnabled ( bool enable )
{
  if ( enable == true ) {
    mFgEnabled = true;
  } else {
    mFgEnabled = false;
    for ( unsigned char i = 0; i < mNumLights; i++ )
      mCBDriver->setRgbLed ( i, CRgbColor(0, 0, 0) );
  }
}
//----------------------------------------------------------------------------
int CCBLights::init()
{
  return 1;
}
//----------------------------------------------------------------------------
void CCBLights::updateData()
{
  if ( mFgEnabled ) {
    for ( unsigned int i = 0; i < mNumLights; i++ ) {
      if ( mBlink[i].enabled ) {
        mBlink[i].tick = mBlink[i].tick - 1;
        if ( mBlink[i].tick <= 0 ) {

          mBlink[i].tick = ( int ) ( 0.5/ ( mBlink[i].freq * CB_T ) );
          if ( mBlink[i].on == true ) {
            mBlink[i].on = false;
            mCBDriver->setRgbLed ( i, mBlink[i].color );
          } else {
            mBlink[i].on = true;
            mCBDriver->setRgbLed ( i, CRgbColor(0, 0, 0) );
          }
        }
      }
    } // for

    switch ( mDot ) {
      case DOT_OFF:
        mCBDriver->set7SegDot ( false );
        mDotBlink.on = false;
        break;

      case DOT_ON:
        mCBDriver->set7SegDot ( true );
        mDotBlink.on = true;
        break;

      case DOT_BLINK:
        mDotBlink.tick = mDotBlink.tick - 1;
        if ( mDotBlink.tick <= 0 ) {

          mDotBlink.tick = ( int ) ( 0.5/ ( mDotBlink.freq * CB_T ) );
          if ( mDotBlink.on == true ) {
            mDotBlink.on = false;
            mCBDriver->set7SegDot ( false );
          } else {
            mDotBlink.on = true;
            mCBDriver->set7SegDot ( true );
          }
        }
        break;

    }  // switch

    // update time stamp of this measurement
    mTimeStamp = timeStamp();
  } // enabled
}
//----------------------------------------------------------------------------
int CCBLights::set7SegDot ( t7SegDot dot )
{
  mDot = dot;

  return 1; // success
}
//----------------------------------------------------------------------------
int CCBLights::setBlink ( int id, bool enabled, float freq )
{

  if ( id == -1 ) {
    for ( unsigned int i =0; i < mNumLights; i++ ) {
      setBlink ( i, enabled, freq );
    }
    return 1;
  }

  if ( ( (unsigned int)id >= mNumLights ) || ( id < 0 ) ) {
    ERROR2 ( "Led id out of bounds %d [0..%d]",id, mNumLights-1 );
    return 0;
  }
  if ( freq == 0 ) {
    PRT_WARN1 ( "Blink frequency for led %d cannot be 0",id );
    return 0;
  }

  mBlink[id].enabled = enabled;
  if ( mBlink[id].freq != freq ) {
    mBlink[id].freq = freq;
    mBlink[id].tick = ( int ) ( 1.0 / ( mBlink[id].freq * CB_T ) );
  }

  return 1;
}
//----------------------------------------------------------------------------
int CCBLights::setLight ( int id, CRgbColor color )
{
  if ( id == -1 ) {
    for ( unsigned int i =0; i < mNumLights; i++ ) {
      setLight ( i, color );
    }
    return 1;
  }

  if ( ( (unsigned int)id >= mNumLights ) || ( id < 0 ) ) {
    ERROR2 ( "Led id out of bounds %d [0..%d]",id, mNumLights-1 );
    return 0;
  }

  mBlink[id].color = color;

  // set led rgb value only if not blinking, if blinking stuff is handled by
  // update()
  if ( mBlink[id].enabled == false )
    return mCBDriver->setRgbLed ( id, color );

  return 1;
}
//----------------------------------------------------------------------------
void CCBLights::print()
{
  printf ( "Lights: " );
  //for (int i = 0; i < mNumRbgLeds; i ++) {
  //  printf(" %d (%03d %03d %03d) ", i,
  printf ( "\n" );
}
//----------------------------------------------------------------------------
int CCBLights::setLight ( int id, tColor color )
{
  switch ( color ) {
    case RED:    return setLight ( id, CRgbColor(255,   0,   0) );
    case GREEN:  return setLight ( id, CRgbColor(  0, 255,   0) );
    case BLUE:   return setLight ( id, CRgbColor(  0,   0, 255) );
    case YELLOW: return setLight ( id, CRgbColor(255, 255,   0) );
    case WHITE:  return setLight ( id, CRgbColor(255, 255, 255) );
    case BLACK:  return setLight ( id, CRgbColor(  0,   0,   0) );

    default:
      ERROR1 ( "Unknown color %d ", color );
  }  // switch;

  return 0;
}
//----------------------------------------------------------------------------

} // namespace
