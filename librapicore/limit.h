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
#ifndef RAPILIMITS_H
#define RAPILIMITS_H

#include "rapivarinterface.h"

namespace Rapi
{

/**
 * This class provides limits, a max and min value
 * @author Jens Wawerla
 */
class CLimit : public IRapiVar
{
  public:
    /**
     * Default constructor
     * @param lower limit
     * @param upper limit
     */
    CLimit( const double lower = 0.0, const double upper = 0.0, std::string name = "Limit" );
    /** Copy constructor */
    CLimit(const CLimit& limit);
    /** Default destructor */
    ~CLimit();
    /**
     * Limits the variable to the limits of this class
     * @param var variable to apply limit to
     * @return limited variable
     */
    double limit(double var) const;
    /**
     * Sets a limit
     * @param lower limit
     * @param upper limit
     */
    void setLimit(const double lower, const double upper);
    /**
     * Gets the lower limit
     * @return lower limit
     */
    double getLowerLimit() const;
    /**
     * Gets the upper limit
     * @return upper limit
     */
    double getUpperLimit() const;
    /** Overloaded = operator */
    CLimit& operator= ( const CLimit& limt );
    /**
     * Prints the values to std out
     */
    void print() const;
    /**
     * Converts the limit data to a string
     * @return string
     */
    std::string toStr() const;
    /**
     * Gets the variable type string
     * @return variable type name
     */
    std::string getTypeStr() const { return "CVelocity2d"; };
    /**
     * Gets the data of the variable as a comma separated value string
     * @return comma separated value string
     */
    std::string toCSV() const;
    /**
     * Gets a header for the variable
     * @return header
     */
    std::string getCVSHeader() const;
  protected:
    /** Lower limit */
    double mLower;
    /** Upper limit */
    double mUpper;

};

} // namespace

#endif
