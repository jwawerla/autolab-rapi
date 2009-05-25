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

namespace Rapi
{

/**
 * This class provides limits, a max and min value
 * @author Jens Wawerla
 */
class CLimit
{
  public:
    /**
     * Default constructor
     * @param lower limit
     * @param upper limit
     */
    CLimit( const double lower = 0.0, const double upper = 0.0 );
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

  protected:
    /** Lower limit */
    double mLower;
    /** Upper limit */
    double mUpper;

};

} // namespace

#endif
