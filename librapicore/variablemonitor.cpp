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
#include "variablemonitor.h"
#include <sstream>

namespace Rapi
{

//-----------------------------------------------------------------------------
CVariableMonitor::CVariableMonitor()
{
}
//-----------------------------------------------------------------------------
CVariableMonitor::~CVariableMonitor()
{
}
//-----------------------------------------------------------------------------
void CVariableMonitor::getVariableString ( unsigned int index,
                                           std::string& varType,
                                           std::string& name,
                                           std::string& value )
{
  tVarEntry entry;
  int intVar;
  bool boolVar;
  float floatVar;
  double doubleVar;
  std::ostringstream strOut;

  if ( index >= mVarList.size() ) {
    name = "unknown";
    value = "unknown";
    varType = "unknown";
    return;
  }

  entry = mVarList[index];

  name = entry.name;

  switch ( entry.varType ) {

    case BOOL:
      varType = "bool";
      boolVar = * ( ( bool* ) entry.ptr );
      if ( boolVar )
        value = "true";
      else
        value = "false";
      break;
    case FLOAT:
      varType = "float";
      floatVar = * ( ( float* ) entry.ptr );
      strOut << floatVar;
      value = strOut.str();
      break;
    case DOUBLE:
      varType = "double";
      doubleVar = * ( ( double* ) entry.ptr );
      strOut << doubleVar;
      value = strOut.str();
      break;
    case INT:
      varType = "int";
      intVar = * ( ( int* ) entry.ptr );
      strOut << intVar;
      value = strOut.str();
      break;
  };
}
//-----------------------------------------------------------------------------
void CVariableMonitor::addVar ( float* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.varType = FLOAT;
  mVarList.push_back ( entry );
}
//-----------------------------------------------------------------------------
void CVariableMonitor::addVar ( bool* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.varType = BOOL;
  mVarList.push_back ( entry );
}
//-----------------------------------------------------------------------------
void CVariableMonitor::addVar ( double* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.varType = DOUBLE;
  mVarList.push_back ( entry );
}
//-----------------------------------------------------------------------------
void CVariableMonitor::addVar ( int* ptr, std::string name )
{
  tVarEntry entry;
  entry.ptr = ptr;
  entry.name = name;
  entry.varType = INT;
  mVarList.push_back ( entry );
}
//-----------------------------------------------------------------------------
} // namespace
