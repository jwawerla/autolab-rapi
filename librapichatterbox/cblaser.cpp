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

#include "cblaser.h"
#include "rapierror.h"
#include "printerror.h"
#include "utilities.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/poll.h>
#include <assert.h>

namespace Rapi
{

const int MAX_READINGS = 769;

//-----------------------------------------------------------------------------
CCBLaser::CCBLaser ( CCBDriver* driver, std::string devName )
    : ARangeFinder ( devName )
{
  float resolution;

  assert(driver);

  mCBDriver = driver;
  // Defaults to SCIP version 1
  mScpiVersion = 1;
  mFd = -1;
  mLaserPort = NULL;
  strncpy ( mLaserDevice, "/dev/ttyS0", 20 );
  mNumSamples = 654;
  mBaudRate = B115200;
  mMinRange = 0.0;
  mMaxRange = 4.0;
  mRangeData = new tRangeData[mNumSamples];
  mRelativeBeamPose = new CPose2d[mNumSamples];
  mFov = D2R ( 230.0 );
  resolution = mFov/ ( float ) ( mNumSamples );
  // fill beam position data structure
  for ( unsigned int i = 0; i < mNumSamples; i ++ ) {
    mRelativeBeamPose[i].mX   = 0;
    mRelativeBeamPose[i].mY   = 0;
    mRelativeBeamPose[i].mYaw = normalizeAngle ( -1* ( mFov/2.0 ) +
                                i * resolution );
  }
  mFgEnabled = false;
  setEnabled ( true );
  PRT_MSG0(4,"Waiting 10 sec. for laser to power up");
  sleep(10);
}
//-----------------------------------------------------------------------------
CCBLaser::~CCBLaser()
{
  setEnabled ( false );
  closePort();
}
//-----------------------------------------------------------------------------
void CCBLaser::setEnabled ( bool enable )
{
  if ( mCBDriver->setLowSideDriver ( CB_LASER_POWER, enable ) == 0 ) {
    ERROR0 ( "Failed to enable/disable laser" );
    return;
  }
  mFgEnabled = enable;
}

//-----------------------------------------------------------------------------
int CCBLaser::init()
{
  if ( openPort ( mLaserDevice, B115200 ) == 0 ) {
    ERROR1 ( "Failed to open port %s ", mLaserDevice );
    return 0;
  }

  if ( getSensorConfig() == 0 ) {
    ERROR0 ( "Failed to configure laser" );
    return 0;
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
void CCBLaser::updateData()
{
  if ( mFgEnabled ) {
    if ( readData() == 0 )
      ERROR0 ( "Failed to read data from device" );

    // update time stamp of this measurement
    mTimeStamp = timeStamp();
  }
}
//-----------------------------------------------------------------------------
int CCBLaser::readUntilNthOccurence ( int n, char c )
{
  int retval = 0;
  unsigned char Buffer[2];
  Buffer[0] = 0;
  Buffer[1] = 0;
  for ( int i = 0; i < n; i++ ) {
    do {
      retval = readUntil ( &Buffer[0], 1, -1 );
    } while ( Buffer[0] != c && retval > 0 );
  }
  return retval;
}
//-----------------------------------------------------------------------------
int CCBLaser::getSCIPVersion ()
{
  unsigned char Buffer [18];
  memset ( Buffer, 0, 18 );

  /////////////////
  // try SCIP1 first:
  /////////////////
  tcflush ( mFd, TCIFLUSH );
  fprintf ( mLaserPort, "V\n" );

  // check the returned command
  memset ( Buffer, 0, 18 );
  readUntil ( Buffer, 4, -1 );

  if ( strncmp ( ( const char * ) Buffer, "V\n0\n", 4 ) != 0 ) {
    // SCIP1.0 failed, so we test it with SCIP2.0:
    tcflush ( mFd, TCIFLUSH );
    fprintf ( mLaserPort, "VV\n" );

    // check the returned command
    memset ( Buffer, 0, 18 );
    readUntil ( Buffer, 7, -1 );
    tcflush ( mFd, TCIFLUSH );

    if ( strncmp ( ( const char * ) Buffer, "VV\n00P\n", 7 ) != 0 )  {
      ERROR1 ( "reading after VV command. Answer: %s", Buffer );
      return 0;
    }
    // Set SCIP version 2 and return
    mScpiVersion = 2;
    return 1; // success
  }
  // we are currently in SCIP 1.0
  else {
    Buffer[0] = 0;
    // Read the rest of the values, up till right before firmware version
    readUntilNthOccurence ( 2, ( char ) 0xa );
    // Read "FIRM:"
    memset ( Buffer, 0, 18 );
    readUntil ( Buffer, 5, -1 );

    if ( strncmp ( ( const char * ) Buffer, "FIRM:", 5 ) != 0 )
      PRT_WARN0 ( "Warning, 'FIRM:' is not where it is supposed to be!" );

    // Read the firmware version major value
    readUntil ( Buffer, 1, -1 );
    Buffer[1] = 0;
    int firmware = atol ( ( const char* ) Buffer );

    readUntilNthOccurence ( 4, ( char ) 0xa );
    if ( firmware < 3 ) {
      // Set SCIP version 1 and return
      mScpiVersion = 1;
      return 1; // success
    } else {
      // try to switch to SCIP2.0
      tcflush ( mFd, TCIFLUSH );
      fprintf ( mLaserPort, "SCIP2.0\n" );
      // check the returned command
      memset ( Buffer, 0, 18 );
      readUntil ( Buffer, 2, -1 );
      if ( strncmp ( ( const char * ) Buffer, "SC", 2 ) != 0 ) {
        // Set SCIP version 1 and return
        mScpiVersion = 1;
        return 1; // success
      } else {
        memset ( &Buffer[2], 0, 16 );
        readUntil ( &Buffer[2], 8, -1 );
        if ( strncmp ( ( const char * ) Buffer, "SCIP2.0\n0\n", 11 ) != 0 ) {
          // Set SCIP version 1 and return
          mScpiVersion = 1;
          return 1; // success
        }
        // Set SCIP version 2, turn laser on and return
        mScpiVersion = 2;
        fprintf ( mLaserPort, "BM\n" );
        readUntilNthOccurence ( 3, ( char ) 0xa );
        tcflush ( mFd, TCIFLUSH );
      }
    }
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBLaser::getSensorConfig ()
{
  float minAngle, maxAngle;

  if ( mLaserPort == NULL ) {
    ERROR0 ( "Laser port not open" );
    return 0;
  }

  if ( mScpiVersion == 1 ) {
    unsigned char Buffer[10];
    memset ( Buffer, 0, 10 );
    tcflush ( mFd, TCIFLUSH );
    // send the command
    fprintf ( mLaserPort, "V\n" );

    // check the returned command
    readUntil ( Buffer, 4, -1 );

    if ( strncmp ( ( const char * ) Buffer, "V\n0\n", 4 ) != 0 )  {
      ERROR1 ( "reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }
    // The following might not work on all versions of the hokuyos
    // since it reads out the Product description returned by 'V'
    readUntilNthOccurence ( 2, ( char ) 0xa );

    // Read FIRM:
    readUntil ( Buffer, 5, -1 );

    if ( strncmp ( ( const char * ) Buffer, "FIRM:", 5 ) == 0 ) {
      // Read the firmware version major value
      readUntil ( Buffer, 1, -1 );
      //Buffer[1] = 0;
      int firmware = atol ( ( const char* ) Buffer );

      if ( firmware < 3 ) {
        ERROR1 ( "Wrong firmware %d", firmware );
        readUntilNthOccurence ( 4, ( char ) 0xa );
        tcflush ( mFd, TCIFLUSH );
        return 0;
      }
    }

    readUntilNthOccurence ( 1, ( char ) '(' );
    readUntilNthOccurence ( 1, ( char ) '-' );

    int i = 0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
    } while ( Buffer[i++] != '[' );

    Buffer[i-1] = 0;
    int max_range = atol ( ( const char* ) Buffer );

    readUntilNthOccurence ( 2, ( char ) ',' );
    i = 0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
    } while ( Buffer[i++] != '-' );

    Buffer[i-1] = 0;
    int min_i = atol ( ( const char* ) Buffer );
    i = 0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
    } while ( Buffer[i++] != '[' );
    Buffer[i-1] = 0;

    int max_i = atol ( ( const char* ) Buffer );

    readUntil ( Buffer, 4, -1 );
    if ( strncmp ( ( const char * ) Buffer, "step", 4 ) != 0 ) {
      ERROR0 ( "reading angle_min_idx and angle_max_idx. Using an "\
               "older firmware?" );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }
    mMaxRange  = max_range / 1000.0;
    minAngle  = ( min_i-384 ) *mBeamConeAngle;
    maxAngle  = ( max_i-384 ) *mBeamConeAngle;
    PRT_MSG4 ( 1,"URG-04 specifications: [min_angle, max_angle, resolution, "\
               " max_range] = [%f, %f, %f, %f]",
               R2D ( minAngle ), R2D ( maxAngle ), R2D ( mBeamConeAngle ),
               mMaxRange );
    tcflush ( mFd, TCIFLUSH );
  } else { // mScpiVersion = 2
    // ask hokuyo: PP
    unsigned char Buffer[10];
    memset ( Buffer, 0, 10 );
    tcflush ( mFd, TCIFLUSH );

    // send the command
    fprintf ( mLaserPort, "PP\n" );
    // check the returned command
    readUntil ( Buffer,7, -1 );

    if ( strncmp ( ( const char * ) Buffer, "PP\n00P\n", 7 ) != 0 )  {
      ERROR1 ( "reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }
    int i = 0;
    readUntilNthOccurence ( 2, ( char ) 0xa );
    // read DMAX
    readUntilNthOccurence ( 1, ':' );
    do {
      readUntil ( &Buffer[i], 1, -1 );
      i++;
    } while ( Buffer[i-1] != ';' );
    Buffer[i-1] = 0;
    mMaxRange = atol ( ( const char* ) Buffer );
    mMaxRange /= 1000;

    // read angular resolution
    readUntilNthOccurence ( 1, ':' );
    i = 0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
      i++;
    } while ( Buffer[i-1] != ';' );
    Buffer[i-1] = 0;
    mBeamConeAngle = D2R ( 360.0 / atol ( ( const char* ) Buffer ) );

    // read AMIN
    readUntilNthOccurence ( 1, ':' );
    i = 0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
      i++;
    } while ( Buffer[i-1] != ';' );
    Buffer[i-1] = 0;
    minAngle = atol ( ( const char* ) Buffer );
    minAngle -= 384.0;
    minAngle *= mBeamConeAngle;

    // read AMAX
    readUntilNthOccurence ( 1, ':' );
    i=0;
    do {
      readUntil ( &Buffer[i], 1, -1 );
      i++;
    } while ( Buffer[i-1] != ';' );
    Buffer[i-1] = 0;
    maxAngle = atol ( ( const char* ) Buffer );
    maxAngle -= 384.0;
    maxAngle *= mBeamConeAngle;

    readUntilNthOccurence ( 4, ( char ) 0xa );

    PRT_MSG4 ( 1,"URG-04 specifications: [min_angle, max_angle, resolution, "\
               " max_range] = [%f, %f, %f, %f]",
               R2D ( minAngle ), R2D ( maxAngle ), R2D ( mBeamConeAngle ),
               mMaxRange );
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBLaser::readUntil ( unsigned char *buf, int len, int timeout )
{
  int ret;
  int current=0;
  struct pollfd ufd[1];
  int retval = 1;

  ufd[0].fd = mFd;
  ufd[0].events = POLLIN;

  do  {
    if ( timeout >= 0 ) {
      if ( ( retval = poll ( ufd, 1, timeout ) ) < 0 )  {
        ERROR1 ( "Failed to poll: %s", strerror ( errno ) );
        return 0;
      } else if ( retval == 0 ) {
        ERROR0 ( "Timeout" );
        return 0;
      }
    }

    ret = read ( mFd, &buf[current], len-current );
    if ( ret < 0 ) {
      ERROR1 ( "Failed reading: %s", strerror ( errno ) );
      return 0;
    }

    current += ret;
    if ( ( current > 2 ) && ( current < len )  && ( buf[current-2] == '\n' )
         && ( buf[current-1] == '\n' ) ) {
      ERROR0 ( "Got an end of command while waiting for more data, "\
               "this is bad." );
      return 0;
    }
  } while ( current < len );
  return len;
}
//-----------------------------------------------------------------------------
int CCBLaser::changeBaud ( int currBaud, int newBaud, int timeout )
{
  struct termios newtio;

  if ( tcgetattr ( mFd, &newtio ) < 0 ) {
    ERROR1 ( "tcgetattr(): %s", strerror ( errno ) );
    close ( mFd );
    return 0;
  }
  cfmakeraw ( &newtio );
  cfsetispeed ( &newtio, currBaud );
  cfsetospeed ( &newtio, currBaud );

  if ( tcsetattr ( mFd, TCSAFLUSH, &newtio ) < 0 ) {
    ERROR1 ( "tcsetattr():%s", strerror ( errno ) );
    close ( mFd );
    return 0;
  }

  unsigned char buf[17];
  memset ( buf,0,sizeof ( buf ) );

  // TODO: Check if this works with SCIP2.0
  if ( mScpiVersion == 1 ) {
    buf[0] = 'S';
    switch ( newBaud ) {
      case B19200:
        buf[1] = '0';
        buf[2] = '1';
        buf[3] = '9';
        buf[4] = '2';
        buf[5] = '0';
        buf[6] = '0';
        break;
      case B57600:
        buf[1] = '0';
        buf[2] = '5';
        buf[3] = '7';
        buf[4] = '6';
        buf[5] = '0';
        buf[6] = '0';
        break;
      case B115200:
        buf[1] = '1';
        buf[2] = '1';
        buf[3] = '5';
        buf[4] = '2';
        buf[5] = '0';
        buf[6] = '0';
        break;
      default:
        ERROR1 ( "unknown baud rate %d", newBaud );
        return 0;
    }
    buf[7] = '0';
    buf[8] = '0';
    buf[9] = '0';
    buf[10] = '0';
    buf[11] = '0';
    buf[12] = '0';
    buf[13] = '0';
    buf[14] = '\n';
  } else { // SCIP 2
    buf[0] = 'S';
    buf[1] = 'S';
    switch ( newBaud ) {
      case B19200:
        buf[2] = '0';
        buf[3] = '1';
        buf[4] = '9';
        buf[5] = '2';
        buf[6] = '0';
        buf[7] = '0';
        break;
      case B57600:
        buf[2] = '0';
        buf[3] = '5';
        buf[4] = '7';
        buf[5] = '6';
        buf[6] = '0';
        buf[7] = '0';
        break;
      case B115200:
        buf[2] = '1';
        buf[3] = '1';
        buf[4] = '5';
        buf[5] = '2';
        buf[6] = '0';
        buf[7] = '0';
        break;
      default:
        ERROR1 ( "unknown baud rate %d", newBaud );
        return 0;
    }
    buf[8] = '\n';
  }

  fprintf ( mLaserPort, "%s", buf );
  memset ( buf, 0, sizeof ( buf ) );
  int len;
  // The docs say that the response ends in 'status LF LF', where
  // status is '0' if everything went alright.  But it seems that
  // the response actually ends in 'LF status LF'.
  if ( ( ( len = readUntil ( buf, sizeof ( buf ), timeout ) ) < 0 ) ||
       ( buf[15] != '0' ) ) {
    ERROR0 ( "failed to change baud rate" );
    return 0;
  } else {
    if ( tcgetattr ( mFd, &newtio ) < 0 ) {
      ERROR1 ( "tcgetattr(): %s", strerror ( errno ) );
      close ( mFd );
      return 0;
    }
    cfmakeraw ( &newtio );
    cfsetispeed ( &newtio, newBaud );
    cfsetospeed ( &newtio, newBaud );
    if ( tcsetattr ( mFd, TCSAFLUSH, &newtio ) < 0 ) {
      ERROR1 ( "tcsetattr(): %s", strerror ( errno ) );
      close ( mFd );
      return 0;
    } else {
      usleep ( 200000 );
    }
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBLaser::openPort ( const char * portName, int baud )
{
  if ( mLaserPort != NULL )
    closePort ();

  mLaserPort = fopen ( portName, "r+" );
  if ( mLaserPort == NULL ) {
    ERROR2 ( "Failed to open port %s: %s",
             portName, strerror ( errno ) );
    return 0;
  }

  mFd = fileno ( mLaserPort );
  PRT_MSG0 ( 1,"Trying to connect at 19200" );
  if ( changeBaud ( B19200, baud, 100 ) == 0 ) {
    PRT_MSG0 ( 1,"Trying to connect at 57600" );
    if ( changeBaud ( B57600, baud, 100 ) == 0 ) {
      PRT_MSG0 ( 1, "Trying to connect at 115200" );
      if ( changeBaud ( B115200, baud, 100 ) == 0 ) {
        ERROR0 ( "failed to connect at any baud" );
        close ( mFd );
        return 0;
      }
    }
  }
  switch ( baud ) {
    case B115200:
      PRT_MSG0 ( 1,"Successfully changed baud rate to 115200" );
      break;
    case B57600:
      PRT_MSG0 ( 1,"Successfully changed baud rate to 57600" );
      break;
    case B19200:
      PRT_MSG0 ( 1,"Successfully changed baud rate to 19200" );
      break;
  } // switch
  return 1; // success
}

//-----------------------------------------------------------------------------
int CCBLaser::closePort ()
{
  int retval;

  if ( mLaserPort ) {
    tcflush ( mFd, TCIOFLUSH );
    retval = fclose ( mLaserPort );
    mLaserPort = NULL;
    return ( retval );
  }
  return 0; // failure
}
//-----------------------------------------------------------------------------
int CCBLaser::readData ()
{
  int min_i, max_i;
  unsigned int k;
  unsigned char Buffer[16];
  unsigned short range;

  // it seems the readings outside this range a in valid
  min_i = 57;
  max_i = 771;

  if ( mLaserPort == NULL ) {
    ERROR0 ( "Serial port to laser not open" );
    return 0;
  }

  if ( mScpiVersion == 1 ) {
    tcflush ( mFd, TCIFLUSH );
    // send the command, request data
    fprintf ( mLaserPort, "G00076801\n" );
    // check the returned command
    readUntil ( Buffer, 10, -1 );

    if ( strncmp ( ( const char * ) Buffer, "G00076801", 9 ) != 0 ) {
      ERROR1 ( "Error reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }

    // check the returned status
    readUntil ( Buffer, 2, -1 );
    if ( Buffer[0] != '0' ) {
      ERROR0 ( "Failed to receive correct status byte from laser" );
      return 0;
    }

    for ( int i=0; ; ++i ) {
      readUntil ( Buffer, 2, -1 );

      if ( Buffer[0] == '\n' && Buffer[1] == '\n' ) {
        break;  // received LF LF, done reading get out of here
      } else if ( Buffer[0] == '\n' ) {
        Buffer[0] = Buffer[1];
        if ( readUntil ( &Buffer[1], 1, -1 ) < 0 ) {
          ERROR0 ( "Undefined error" );
          return 0;
        }
      }

      if ( i < MAX_READINGS ) {
        k = i-min_i;

        if ( ( k >= 0 ) && ( k < mNumSamples ) ) {
          // read 2 bytes from the buffer
          range = ( ( Buffer[0]-0x30 ) << 6 ) | ( Buffer[1]-0x30 );
          // check if the range is at the maximum sensor range, for some
          // wired reason the URG returns a value < 20 in that case
          if ( range < 20 )
            mRangeData[k].range = mMaxRange;
          else
            mRangeData[k].range = ( float ) ( range ) / 1000.0;
          //printf("%d -> %d (%f) \n", k, range, mRangeData[k].range);
        }
      } else
        ERROR1 ( "Got too many readings! %d",i );

    }
  } else { // mScpiVersion == 2
    printf ( "SCPI VERSION 2 !!!!!\n" );
    tcflush ( mFd, TCIFLUSH );
    // send the command
    fprintf ( mLaserPort, "GD0000076801\n" );
    // check the returned command
    readUntil ( Buffer, 13, -1 );

    if ( strncmp ( ( const char * ) Buffer, "GD0000076801", 12 ) != 0 ) {
      ERROR1 ( "Error reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }

    // check the returned status
    readUntil ( Buffer, 3, -1 );
    Buffer[2] = 0;
    if ( Buffer[0] != '0' || Buffer[1] != '0' )
      return ( Buffer[0] - '0' ) *10 + ( Buffer[1] - '0' );

    readUntilNthOccurence ( 2, ( char ) 0xa );

    // NOTE: This only works for 769 requested samples.. (64 data bytes
    // blocks are not the best choice for 3-byte values...)
    for ( int i = 0; ; ++i ) {
      if ( ( Buffer[1] == '\n' ) && ( Buffer[2] == '\n' ) ) {
        break;
      } else if ( Buffer[2] == '\n' ) {
        if ( readUntil ( &Buffer[1], 2, -1 ) < 0 )
          return 0;
      } else if ( Buffer[0] == '\n' )  {
        if ( i <= MAX_READINGS ) {
          range = ( unsigned short ) ( mRangeData[i-1].range*1000 );
          range = ( ( range & 0xFFC0 ) | ( Buffer[1]-0x30 ) );
          mRangeData[i].range = ( float ) ( range ) / 1000.0;
          Buffer [0] = Buffer [2];
          if ( readUntil ( &Buffer[1], 2, -1 ) < 0 )
            return 0;
        } else
          ERROR1 ( "Got too many readings! %d",i );
      } else if ( Buffer[1] == '\n' ) {
        Buffer[0] = Buffer[2];
        if ( readUntil ( &Buffer[1], 2, -1 ) < 0 )
          return 0;
      }

      if ( i < MAX_READINGS ) {
        mRangeData[i].range = ( ( Buffer[0]-0x30 ) << 12 ) |
                              ( ( Buffer[1]-0x30 ) << 6 ) |
                              ( Buffer[2]-0x30 );
        if ( ( mRangeData[i].range > 5.6 ) && ( i >= min_i )
             && ( i <= max_i ) )
          PRT_WARN2 ( "[%d] read error: %f is bigger than 5.6 meters\n",
                      i, mRangeData[i].range );
      } else
        ERROR1 ( "Got too many readings! %d",i );
    }
  }
  return 1; // success
}
//-----------------------------------------------------------------------------
int CCBLaser::getIDInfo ()
{
  unsigned char Buffer [18];
  memset ( Buffer, 0, 18 );
  int i;
  int id;
  if ( mLaserPort == NULL )
    return 0;

  tcflush ( mFd, TCIFLUSH );

  if ( mScpiVersion == 1 ) {
    // send the command
    fprintf ( mLaserPort, "V\n" );
    // check the returned command
    readUntil ( Buffer, 2, -1 );

    if ( strncmp ( ( const char * ) Buffer, "V", 1 ) != 0 ) {
      ERROR1 ( "Error reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }
    // check the returned status
    readUntil ( Buffer, 2, -1 );
    if ( Buffer[0] != '0' )
      return 0;

    Buffer[0] = 0;
    // Read the rest of the values
    for ( i = 0; i < 4; i++ ) {
      do {
        readUntil ( &Buffer[0], 1, -1 );
      } while ( Buffer[0] != 0xa );
    }

    // Read "SERI:H"
    readUntil ( Buffer, 6, -1 );
    // Read the serial number value
    for ( i = 0; ; i++ ) {
      readUntil ( &Buffer[i], 1, -1 );
      if ( Buffer[i] == 0xa )
        break;
    }

    id = atol ( ( const char* ) Buffer );
    // Read the last LF
    readUntil ( Buffer, 1, -1 );
  } else { // mScpiVersion == 2
    // send the command
    fprintf ( mLaserPort, "VV\n" );
    // check the returned command
    readUntil ( Buffer, 7, -1 );

    if ( strncmp ( ( const char * ) Buffer, "VV\n00P\n", 7 ) != 0 ) {
      ERROR1 ( "Error reading command result: %s", Buffer );
      tcflush ( mFd, TCIFLUSH );
      return 0;
    }

    Buffer[0] = 0;
    // Read the rest of the values
    for ( i = 0; i < 4; i++ ) {
      do {
        readUntil ( &Buffer[0], 1, -1 );
      } while ( Buffer[0] != 0xa );
    }

    // Read "SERI:H"
    readUntil ( Buffer, 6, -1 );
    // Read the serial number value
    for ( i = 0; ; i++ ) {
      readUntil ( &Buffer[i], 1, -1 );
      if ( Buffer[i] == ';' ) {
        Buffer[i] = 0;
        break;
      }
    }
    id = atol ( ( const char* ) Buffer );

    readUntil ( Buffer, 3, -1 );
  }
  return id;
}
//----------------------------------------------------------------------------
void CCBLaser::print()
{
  printf ( "Laser: " );
  printf ( "%01.1f ", mRangeData[20].range );
  printf ( "\n" );
}
//----------------------------------------------------------------------------

} // namespace

