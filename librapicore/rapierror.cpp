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
#include "rapierror.h"
#include <stdio.h>
#include <sstream>
#include <stdarg.h>

namespace Rapi
{

/** Only instance of this class */
CRapiError* instance = NULL;

//-----------------------------------------------------------------------------
CRapiError::CRapiError()
{
}
//-----------------------------------------------------------------------------
CRapiError::~CRapiError()
{
}
//-----------------------------------------------------------------------------
CRapiError* CRapiError::getInstance()
{
  if ( instance == NULL )
    instance = new CRapiError();

  return instance;
}
//-----------------------------------------------------------------------------
bool CRapiError::hasError()
{
  return !mErrorMsgStack.empty();
}
//-----------------------------------------------------------------------------
void CRapiError::clear()
{
  while ( mErrorMsgStack.empty() == false ) {
    mErrorMsgStack.pop();
  }
}
//-----------------------------------------------------------------------------
void CRapiError::print()
{
  tError err;

  if ( mErrorMsgStack.empty() ) {
    fprintf ( stderr, "no errors\n" );
    return;
  }

  while ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
    fprintf ( stderr, "\033[1;41;32mERROR\033[0m:" );
    fprintf ( stderr, "%s \n", err.errorStr.c_str() );
    fprintf ( stderr, "  %s \n", err.codeRef.c_str() );
  }
}
//-----------------------------------------------------------------------------
tError CRapiError::getLast()
{
  tError err;

  if ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
  }

  return err;
}
//-----------------------------------------------------------------------------
std::string CRapiError::getMsg()
{
  tError err;
  std::string errorStr;

  if ( mErrorMsgStack.empty() ) {
    return "no errors";
  }

  while ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
    errorStr += err.errorStr;

    if ( mErrorMsgStack.empty() == false )
      errorStr += "\n";
  }
  return errorStr;
}
//-----------------------------------------------------------------------------
std::string CRapiError::getLastMsg()
{
  tError err;

  if ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
    return err.errorStr;
  }

  return "no error";
}
//-----------------------------------------------------------------------------
std::string CRapiError::getFullMsg()
{
  tError err;
  std::string errorStr;

  if ( mErrorMsgStack.empty() ) {
    return "no errors";
  }

  while ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
    errorStr += err.errorStr + "\n  " + err.codeRef;

    if ( mErrorMsgStack.empty() == false )
      errorStr += "\n";
  }
  return errorStr;
}
//-----------------------------------------------------------------------------
std::string CRapiError::getLastFullMsg()
{
  tError err;

  if ( mErrorMsgStack.empty() == false ) {
    err = mErrorMsgStack.top();
    mErrorMsgStack.pop();
    return err.errorStr + "\n  " + err.codeRef;
  }

  return "no error";
}
//-----------------------------------------------------------------------------
void CRapiError::set ( std::string info )
{
  tError newErr;

  newErr.codeRef = "";
  newErr.errorStr = info;

  mErrorMsgStack.push ( newErr );
}
//-----------------------------------------------------------------------------
void CRapiError::set ( const char *file, int line, const char *fmt, ... )
{
  tError newErr;
  std::stringstream fileOut;
  std::stringstream lineOut;
  std::stringstream infoOut;
  char buffer[1024];
  std::string errStr;
  va_list ap;

  va_start ( ap, fmt );
  vsnprintf ( buffer, 1024, fmt, ap );
  va_end ( ap );

  lineOut << line;
  fileOut << file;
  infoOut << buffer;
  newErr.errorStr = infoOut.str();
  newErr.codeRef = fileOut.str() + ": " + lineOut.str();

  mErrorMsgStack.push ( newErr );

  return;
}
//-----------------------------------------------------------------------------
void CRapiError::set ( std::string file, int line, std::string info )
{
  tError newErr;
  std::stringstream lineOut;

  lineOut << line;
  newErr.errorStr =  info;
  newErr.codeRef = file + ":" + lineOut.str();

  mErrorMsgStack.push ( newErr );
}
//-----------------------------------------------------------------------------


} // namespace
