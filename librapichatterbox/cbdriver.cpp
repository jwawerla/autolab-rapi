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

#include "cbdriver.h"
#include "cbdefinitions.h"
#include "robostix_drv.h"
#include "utilities.h"
#include "rapierror.h"
#include "printerror.h"
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/poll.h>

extern "C"
{
#include "i2c-io-api.h"
#include "i2c-api.h"
}

namespace Rapi
{

typedef enum {RED_CH, GREEN_CH, BLUE_CH} tLed;

// TODO: add singleton
//-----------------------------------------------------------------------------
CCBDriver::CCBDriver()
{
  mFd = 0;
  mLowSideDriverStatus = 0;
  m7SegByteValue = 0;
  mEstDistance = 0.0;
  mEstAngle = 0.0;
  mAccDistanceCmd = 0.0;
  mAccAngleCmd = 0.0;
  // ensure 'commanded velocity' is zero
  mCreateSensorPackage.rightWheelVelocity = 0;
  mCreateSensorPackage.leftWheelVelocity = 0;
}
//--------------------------------------------------------------------------
CCBDriver::~CCBDriver()
{
  // disable lights
  setRgbLed ( 0, CRgbColor ( 0, 0, 0 ) );
  setRgbLed ( 1, CRgbColor ( 0, 0, 0 ) );
  setRgbLed ( 2, CRgbColor ( 0, 0, 0 ) );
  setRgbLed ( 3, CRgbColor ( 0, 0, 0 ) );
  setRgbLed ( 4, CRgbColor ( 0, 0, 0 ) );

  // disable  7 seg display
  set7SegDisplay ( 0 );

  // disable IR sensors
  enableIr ( false );

  // close connection to create
  closePort();

  // turn off power
  if ( createPowerEnable ( false ) == 0 )
    ERROR0 ( "Failed to power down Create" );
}
//-----------------------------------------------------------------------------
int CCBDriver::init()
{
  PRT_STATUS ( "Initializing IRobot Create..." );

  // lets reset robotstix, just in case
  if ( resetRobostix() == 0 ) {
    ERROR0 ( "Failed to reset robotstix" );
    return 0; // failure
  }
  sleep ( 2 ); // give robostix time to boot

  // init all the robostix and PCA9634 stuff
  if ( initI2c() == 0 )
    return 0; // failure

  // power create up
  if ( createPowerEnable ( true ) == 0 ) {
    ERROR0 ( "Failed to power up Create" );
    return 0; // failure
  }

  return 1; // success
}
//--------------------------------------------------------------------------
int CCBDriver::openPort ( const char* port )
{

  struct termios term;
  int flags;

  PRT_STATUS ( "Opening port..." );

  if ( ( mFd = open ( port, O_RDWR | O_NONBLOCK, S_IRUSR | S_IWUSR ) ) < 0 ) {
    ERROR2 ( "Failed to open port %s: %s", port, strerror ( errno ) );
    return 0; // failure
  }

  if ( tcflush ( mFd, TCIOFLUSH ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  if ( tcgetattr ( mFd, &term ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  cfmakeraw ( &term );

  cfsetispeed ( &term, B57600 );
  cfsetospeed ( &term, B57600 );

  if ( tcsetattr ( mFd, TCSAFLUSH, &term ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  sleep ( 1 );
  if ( startCreate() == 0 ) {
    ERROR0 ( "failed to initialize connection to Create" );
    return 0; // failure
  }
  sleep ( 1 );
  if ( ( flags = fcntl ( mFd, F_GETFL ) ) < 0 ) {
    ERROR1 ( "IO error: %s ", strerror ( errno ) );
    return 0; // failure
  }

  if ( fcntl ( mFd, F_SETFL, flags ^ O_NONBLOCK ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  return 1; // success
}
//--------------------------------------------------------------------------
int CCBDriver::closePort()
{
  if ( mCreateSensorPackage.oiMode == CB_MODE_PASSIVE ) {
    activateDemo ( CB_DEMO_STOP );
  }
  else {
    setSpeed ( CVelocity2d ( 0.0, 0.0, 0.0 ) );
  }

  sleep ( 1 );
  // return control to create, otherwise it won't charge etc.
  setOIMode ( CB_MODE_PASSIVE );

  usleep ( CREATE_DELAY_MODECHANGE_MS * 1000 );

  if ( close ( mFd ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  return 1; // success
}
//--------------------------------------------------------------------------
int CCBDriver::startCreate()
{
  int loopCount = 0;
  unsigned char cmdbuf[1];

  cmdbuf[0] = CREATE_OPCODE_START;

  if ( write ( mFd, cmdbuf, 1 ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0;
  }
  sleep ( 1 );

  do {
    loopCount ++;

    // set mode to SAFE
    if ( setOIMode ( CB_MODE_SAFE ) == 0 ) {
      ERROR0 ( "Failed to set SAFE mode" );
      return 0;
    }
    //usleep( CREATE_DELAY_MODECHANGE_MS * 1000 );
    sleep ( 2 );
    // try to read data
    if ( readSensorData( 0.0 ) == 0 ) {
      ERROR0 ( "Connected but failed to read 1. data package" );
      return 0;
    }
    if ( loopCount > 10 ) {
      ERROR0 ( "Failed to enable SAFE mode on Create" );
      return 0;
    }
    if ( mCreateSensorPackage.oiMode != CB_MODE_SAFE ) {
      PRT_WARN1 ( "Create still in %s mode, but should be in SAFE mode",
                  getOiModeString() );
    }
  }
  while ( mCreateSensorPackage.oiMode != CB_MODE_SAFE );

  // disable all low side drivers
  if ( setLowSideDriver ( 0, false ) == 0 ) {
    return 0;
  }
  if ( setLowSideDriver ( 1, false ) == 0 ) {
    return 0;
  }
  if ( setLowSideDriver ( 2, false ) == 0 ) {
    return 0;
  }
  return 1;
}
//--------------------------------------------------------------------------
const char* CCBDriver::getOiModeString() const
{
  switch ( mCreateSensorPackage.oiMode ) {
    case CB_MODE_OFF:     return "OFF";
    case CB_MODE_PASSIVE: return "PASSIVE";
    case CB_MODE_SAFE:    return "SAFE";
    case CB_MODE_FULL:    return "FULL";
  } // switch

  return "Unknown";
}
//--------------------------------------------------------------------------
tOIMode CCBDriver::getOIMode()  const
{
  return ( tOIMode ) mCreateSensorPackage.oiMode;
}
//--------------------------------------------------------------------------
int CCBDriver::setOIMode ( tOIMode mode )
{
  unsigned char cmdbuf[1];

  switch ( mode ) {
    case CB_MODE_OFF:
      PRT_WARN0 ( "This mode is not implemented, switching to passive" );
      PRT_MSG0 ( 7, "Mode changed to OFF" );
      cmdbuf[0] = CREATE_OPCODE_START;
      break;
    case CB_MODE_PASSIVE:
      cmdbuf[0] = CREATE_OPCODE_START;  // this also puts us into passive
      PRT_MSG0 ( 7, "Mode changed to PASSIVE" );
      break;
    case CB_MODE_SAFE:
      cmdbuf[0] = CREATE_OPCODE_SAFE;
      PRT_MSG0 ( 7, "Mode changed to SAFE" );
      break;
    case CB_MODE_FULL:
      cmdbuf[0] = CREATE_OPCODE_FULL;
      PRT_MSG0 ( 7, "Mode changed to FULL" );
      break;
  } // switch
  if ( write ( mFd, cmdbuf, 1 ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0;
  }
  return 1;
}

//--------------------------------------------------------------------------
int CCBDriver::setSpeed ( CVelocity2d vel )
{
  unsigned char cmdbuf[5];
  int16_t vL, vR;
  double v, w, r;

  mVelocityCmd = vel;

  v = vel.mXDot * 1e3;
  w = vel.mYawDot * 1e3;
  r = CREATE_AXLE_LENGTH / 2;

  vL = ( int16_t ) ( v - r * w );
  vR = ( int16_t ) ( v + r * w );

  cmdbuf[0] = CREATE_OPCODE_DRIVE_DIRECT;
  cmdbuf[1] = ( unsigned char ) ( vR >> 8 );
  cmdbuf[2] = ( unsigned char ) ( vR & 0xff );
  cmdbuf[3] = ( unsigned char ) ( vL >> 8 );
  cmdbuf[4] = ( unsigned char ) ( vL & 0xff );

  if ( write ( mFd, cmdbuf, 5 ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror ( errno ) );
    return 0; // failure
  }

  return 1; // success
}
//---------------------------------------------------------------------------
//int CCBDriver::getOdoData()
//{
//  // get odometry from Create
//  const int nCmdBytes = 6, nDataBytes = 4;
//  uint8_t cmdBuf[nCmdBytes];
//  union { // anonymous union to save type conversions
//    uint8_t dataBuf[nDataBytes];
//    struct {
//      int16_t distance;
//      int16_t angle;
//    } odom;
//  };
//  cmdBuf[0] = CREATE_OPCODE_STREAM_SENSORS;
//  cmdBuf[1] = 2;  // two packets IDs to follow
//  cmdBuf[2] = 19; // distance packet
//  cmdBuf[3] = 20; // angle packet
//  if( readSerialData( cmdBuf, nCmdBytes, dataBuf, nDataBytes ) == 0 )
//    return 0; // just pass errors up
//
//  // fuse measured data with accumulated estimate data
//  double measDistance = (double) int16_t( ntohs( (uint16_t) odom.distance ) );
//  double measAngle = (double) int16_t( ntohs( (uint16_t) odom.angle ) );
//  mCreateSensorPackage.distance = measDistance - mEstDistance;
//  mCreateSensorPackage.angle = measAngle - mEstAngle;
//  mEstDistance = 0.0;
//  mEstAngle = 0.0;
//
//  return 1; // success
//}
//---------------------------------------------------------------------------
int CCBDriver::getOdoData()
{
  uint8_t cmdBuf[4];
  uint8_t dataBuf[2];
  short value;
  double measDistance = 0.0;
  double measAngle = 0.0;

  if ( (mAccDistanceCmd > REQ_DISTANCE_ODOM_THRESHOLD) &&
       (mAccDistanceCmd < -REQ_DISTANCE_ODOM_THRESHOLD) )  {
    // read distance only

    cmdBuf[0] = CREATE_OPCODE_STREAM_SENSORS;
    cmdBuf[1] = 1;  // one packet ID to follow
    cmdBuf[2] = CB_PACKET_DISTANCE; // distance packet

    if( readSerialData( cmdBuf, 3, dataBuf, 2 ) == 0 )
      return 0; // just pass errors up

    memcpy((char*)&value, dataBuf, 2);

    measDistance = (double) int16_t( ntohs( value ) ); // [mm]
    mCreateSensorPackage.distance = measDistance - ( mAccDistanceCmd * 1e3 );

    //printf("CCBDriver: dist meas %f  acc %f \n", measDistance, mAccDistanceCmd*1e3);
    mAccDistanceCmd = 0.0;
  }

  if ( (mAccAngleCmd >  REQ_ANGLE_ODOM_THRESHOLD) ||
      (mAccAngleCmd < -REQ_ANGLE_ODOM_THRESHOLD) ) {
    // read angle only
    cmdBuf[0] = CREATE_OPCODE_STREAM_SENSORS;
    cmdBuf[1] = 1;  // one packet ID to follow
    cmdBuf[2] = CB_PACKET_ANGLE; // angle packet

    if( readSerialData( cmdBuf, 3, dataBuf, 2 ) == 0 )
      return 0; // just pass errors up

    memcpy((char*)&value, dataBuf, 2);
    measAngle = (double) int16_t( ntohs( value ) ); // [deg]
    //mCreateSensorPackage.angle = measAngle - R2D(mAccAngleCmd);
    //printf("CCBDriver: angle meas %f  acc %f \n", measAngle, R2D(mAccAngleCmd));
    mAccAngleCmd = 0.0;
  }

  //fprintf(fpFoo,  "%f, %f, %f, %f \n", measDistance, mAccDistanceCmd*1e3,  measAngle, R2D(mAccAngleCmd) );
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::getMostData( double dt )
{
  // estimate odometry based on commanded velocity over previous interval
  //double distRight = mCreateSensorPackage.rightWheelVelocity * dt;
  //double distLeft = mCreateSensorPackage.leftWheelVelocity * dt;
  //double estDist = 0.5 * (distRight + distLeft);
  //double estAngle = R2D( (distRight - distLeft) / (1e3 * CREATE_AXLE_LENGTH) );
  //mEstDistance += estDist;
  //mEstAngle += estAngle;
  //mCreateSensorPackage.distance = estDist;
  //mCreateSensorPackage.angle = estAngle;

  mCreateSensorPackage.distance = mVelocityCmd.mXDot * dt * 1e3; // [mm]
  mCreateSensorPackage.angle = R2D(mVelocityCmd.mYawDot * dt); // [deg]

  // get the rest of the data from the Create
  const int nCmdBytes = 8, nDataBytes = 48;
  uint8_t cmdBuf[nCmdBytes];
  uint8_t dataBuf[nDataBytes];
  cmdBuf[0] = CREATE_OPCODE_STREAM_SENSORS;
  cmdBuf[1] = 6;  // six packet IDs to follow
  cmdBuf[2] = 1;  // packets 7-16
  cmdBuf[3] = 17; // IR byte
  cmdBuf[4] = 18; // Buttons
  cmdBuf[5] = 3;  // packets 21-26
  cmdBuf[6] = 4;  // packets 27-34
  cmdBuf[7] = 5;  // packets 35-42
  if( readSerialData( cmdBuf, nCmdBytes, dataBuf, nDataBytes ) == 0 )
    return 0; // just pass errors up
  memcpy ( (uint8_t *) &mCreateSensorPackage, dataBuf, nDataBytes );

  // next we need to fix the byte order of data types with more then 1 byte
  mCreateSensorPackage.voltage = ntohs ( mCreateSensorPackage.voltage );
  mCreateSensorPackage.current
  = ( short ) ntohs ( (uint16_t) mCreateSensorPackage.current );
  mCreateSensorPackage.batCapacity
  = (short) ntohs ( mCreateSensorPackage.batCapacity );
  mCreateSensorPackage.batMaxCapacity
  = ntohs ( mCreateSensorPackage.batMaxCapacity );
  mCreateSensorPackage.wallSignal
  = ntohs ( mCreateSensorPackage.wallSignal );
  mCreateSensorPackage.cliffSignalLeft
  = ntohs ( mCreateSensorPackage.cliffSignalLeft );
  mCreateSensorPackage.cliffSignalFrontLeft
  = ntohs ( mCreateSensorPackage.cliffSignalFrontLeft );
  mCreateSensorPackage.cliffSignalFrontRight
  = ntohs ( mCreateSensorPackage.cliffSignalFrontRight );
  mCreateSensorPackage.cliffSignalRight
  = ntohs ( mCreateSensorPackage.cliffSignalRight );
  mCreateSensorPackage.analogInput
  = ntohs ( mCreateSensorPackage.analogInput );
  mCreateSensorPackage.velocity
  = ( short ) ntohs ( ( uint16_t ) mCreateSensorPackage.velocity );
  mCreateSensorPackage.radius
  = ( short ) ntohs ( ( uint16_t ) mCreateSensorPackage.radius );
  mCreateSensorPackage.rightWheelVelocity
  = ( short ) ntohs ( ( uint16_t ) mCreateSensorPackage.rightWheelVelocity );
  mCreateSensorPackage.leftWheelVelocity
  = ( short ) ntohs ( ( uint16_t ) mCreateSensorPackage.leftWheelVelocity );

  // mask out unused bytes and check for over current condition
  if ( ( mCreateSensorPackage.overCurrents & 0x1F ) != 0 )
    PRT_MSG1 ( 8, "OVER CURRENT -- OVER CURRENT code %d",
                mCreateSensorPackage.overCurrents );

  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::readSerialData( uint8_t * cmdBuf, int nCmdBytes,
                               uint8_t * dataBuf, int nDataBytes )
{
  struct pollfd ufd[1];

  // write command to Create
  if ( write( mFd, cmdBuf, nCmdBytes ) < 0 ) {
    ERROR1 ( "IO error: %s", strerror( errno ) );
    return 0;
  }

  // read response from Create
  int totalNumRead = 0;
  ufd[0].fd = mFd;
  ufd[0].events = POLLIN;
  while ( totalNumRead < nDataBytes ) {
    int retVal = poll( ufd, 1, READ_TIMEOUT );
    if ( retVal < 0 ) { // poll fails
      if ( errno == EINTR ) // interrupted - try again
        continue;
      else {
        ERROR1( "IO error: %s", strerror( errno ) );
        return 0;
      }
    }
    else if ( retVal == 0 ) { // timed-out
      ERROR1( "Timeout (bytes read %d)", totalNumRead );
      return 0;
    }
    else { // poll returned successfully
      int numRead = read( mFd, dataBuf + totalNumRead, nDataBytes - totalNumRead );
      if( numRead < 0 ) { // error reading from serial
        ERROR1( "IO error: %s", strerror( errno ) );
        return 0;
      }
      totalNumRead += numRead;
    }
  }

  // check that we read enough bytes
  if ( totalNumRead != nDataBytes ) {
    ERROR2( "Wrong package size received, %d expected but got % d",
            nDataBytes, totalNumRead );
    return 0;
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::readSensorData( double dt )
{
  // The Create resets the odometry data (distance and angle) each time these
  // are read. This data is quite low resolution so if we read it frequently
  // we get lots of rounding error in our data; this is particularly damaging
  // to the angular measurements where a small error can drastically affect our
  // estimated position.  As such, we read all the other sensors frequently but
  // only read odometry once every CREATE_READ_ODO_COUNT cycles. In the
  // interim, we estimate our odometry by integrating the commanded velocity.
  // We then correct this estimate with our measured values.
  //static int count = 0; // read the first time
  //bool fgReadOdo = ( count == 0 ) ? true : false;
  //count = fgReadOdo ? (CREATE_READ_ODO_COUNT - 1) : count - 1;


  // integrate speed commands for distance relative odometry updates
  mAccDistanceCmd = mAccDistanceCmd + mVelocityCmd.mXDot * dt; // [m]
  mAccAngleCmd = mAccAngleCmd + mVelocityCmd.mYawDot * dt; // [rad]
  mAccAngleCmd = normalizeAngle(mAccAngleCmd);

  //printf("Acc: dist %f angle %f (cmd %f %f) \n", mAccDistanceCmd, mAccAngleCmd, mVelocityCmd.mXDot, mVelocityCmd.mYawDot);

  if( getMostData( dt ) == 0 )
    return 0; // pass up errors

  //if( fgReadOdo ) {
    if( getOdoData() == 0 )
      return 0; // pass up errors
  //}

  return 1; // success
}
//---------------------------------------------------------------------------
unsigned short CCBDriver::getRawCliffSensor(tCliffSensor id) const
{
  switch ( id ) {
    case CB_ALL_CLIFF:
      return (mCreateSensorPackage.cliffSignalLeft +
              mCreateSensorPackage.cliffSignalFrontLeft +
              mCreateSensorPackage.cliffSignalFrontRight +
              mCreateSensorPackage.cliffSignalRight);
      break;
    case CB_LEFT_CLIFF:
      return mCreateSensorPackage.cliffSignalLeft;
      break;
    case CB_FRONT_LEFT_CLIFF:
      return mCreateSensorPackage.cliffSignalFrontLeft;
      break;
    case CB_FRONT_RIGHT_CLIFF:
      return mCreateSensorPackage.cliffSignalFrontRight;
      break;
    case CB_RIGHT_CLIFF:
      return mCreateSensorPackage.cliffSignalRight;
      break;

    default:
      ERROR1 ( "Unknown cliff sensor requested %d", id );
      return 0;
      break;
  }  // switch
}
//---------------------------------------------------------------------------
bool CCBDriver::getCliffSensor ( tCliffSensor id )  const
{
  switch ( id ) {
    case CB_ALL_CLIFF:
      if ( ( mCreateSensorPackage.cliffLeft > 0 ) ||
           ( mCreateSensorPackage.cliffFrontLeft > 0 ) ||
           ( mCreateSensorPackage.cliffFrontRight > 0 ) ||
           ( mCreateSensorPackage.cliffRight > 0 ) )
        return true;
      else
        return false;
      break;
    case CB_LEFT_CLIFF:
      return mCreateSensorPackage.cliffLeft;
      break;
    case CB_FRONT_LEFT_CLIFF:
      return mCreateSensorPackage.cliffFrontLeft;
      break;
    case CB_FRONT_RIGHT_CLIFF:
      return mCreateSensorPackage.cliffFrontRight;
      break;
    case CB_RIGHT_CLIFF:
      return mCreateSensorPackage.cliffRight;
      break;

    default:
      ERROR1 ( "Unknown cliff sensor requested %d", id );
      break;
  }  // switch

  return false; // failure
}
//---------------------------------------------------------------------------
int CCBDriver::setCreateLed ( bool advance, bool play, unsigned char powerColor,
                              unsigned char powerIntensity )
{
  unsigned char cmdBuf[4];
  cmdBuf[1] = 0;

  cmdBuf[0] = CREATE_OPCODE_LED;
  if ( advance )
    cmdBuf[1] |= 0x08;
  if ( play )
    cmdBuf[1] |= 0x02;

  cmdBuf[2] = powerColor;
  cmdBuf[3] = powerIntensity;

  if ( write ( mFd, cmdBuf, 4 ) < 0 ) {
    ERROR1 ( "Failed to send LED command: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1;  // success
}
//----------------------------------------------------------------------------
int CCBDriver::resetCreate()
{
  unsigned char cmdBuf[1];

  cmdBuf[0] = CREATE_OPCODE_RESET;
  if ( write ( mFd, cmdBuf, 1 ) < 0 ) {
    ERROR1 ( "Failed to reset create: %s", strerror ( errno ) );
    return 0; // failure
  }
  PRT_MSG0 ( 7, "Reset command send, now we wait for 10 sec, and "\
             "hope for the best" );
  sleep ( 10 );
  return 1;
}
//----------------------------------------------------------------------------
bool CCBDriver::bumper()  const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_LEFT_BUMPER ) ||
         ( mCreateSensorPackage.bumpsWheel & CB_RIGHT_BUMPER );
}
//----------------------------------------------------------------------------
bool CCBDriver::leftBumper() const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_LEFT_BUMPER );
}
//----------------------------------------------------------------------------
bool CCBDriver::rightBumper() const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_RIGHT_BUMPER );
}
//----------------------------------------------------------------------------
bool CCBDriver::wheelDrop() const
{
  if ( mCreateSensorPackage.bumpsWheel >= 4 )
    return true;

  return false;
}
//----------------------------------------------------------------------------
bool CCBDriver::leftWheelDrop() const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_LEFT_WHEEL_DROP );
}
//----------------------------------------------------------------------------
bool CCBDriver::rightWheelDrop() const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_RIGHT_WHEEL_DROP );
}
//----------------------------------------------------------------------------
bool CCBDriver::casterWheelDrop() const
{
  return ( mCreateSensorPackage.bumpsWheel & CB_CASTER_WHEEL_DROP );
}
//----------------------------------------------------------------------------
bool CCBDriver::leftWheelOverCurrent() const
{
  return ( mCreateSensorPackage.overCurrents & CB_OVERCURRENT_LEFT_WHEEL );
}
//----------------------------------------------------------------------------
bool CCBDriver::rightWheelOverCurrent() const
{
  return ( mCreateSensorPackage.overCurrents & CB_OVERCURRENT_RIGHT_WHEEL );
}
//----------------------------------------------------------------------------
bool CCBDriver::lowSideDriverOverCurrent ( unsigned char id ) const
{
  switch ( id ) {
    case 0:
      return ( mCreateSensorPackage.overCurrents & CB_OVERCURRENT_LSD_0 );
    case 1:
      return ( mCreateSensorPackage.overCurrents & CB_OVERCURRENT_LSD_1 );
    case 2:
      return ( mCreateSensorPackage.overCurrents & CB_OVERCURRENT_LSD_2 );
    default:
      ERROR1 ( "Low side driver id %d out of bounds [0..2]", id );
  } // switch
  return false;
}
//---------------------------------------------------------------------------
bool CCBDriver::isPlayButtonPressed() const
{
  if ( mCreateSensorPackage.button & CB_PLAY_BUTTON_MASK )
    return true;

  return false;
}
//---------------------------------------------------------------------------
bool CCBDriver::isAdvanceButtonPressed() const
{
  if ( mCreateSensorPackage.button & CB_ADVACNE_BUTTON_MASK )
    return true;

  return false;
}
//---------------------------------------------------------------------------
int CCBDriver::playSong ( unsigned char id )
{
  unsigned char cmdBuf[2];

  if ( id > 15 ) {
    ERROR1 ( "Song id %d out of bounds [0..15] ", id );
    return 0; // failure
  }

  cmdBuf[0] = CREATE_OPCODE_PLAY_SONG;
  cmdBuf[1] = id;

  if ( write ( mFd, cmdBuf, 2 ) < 0 ) {
    ERROR1 ( "Failed to send play sound command: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::setLowSideDriver ( unsigned char id, bool on )
{
  unsigned char cmdBuf[2];

  if ( not ( ( mCreateSensorPackage.oiMode == CB_MODE_SAFE ) ||
             ( mCreateSensorPackage.oiMode == CB_MODE_FULL ) ) ) {
    PRT_WARN1 ( "Low side driver access is only available in SAFE "\
                "or FULL mode (mode is %d)",  mCreateSensorPackage.oiMode );
    return 0; // failure
  }

  switch ( id ) {
    case 0:
      if ( on )
        mLowSideDriverStatus = mLowSideDriverStatus | 0x01;
      else
        mLowSideDriverStatus = mLowSideDriverStatus & 0xFE; // 1110
      break;

    case 1:
      if ( on )
        mLowSideDriverStatus = mLowSideDriverStatus | 0x02;
      else
        mLowSideDriverStatus = mLowSideDriverStatus & 0xFD; // 1101
      break;

    case 2:
      if ( on )
        mLowSideDriverStatus = mLowSideDriverStatus | 0x04;
      else
        mLowSideDriverStatus = mLowSideDriverStatus & 0xFB; // 1011
      break;

    default:
      ERROR1 ( "Low side driver id %d out of bounds [0..2]", id );
      return 0; // failure
      break;
  } // switch

  cmdBuf[0] = CREATE_OPCODE_LOW_SIDE_DRIVER;
  cmdBuf[1] = mLowSideDriverStatus;

  if ( write ( mFd, cmdBuf, 2 ) < 0 ) {
    ERROR1 ( "Failed to send low side driver command: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::activateDemo ( tDemo demo )
{
  unsigned char cmdBuf[2];

  cmdBuf[0] = CREATE_OPCODE_DEMO;
  cmdBuf[1] = ( char ) demo;

  if ( write ( mFd, cmdBuf, 2 ) < 0 ) {
    ERROR1 ( "Failed to active demo: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::sendIr ( unsigned char byte )
{
  unsigned char cmdBuf[2];

  cmdBuf[0] = CREATE_OPCODE_SEND_IR;
  cmdBuf[1] = byte;

  if ( write ( mFd, cmdBuf, 2 ) < 0 ) {
    ERROR1 ( "Failed to send IR byte: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::defineSoundSequence ( unsigned char id, unsigned char* sequence,
                                     unsigned char len )
{
  unsigned cmdBuf[2+2*16];
  if ( id > 15 ) {
    ERROR1 ( "Song id %d out of bounds [0..15]", id );
    return 0; // failure
  }

  if ( ( len < 1 ) || ( len > 16 ) ) {
    ERROR1 ( "Song length %d out of bounds [1..16]", len );
    return 0; // failure
  }

  cmdBuf[0] = CREATE_OPCODE_DEFINE_SONG;
  cmdBuf[1] = len;
  for ( unsigned char i = 0; i < len; i += 2 ) {
    if ( ( sequence[i] < 31 ) || ( sequence[i] > 127 ) ) {
      ERROR2 ( "Note(%d) = %d out of bounds [31..127] ", i, sequence[i] );
      return 0; // failure
    }
    cmdBuf[i+2] = sequence[i];
    cmdBuf[i+3] = sequence[1+1];
  }
  if ( write ( mFd, cmdBuf, 2 ) < 0 ) {
    ERROR1 ( "Failed to send sound sequence: %s", strerror ( errno ) );
    return 0; // failure
  }
  return 1; // success
}
//---------------------------------------------------------------------------
int CCBDriver::createPowerEnable ( bool on )
{
  int i = 0;
  unsigned int toggleTime;

  if ( on == isCreatePowerEnabled() )
    return 1; // success, we are already in the correct power state

  if ( on )
    toggleTime = CREATE_TOGGLE_ON_TIME;
  else
    toggleTime = CREATE_TOGGLE_OFF_TIME;

  while ( ( isCreatePowerEnabled() != on ) &&
          ( i < NUM_POWER_TOGGLE_RETRY ) ) {

    if ( createPowerToggle ( toggleTime ) == 0 )
      return 0; // error while toggleing power line
    sleep ( 2 );
    i++;
  }

  if ( isCreatePowerEnabled() != on ) {
    ERROR0 ( "Can't turn Create ON/OFF!\n" );
    return 0; // error, this is not the result we wanted
  }

  sleep ( 4 ); // wait for power change to take effect
  return 1; // success
}
//---------------------------------------------------------------------------
bool CCBDriver::isCreatePowerEnabled()
{
  unsigned char value = 0;

  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  // 2 => port C
  if ( I2C_IO_GetGPIO ( mI2cDev, 2, &value ) == 0 ) {
    ERROR0 ( "Failed to read 5V power pin" );
  }
  return ( value & 0x08 ); // we are interested in PC3
}
//---------------------------------------------------------------------------
int CCBDriver::initPCA9634 ( unsigned char addr )
{
  unsigned char data[2];

  I2cSetSlaveAddress ( mI2cDev, addr, I2C_NO_CRC );

  // enable clock
  data[0] = 0x00;   // register address
  data[1] = 0x0f;   // register value
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR1 ( "Failed to initialize chip %d", addr );
    return 0; // failure
  }

  // enable led output A
  data[0] = 0x0c;   // register address
  data[1] = 0xff;   // register value
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR1 ( "Error initializing chip %d", addr );
    return 0; // failure
  }

  // enable led output B
  data[0] = 0x0d;   // register address
  data[1] = 0xff;   // register value
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR1 ( "Error initializing chip %d", addr );
    return 0; // failure
  }

  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBDriver::createPowerToggle ( unsigned int delay )
{
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  // Create: power pin toggle needs low - high transition

  // set power pin PC0 to "0" (port 2 pinmask 1)
  if ( I2C_IO_SetGPIO ( mI2cDev, 2, 1, 0 ) == 0 ) {
    ERROR0 ( "failed to toggle Create power pin (on)" );
    return 0; // error
  }
  usleep ( delay );
  /// set power pin PC0 to "1" (port 2 pinmask 1)
  if ( I2C_IO_SetGPIO ( mI2cDev, 2, 1, 0xFF ) == 0 ) {
    ERROR0 ( "failed to toggle Create power pin (off)" );
    return 0; // failure
  }

  PRT_MSG0 ( 7, "Power pin toggled" );
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBDriver::resetPCA9634()
{
  unsigned char data[2];

  // Call ALL address
  I2cSetSlaveAddress ( mI2cDev, 0x03, I2C_NO_CRC );

  // reset all
  data[0] = 0xa5;
  data[1] = 0x5a;
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR0 ( "failed to reset pca9634 chips" );
    return 0; // failure
  }

  return 1;  // success
}
//----------------------------------------------------------------------------
int CCBDriver::set7SegDisplay ( unsigned char value )
{
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );
  if ( I2C_IO_WriteReg8 ( mI2cDev, PORTA, value ) == 0 ) {
    ERROR0 ( "failed to set 7 segment display" );
    return 0; // failure
  }

  m7SegByteValue = value;
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBDriver::initI2c()
{
  I2C_IO_Info_t   info;

  PRT_MSG0 ( 7, "Initializing I2C stuff" );
  // open I2C device
  if ( ( mI2cDev = open ( I2C_DEV_NAME, O_RDWR ) ) < 0 ) {
    ERROR2 ( "Failed to open port %s: %s", I2C_DEV_NAME, strerror ( errno ) );
    return 0; // failure
  }

  //*****************************************
  // Robostix stuff

  // lets talk to robostix
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  // check if software version robostix is high enough
  if ( !I2C_IO_GetInfo ( mI2cDev, &info ) ) {
    ERROR1 ( "Unable to retrieve information from i2c address %d",
             ROBOSTIX_ADDR );
    return 0; // failure
  }
  if ( info.version < 2 ) {
    ERROR0 ( "i2c-io.hex on the robostix is too old and needs to be updated" );
    return 0; // failure
  }

  // configure DDRA port as output (0xFF)
  if ( I2C_IO_WriteReg8 ( mI2cDev, DDRA, 0xFF ) == 0 ) {
    ERROR0 ( "Failed to setup 7 seg display" );
    return 0; // failure
  }

  // clear 7 seg display
  set7SegDisplay ( 0 );


  // initializing PORT[A, C]  to 0
  if ( I2C_IO_WriteReg8 ( mI2cDev, PORTA, 0x00 ) == 0 ) {
    ERROR0 ( "Failed to reset PORTA" );
    return 0; // failure
  }
  if ( I2C_IO_WriteReg8 ( mI2cDev, PORTC, 0x00 ) == 0 ) {
    ERROR0 ( "Failed to reset PORTC" );
    return 0; // failure
  }


  //*********************************************************
  // configure ir enable pin as output (PC2 = port 2 pinmask 4)
  if ( I2C_IO_SetGPIODir ( mI2cDev, 2, 4, 0xff ) == 0 ) {
    ERROR0 ( "Failed to setup IR enable pin" );
    return 0; // failure
  }

  // set ir enabled pin 0 (PC2 = port 2 pinmask 4)
  if ( I2C_IO_SetGPIO ( mI2cDev, 2, 4, 0 ) == 0 ) {
    ERROR0 ( "Failed to clear Create power pin" );
    return 0; // failure
  }

  //*********************************************************
  // configure create 5V pin as input, this pin allows us to
  // see if create is on or not (PC3 = port 2 pinmask 8)
  if ( I2C_IO_SetGPIODir ( mI2cDev, 2, 8, 0 ) == 0 ) {
    ERROR0 ( "Failed to setup power monitor pin" );
    return 0; // failure
  }

  //*********************************************************
  // configure create power toggle pin as output (PC0 = port 2 pinmask 1)
  if ( I2C_IO_SetGPIODir ( mI2cDev, 2, 1, 0xff ) == 0 ) {
    ERROR0 ( "Failed to setup power toggle pin" );
    return 0; // failure
  }

  //*********************************************************
  // configure create baud rate toggle pin as output
  // (PC4 = port 2 pinmask 16)
  if ( I2C_IO_SetGPIODir ( mI2cDev, 2, 16, 0xff ) == 0 ) {
    ERROR0 ( "Failed to setup baud rate toggle pin" );
    return 0; // failure
  }
  // set baud rate pin "1" (PC0 = port 2 pinmask 16)
  if ( I2C_IO_SetGPIO ( mI2cDev, 2, 16, 0xff ) == 0 ) {
    ERROR0 ( "Failed to set Create baud rate pin" );
    return 0; // failure
  }

  //****************************************
  // RGB stuff

  mLed[0][RED_CH].chipAddr   = LED0_R_ADDR;
  mLed[0][RED_CH].ledAddr    = LED0_R_PORT;
  mLed[0][GREEN_CH].chipAddr = LED0_G_ADDR;
  mLed[0][GREEN_CH].ledAddr  = LED0_G_PORT;
  mLed[0][BLUE_CH].chipAddr  = LED0_B_ADDR;
  mLed[0][BLUE_CH].ledAddr   = LED0_B_PORT;

  mLed[1][RED_CH].chipAddr   = LED1_R_ADDR;
  mLed[1][RED_CH].ledAddr    = LED1_R_PORT;
  mLed[1][GREEN_CH].chipAddr = LED1_G_ADDR;
  mLed[1][GREEN_CH].ledAddr  = LED1_G_PORT;
  mLed[1][BLUE_CH].chipAddr  = LED1_B_ADDR;
  mLed[1][BLUE_CH].ledAddr   = LED1_B_PORT;

  mLed[2][RED_CH].chipAddr   = LED2_R_ADDR;
  mLed[2][RED_CH].ledAddr    = LED2_R_PORT;
  mLed[2][GREEN_CH].chipAddr = LED2_G_ADDR;
  mLed[2][GREEN_CH].ledAddr  = LED2_G_PORT;
  mLed[2][BLUE_CH].chipAddr  = LED2_B_ADDR;
  mLed[2][BLUE_CH].ledAddr   = LED2_B_PORT;

  mLed[3][RED_CH].chipAddr   = LED3_R_ADDR;
  mLed[3][RED_CH].ledAddr    = LED3_R_PORT;
  mLed[3][GREEN_CH].chipAddr = LED3_G_ADDR;
  mLed[3][GREEN_CH].ledAddr  = LED3_G_PORT;
  mLed[3][BLUE_CH].chipAddr  = LED3_B_ADDR;
  mLed[3][BLUE_CH].ledAddr   = LED3_B_PORT;

  mLed[4][RED_CH].chipAddr   = LED4_R_ADDR;
  mLed[4][RED_CH].ledAddr    = LED4_R_PORT;
  mLed[4][GREEN_CH].chipAddr = LED4_G_ADDR;
  mLed[4][GREEN_CH].ledAddr  = LED4_G_PORT;
  mLed[4][BLUE_CH].chipAddr  = LED4_B_ADDR;
  mLed[4][BLUE_CH].ledAddr   = LED4_B_PORT;

  // reset all chips and then initialize them
  if ( resetPCA9634() == 0 )
    return 0; // failure

  if ( initPCA9634 ( PCA98634_CHIP_A_ADDR ) == 0 )
    return 0; // failure

  if ( initPCA9634 ( PCA98634_CHIP_B_ADDR ) == 0 )
    return 0; // failure

  return 1;  // success
}
//----------------------------------------------------------------------------
int CCBDriver::setIrLed ( unsigned char data )
{
  unsigned char buffer[2];

  //*************************************
  // send ir data
  I2cSetSlaveAddress ( mI2cDev, PCA98634_CHIP_B_ADDR, I2C_NO_CRC );

  buffer[0] = IR_LED_ADDR;
  buffer[1] = data;
  if ( I2cSendBytes ( mI2cDev, buffer, 2 ) != 0 ) {
    ERROR1 ( "Failed to set data %d to top IR led", data );
    return 0; // failure
  }
  return 1; // success
}
//----------------------------------------------------------------------------
int CCBDriver::setRgbLed ( unsigned char id, CRgbColor color )
{
  unsigned char data[2];

  if ( id > 4 ) {
    ERROR1 ( "mLed id out of bounds [0..4] %d", id );
    return 0; // failure
  }

  //*************************************
  // send red channel data
  I2cSetSlaveAddress ( mI2cDev, mLed[id][RED_CH].chipAddr, I2C_NO_CRC );

  data[0] = mLed[id][RED_CH].ledAddr;
  data[1] = color.mRed;
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR2 ( "Failed to set color red of led %d to %d", id, color.mRed );
    return 0; // failure
  }

  //*************************************
  // send green channel data
  I2cSetSlaveAddress ( mI2cDev, mLed[id][GREEN_CH].chipAddr, I2C_NO_CRC );

  data[0] = mLed[id][GREEN_CH].ledAddr;
  data[1] = color.mGreen;
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR2 ( "Failed to set color green of led %d to %d", id, color.mGreen );
    return 0; // failure
  }

  //*************************************
  // send blue channel data
  I2cSetSlaveAddress ( mI2cDev, mLed[id][BLUE_CH].chipAddr, I2C_NO_CRC );

  data[0] = mLed[id][BLUE_CH].ledAddr;
  data[1] = color.mBlue;
  if ( I2cSendBytes ( mI2cDev, data, 2 ) != 0 ) {
    ERROR2 ( "Failed to set color blue of led %d to %d \n", id, color.mBlue );
    return 0; // failure
  }

  return 1; // success
}

//-----------------------------------------------------------------------------
int CCBDriver::set7SegHexNumber ( unsigned char n, tTriState dot )
{
  unsigned char byteCode;

  switch ( n ) {
    case '0':
      byteCode = 0xFC;
      break;
    case '1':
      byteCode = 0x60;
      break;
    case '2':
      byteCode = 0xDA;
      break;
    case '3':
      byteCode = 0xF2;
      break;
    case '4':
      byteCode = 0x66;
      break;
    case '5':
      byteCode = 0xB6;
      break;
    case '6':
      byteCode = 0xBE;
      break;
    case '7':
      byteCode = 0xE0;
      break;
    case '8':
      byteCode = 0xFE;
      break;
    case '9':
      byteCode = 0xE6;
      break;
    case 'A':
      byteCode = 0xEE;
      break;
    case 'B':
      byteCode = 0x3E;
      break;
    case 'C':
      byteCode = 0x1A;
      break;
    case 'D':
      byteCode = 0x7A;
      break;
    case 'E':
      byteCode = 0x9E;
      break;
    case 'F':
      byteCode = 0x8E;
      break;
    default:
      ERROR1 ( "character %c out of bounds [0..F] \n", n );
      return 0; // failure

  } // switch

  // should we turn the dot on ?
  switch ( dot ) {
    case CB_ON:
      byteCode |= 0x01;
      break;
    case CB_OFF:
      byteCode &= 0xFE;
      break;
    case CB_DONT_CARE:
      if ( m7SegByteValue & 0x01 )
        byteCode |= 0x01;
      else
        byteCode &= 0xFE;
      break;
  } // switch

  return set7SegDisplay ( byteCode );
}
//-----------------------------------------------------------------------------
int CCBDriver::rotateStep()
{
  unsigned char byteCode;
  static int state = 0;

  switch ( state ) {
    case 0:
      byteCode = 0x80;
      break;
    case 1:
      byteCode = 0x40;
      break;
    case 2:
      byteCode = 0x20;
      break;
    case 3:
      byteCode = 0x10;
      break;
    case 4:
      byteCode = 0x08;
      break;
    case 5:
      byteCode = 0x04;
      state = -1;
      break;
    default:
      byteCode = 0;
      state = 0;
  } // switch

  state++;

  return set7SegDisplay ( byteCode );
}
//-----------------------------------------------------------------------------
int CCBDriver::enableIr ( bool enable )
{
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  if ( enable == true ) {
    PRT_MSG0 ( 7, "Enableing IR sensor" );
    // (PC2 = port 2 pinmask 4, pinvalue 4)
    if ( I2C_IO_SetGPIO ( mI2cDev, 2, 4, 4 ) == 0 ) {
      ERROR0 ( "Failed to enable IR" );
      return 0; // failure
    }
  }
  else {
    PRT_MSG0 ( 7, "Disableing IR sensor" );
    // (PC2 = port 2 pinmask 4)
    if ( I2C_IO_SetGPIO ( mI2cDev, 2, 4, 0 ) == 0 ) {
      ERROR0 ( "Failed to disable IR" );
      return 0; // failure
    }
  }

  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBDriver::isIrEnabled()
{
  unsigned char pinVal;

  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );
  // PC2 => port 2
  if ( I2C_IO_GetGPIO ( mI2cDev, 2, &pinVal ) == 0 ) {
    ERROR0 ( "Failed to read ir status" );
    return 0; // failure
  }
  // PC2 is 0x04
  return ( pinVal & 0x04 );
}
//-----------------------------------------------------------------------------
float CCBDriver::readPhotoSensor()
{
  return readLpfAdc ( 1 );
}
//-----------------------------------------------------------------------------
int CCBDriver::readAdc ( unsigned char id )
{
  unsigned short adcVal;

  if ( id > 7 ) {
    ERROR1 ( "ADC id out of bounds [0..7] %d", id );
    return 0; // failure
  }
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  if ( I2C_IO_GetADC ( mI2cDev, id, &adcVal ) == 0 ) {
    ERROR1 ( "Failed to read from ADC %d", id );
    return 0; // failure
  }
  return adcVal;
}
//-----------------------------------------------------------------------------
float CCBDriver::readLpfAdc ( unsigned char id )
{
  float adcVal;

  if ( id > 7 ) {
    ERROR1 ( "ADC id out of bounds [0..7] %d", id );
    return 0; // failure
  }
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  if ( I2C_IO_GetLpfADC ( mI2cDev, id, &adcVal ) == 0 ) {
    ERROR1 ( "Failed to read from ADC %d", id );
    return 0; // failure
  }
  return adcVal;
}
//-----------------------------------------------------------------------------
int CCBDriver::setLpfAdcTau ( float tau, unsigned char channel )
{
  I2cSetSlaveAddress ( mI2cDev, ROBOSTIX_ADDR, I2C_USE_CRC );

  if ( I2C_IO_SetTauLpf ( mI2cDev, tau, channel ) == 0 ) {
    ERROR0 ( "Failed to set time constant for ADC low pass filter" );
    return 0; // failure
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
float CCBDriver::readDistance ( unsigned char id )
{
  float voltage;
  float v;
  float distance;
  unsigned char adcId = 0;

  if ( id > 5 ) {
    ERROR1 ( "IR id out of bounds [0..5] %d", id );
    return 0; // failure
  }

  switch ( id ) {
    case 0:      // front center
      adcId = 7;
      break;
    case 1:      // front left
      adcId = 4;
      break;
    case 2:      // left side
      adcId = 3;
      break;
    case 3:      // back center
      adcId = 6;
      break;
    case 4:      // right side
      adcId = 2;
      break;
    case 5:      // front right
      adcId = 5;
      break;
  } // switch

  //voltage = (float)(readAdc(adcId)) / 1024.0 * VREF;
  voltage = ( float ) ( readLpfAdc ( adcId ) ) / 1024.0 * VREF;
  v = voltage;
  distance = K5;
  distance += K4 * v;
  v = v * voltage;
  distance += K3 * v;
  v = v * voltage;
  distance += K2 * v;
  v = v * voltage;
  distance += K1 * v;
  v = v * voltage;
  distance += K0 * v;

  // return voltage;
  return distance;
}
//---------------------------------------------------------------------------
int CCBDriver::resetRobostix()
{
  int fd;

  if ( ( fd = open ( "/dev/robostix",  O_RDWR ) ) < 0 ) {
    ERROR1 ( "Failed to open /dev/robostix: %s", strerror ( errno ) );
    return 0;
  }

  // power off
  if ( ioctl ( fd, ROBOSTIX_IOCTL_POWER_VCC5, 0 ) != 0 ) {
    ERROR1 ( "Failed to reset robotstix: %s", strerror ( errno ) );
    return 0;
  }
  usleep ( ( int ) ( 500 * 1e3 ) );

  // power on
  if ( ioctl ( fd, ROBOSTIX_IOCTL_POWER_VCC5, 1 ) != 0 ) {
    ERROR1 ( "Failed to reset robotstix: %s", strerror ( errno ) );
    return 0;
  }
  return 1;
}
//---------------------------------------------------------------------------
int CCBDriver::set7SegDot ( bool dot )
{
  if ( dot == true )
    m7SegByteValue |= 0x01;
  else
    m7SegByteValue &= 0xFE;

  return set7SegDisplay ( m7SegByteValue );
}
//---------------------------------------------------------------------------

} // namespace
