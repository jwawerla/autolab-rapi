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

#ifndef RAPIRAPIVARIABLE_H
#define RAPIRAPIVARIABLE_H

#include "rapivarinterface.h"
#include "robotupdateinterface.h"
#include "robot.h"
#include <assert.h>
#include "printerror.h"
#include <sstream>
#include <typeinfo>

namespace Rapi
{

/**
 * Template class for variables that keep track of their modificiations
 * @author Jens Wawerla
 */
template<class T>
class CRapiVariable : public IRapiVar, public IRobotUpdate
{
  public:
    /**
     * Default constructor
     */
    CRapiVariable() {
      mRobot = NULL;
      mName = "noname";
      mModificationTimestamp = 0.0;
      mFgModified = false;
    };
    /** Default destructor */
    virtual ~CRapiVariable() {};
    /**
     * Sets a robot for this variable to handle automatic time tracking
     * @param robot this variable is used by
     */
    void setRobot ( ARobot* robot ) {
      assert ( robot );
      mRobot = robot;
      mRobot->registerStateVariable ( this );
      mModificationTimestamp = robot->getCurrentTime();
    };
    /**
     * Sets the name of a variable
     * @param name to be set
     */
    void setName(std::string name) {
       mName = name;
    }
    /**
     * Checks if variable has changed since last time step
     * @return true if changed, false other wise
     */
    bool hasChanged() const { return mFgModified; };
    /**
     * Gets the time that has elapsed since the last change of the variable
     * @return [s]
     */
    double getElapsedTimeSinceChange() const {
      return mRobot->getCurrentTime() - mModificationTimestamp;
    };
    /**
     * Gets the time of the last modification
     * @return [s]
     */
    double getTimeOfChange() const { return mModificationTimestamp; };
    /**
     * Gets the data of the varible as a string
     * @return string data
     */
    std::string toStr() const {
      std::ostringstream strOut;
      strOut << "value=" << mValue << " since " <<
      (mRobot->getCurrentTime() - mModificationTimestamp);
      return strOut.str();
    };
    /**
     * Gets the variable type string
     * @return variable type name
     */
    std::string getTypeStr() const {
      std::ostringstream strOut;
      strOut << typeid ( T ).name();
      return "CRapiVariable<" + strOut.str() + ">";
    };
    /**
     * Gets the data of the variable as a comma separated value string
     * @return comma separated value string
     */
    std::string toCSV() const {
      std::ostringstream strOut;
      strOut << mValue;
      return strOut.str();
    }
    /**
     * Gets a header for the variable 
     * @return header
     */
    std::string getCVSHeader() const {
      return mName;
    }
    /**
     * Gets the value of the variable
     * @return value of the actual variable
     */
    T value () { return mValue; };
    /** Overloaded ! operator */
    T operator! () { return !mValue; };
    /** Overloaded = operator */
    T operator= ( const T value ) {
      if ( not mRobot )
        PRT_ERR0 ( "No robot available, use setRobot()" );
      assert ( mRobot );

      if ( mValue != value ) {
        mModificationTimestamp = mRobot->getCurrentTime();
        mFgModified = true;
        mValue = value;
      }

      return mValue;
    };
    /** Overloaded != operator */
    bool operator== ( const T value ) {
      if ( mValue == value )
        return true;
      return false;
    };
    /** Overloaded != operator */
    bool operator!= ( const T value ) {
      if ( mValue == value )
        return false;
      return true;
    };


  protected:
    /** Robot this variable is used by */
    ARobot* mRobot;
    /**
     * Called by the frame work to update the state variable
     * @param dt time since last call [s]
     */
    virtual void updateData ( float dt ) {
      if ( mRobot->getCurrentTime() - mModificationTimestamp > dt )
        mFgModified = false;
    };
    /** Value of the variable */
    T mValue;
    /** Time stamp of last modification [s] */
    double mModificationTimestamp;
    /** Flags if the value of the variable has been modified or not */
    bool mFgModified;
};

}  // namespace

#endif
