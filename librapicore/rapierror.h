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
#ifndef RAPIRAPIERROR_H
#define RAPIRAPIERROR_H

#include <stdio.h>
#include <string>
#include <stack>

namespace Rapi
{

/** Type definition for the stack */
typedef struct {
  /** Text desribtion of error */
  std::string errorStr;
  /** Source code reference */
  std::string codeRef;
} tError;

/**
 * This class contains information about the last errors. All errors are
 * put on a stack for the uses to analyse.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CRapiError
{
  public:
    /** Default destructor */
    ~CRapiError();
    /** Gets the only instance of this class */
    static CRapiError* getInstance();
    /**
     * Checks if we have an error
     * @return true if error on stack, false otherwise
     */
    bool hasError();
    /**
     * Gets all error messages and clears the stack
     * @return message
     */
    std::string getMsg();
    /**
     * Gets the last error message and removes it from the stack
     * @return message
     */
    std::string getLastMsg();
    /**
     * Gets all error messages including source code references and clears
     * the stack.
     * @return message
     */
    std::string getFullMsg();
    /**
     * Gets the last error message including source code references and removes
     * it from the stack.
     * @return message
     */
    std::string getLastFullMsg();
    /**
     * Gets the last error and removes it from the stack
     * @return error
     */
    tError getLast();
    /**
     * Prints all error messages and clears the stack
     */
    void print();
    /**
     * Creates a new error and overrided older ones
     * @param info information about error
     */
    void set ( std::string info );
    /**
     * Creates a new error and overrided older ones
     * @param file filename in which the error occured
     * @param line source code line of error
     * @param info information about error
     */
    void set ( std::string file, int line, std::string info );
    /**
     * Creates a new error and overrided older ones
     * @param file filename in which the error occured
     * @param line source code line of error
     */
    void set ( const char *file, int line, const char *fmt, ... );
    /**
     * Clears all errors in the stack
     */
    void clear();



  private:
    /** Default constructor */
    CRapiError();
    /** Stack of error messages */
    std::stack<tError> mErrorMsgStack;

};

/** Global instance of the error class */
static CRapiError* rapiError = CRapiError::getInstance();

/** Convenience functions for setting errors */
#define ERROR0( msg) rapiError->set( __FILE__, __LINE__,msg);
#define ERROR1( msg, a) rapiError->set( __FILE__, __LINE__,msg, a);
#define ERROR2( msg, a, b) rapiError->set( __FILE__, __LINE__,msg, a, b);
#define ERROR3( msg, a, b, c) rapiError->set( __FILE__, __LINE__,msg, a, b);
#define ERROR4( msg, a, b, c, d) rapiError->set( __FILE__, __LINE__,msg, a, b);
#define ERROR5( msg, a, b, c, d, e) rapiError->set( __FILE__, __LINE__,msg, a, b);
}

#endif
