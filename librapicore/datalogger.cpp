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
#include "datalogger.h"
#include <assert.h>
#include "printerror.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CDataLogger::CDataLogger( std::string filename, tWriteMode overwrite,
                          std::string commentSymbole )
    : CRapiTypeSystem()
{
  mLogInterval = 0.0;
  mLastWriteTimestamp = 0.0;
  mCommentSymbole = commentSymbole;
  mFilename = filename;
  if ( overwrite == OVERWRITE ) {
    mFp = fopen( filename.c_str(), "w" );
  }
  else {
    mFp = fopen( filename.c_str(), "a" );
  }
  if ( mFp == NULL ) {
    PRT_ERR1( "Failed to open file %s", filename.c_str() );
  }
  assert( mFp );

  mFgHeader = false;
}
//-----------------------------------------------------------------------------
CDataLogger::~CDataLogger()
{
}
//-----------------------------------------------------------------------------
CDataLogger* CDataLogger::getInstance( std::string filename,
                                       tWriteMode overwrite,
                                       std::string commentSymbole )
{
  static CDataLogger* instance = NULL;

  if ( instance == NULL )
    instance = new CDataLogger( filename, overwrite, commentSymbole );

  return instance;
}
//-----------------------------------------------------------------------------
void CDataLogger::setInterval( float dt )
{
  mLogInterval = dt;
}
//-----------------------------------------------------------------------------
void CDataLogger::write( float timestamp )
{
  tVarEntry entry;
  int intVar;
  bool boolVar;
  float floatVar;
  double doubleVar;
  CPose2d poseVar;
  CPoint2d pointVar;
  CVelocity2d velocityVar;
  CWaypoint2d waypointVar;


  if ( timestamp - mLastWriteTimestamp  < mLogInterval )
    return; // no loging necessary

  mLastWriteTimestamp = timestamp;
  if ( not mFgHeader ) {
    writeHeader();
    mFgHeader = true;
  }
  fprintf( mFp, "%0.1f", timestamp );

  for ( unsigned int i = 0; i < mVarList.size(); i++ ) {
    entry = mVarList[i];
    switch ( entry.varType ) {
      case POINT2D:
        pointVar = * (( CPoint2d* ) entry.ptr );
        fprintf( mFp, ",%f,%f", pointVar.mX, pointVar.mY );
        break;
      case WAYPOINT2D:
        waypointVar = * (( CWaypoint2d* ) entry.ptr );
        fprintf( mFp, ",%s,%f,%f,%f", waypointVar.getLabel().c_str(),
                 waypointVar.getPose().mX,
                 waypointVar.getPose().mY,
                 waypointVar.getPose().mYaw );
        break;
      case BOOL:
        boolVar = * (( bool* ) entry.ptr );
        fprintf( mFp, ",%d", boolVar );
        break;
      case FLOAT:
        floatVar = * (( float* ) entry.ptr );
        fprintf( mFp, ",%s", toDoubleToStr( floatVar, entry.decimalPlaces ).c_str() );
        break;
      case DOUBLE:
        doubleVar = * (( double* ) entry.ptr );
        fprintf( mFp, ",%s", toDoubleToStr( doubleVar, entry.decimalPlaces ).c_str() );
        break;
      case INT:
        intVar = * (( int* ) entry.ptr );
        fprintf( mFp, ",%d", intVar );
        break;
      case RAPI_VAR:
        fprintf( mFp, ",%s", (( IRapiVar* ) entry.ptr )->toCSV().c_str() );
        break;
    } // switch

  }

  fprintf( mFp, "\n" );
  // make sure everything is on disk
  fflush( mFp );
}
//-----------------------------------------------------------------------------
void CDataLogger::writeHeader() const
{
  tVarEntry entry;

  // Write comment symbole
  fprintf( mFp, "%s", mCommentSymbole.c_str() );

  for ( unsigned int i = 0; i < mVarList.size(); i++ ) {
    entry = mVarList[i];
    switch ( entry.varType ) {
      case RAPI_VAR:
        fprintf( mFp, "%s", (( IRapiVar* ) entry.ptr )->getCVSHeader().c_str() );
        break;
      default:
        fprintf( mFp, "%s", mVarList[i].name.c_str() );
        break;
    } // switch
    if ( i + 1  < mVarList.size() )
      fprintf( mFp, "," );
  } // for

  fprintf( mFp, "\n" );
  // make sure everything is on disk
  fflush( mFp );
}
//-----------------------------------------------------------------------------
} // namespace

