/***************************************************************************
 * Project: autolab-wp                                                     *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: ccolor.h,v 1.1.1.1 2009-03-15 03:52:02 jwawerla Exp $
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

#ifndef RAPIRGBCOLOR_H
#define RAPIRGBCOLOR_H

#include "rapivarinterface.h"

namespace Rapi
{

/**
 * Defines a rgb color
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 12/2007
 */
class CRgbColor : public IRapiVar
{
  public:
    /**
     * Default constructor
     * @param r red
     * @param g green
     * @param b blue
     */
    CRgbColor ( unsigned char r=0, unsigned char g=0, unsigned char b=0 );
    /**
     * Copy constructor
     */
    CRgbColor ( CRgbColor const &color );
    /** Default destructor */
    ~CRgbColor();
    /**
     * Prints the color value
     */
    void print() const;
    /**
     * Gets the data of the varible as a string
     * @return string data
     */
    std::string toStr() const;
    /**
     * Gets the variable type string
     * @return variable type name
     */
    std::string getTypeStr() const { return "CRgbColor"; };
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

    /** Assignment operator */
    CRgbColor& operator = ( const CRgbColor &color );
    /** Overloaded == operator */
    bool operator== ( const CRgbColor color );
    /** Red value */
    unsigned char mRed;
    /** Green value */
    unsigned char mGreen;
    /** Blue value */
    unsigned char mBlue;


};

/** Define some handy colors */
const CRgbColor RED ( 255,0,0 );
const CRgbColor GREEN ( 0,255,0 );
const CRgbColor BLUE ( 0,0,255 );
const CRgbColor WHITE ( 255,255,255 );
const CRgbColor BLACK ( 0,0,0 );

} // namespace

#endif
