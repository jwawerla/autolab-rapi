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
#include "rapitypesystem.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CRapiTypeSystem::CRapiTypeSystem()
{
}
//-----------------------------------------------------------------------------
CRapiTypeSystem::~CRapiTypeSystem()
{
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( std::string* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = STRING;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( IRapiVar* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = RAPI_VAR;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( CWaypoint2d* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = WAYPOINT2D;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
/*
void CRapiTypeSystem::addVar ( CPose2d* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.varType = POSE2D;
  mVarList.push_back ( entry );
}
*/
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( CPoint2d* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = POINT2D;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
/*
void CRapiTypeSystem::addVar ( CVelocity2d* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.varType = VELOCITY2D;
  mVarList.push_back ( entry );
}
*/
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( float* ptr, std::string name,
                              unsigned int decimals )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = decimals;
  entry.varType = FLOAT;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( bool* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = BOOL;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( double* ptr, std::string name,
                              unsigned int decimals )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = decimals;
  entry.varType = DOUBLE;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar ( unsigned int* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = UINT;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
void CRapiTypeSystem::addVar( int* ptr, std::string name )
{
  tVarEntry entry;

  entry.ptr = ptr;
  entry.name = name;
  entry.decimalPlaces = 0;
  entry.varType = INT;
  mVarList.push_back( entry );
}
//-----------------------------------------------------------------------------
std::string CRapiTypeSystem::toDoubleToStr( double value, unsigned int decimals )
{
  char str[30];

  switch ( decimals ) {
    case 0: snprintf( str, 30, "%0.0lf", value ); break;
    case 1: snprintf( str, 30, "%0.1lf", value ); break;
    case 2: snprintf( str, 30, "%0.2lf", value ); break;
    case 3: snprintf( str, 30, "%0.3lf", value ); break;
    case 4: snprintf( str, 30, "%0.4lf", value ); break;
    case 5: snprintf( str, 30, "%0.5lf", value ); break;
    case 6: snprintf( str, 30, "%0.6lf", value ); break;
    case 7: snprintf( str, 30, "%0.7lf", value ); break;
    case 8: snprintf( str, 30, "%0.8lf", value ); break;
    case 9: snprintf( str, 30, "%0.9lf", value ); break;
    default:
      snprintf( str, 30, "%lf", value );
      break;
  } // switch

  return str;
}
//-----------------------------------------------------------------------------
} // namespace
