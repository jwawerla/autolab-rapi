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
#ifndef RAPIRECTANGLE_H
#define RAPIRECTANGLE_H

#include "point2d.h"
#include <string>

namespace Rapi
{

/**
 * Defines a rectangle
 * @author Jens Wawerla
 */
class CRectangle
{
  public:
    /**
     * Constructs a rectangle with (x, y) as its bottom-left corner and the
     * given width and height.
     * @param x coordinate
     * @param y coordinate
     * @param w width
     * @param h height
     */
    CRectangle( float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0 );
    /**
     * Constructs a rectangle with the given bottomLeft and topRight corners.
     * @param bottomLeft
     * @param topRight
     */
    CRectangle(CPoint2d bottomLeft, CPoint2d topRight);
    /**
     * Constructs a rectangle from another rectangle
     */
    CRectangle(const CRectangle& rect);
    /** Default destructor */
    ~CRectangle();
    /**
     * Gets the width of the rectangle
     * @return width
     */
    double getWidth() const;
    /**
     * Gets the height of the rectangle
     * @return height
     */
    double getHeight() const;
    /**
     * Gets the bottom left corner
     * @return bottom left
     */
    CPoint2d getBottomLeft() const;
    /**
     * Gets the top right corner
     * @return top right
     */
    CPoint2d getTopRight() const;
        /**
     * Prints the pose data to std out
     */
    void print() const;
    /**
     * Converts the pose data to a string
     * @return string
     */
    std::string toStr() const;
    /** Overloaded = operator */
    CRectangle& operator= ( const CRectangle& rect );
    /**
     * Checks if the rectangle is valid. A valid rectangle must have 
     * top > bottom and right > left
     * @return true is valid, false otherwise
     */
     bool isValid();
     /**
      * Sets the coordinates of the rectangle
      * @param (x1, y1) bottom left corner
      * @param (x2, y2) top right corner
      */
     void setCoordinates(float x1, float y1, float x2, float y2);
     /**
      * Sets the bottom left corner
      * @param point to be set
      */
     void setBottomLeft(CPoint2d point);
     /**
      * Sets the top right corner
      * @param point to be set
      */
     void setTopRight(CPoint2d point);
     /**
      * Checks if a point is inside the rectangle
      * @return true if inside, false otherwise
      */
     bool isInside(CPoint2d point);
     /**
      * Get the left side of the rectangle
      * @return left
      */
     double getLeft() const;
      /**
      * Get the right side of the rectangle
      * @return right
      */
     double getRight() const;
      /**
      * Get the top side of the rectangle
      * @return top
      */
     double getTop() const;
     /**
      * Get the bottom side of the rectangle
      * @return bottom
      */
     double getBottom() const;

  protected:
    /** bottom left corner */
    CPoint2d mBottomLeft;
    /** Width */
    double mWidth;
    /** Height */
    double mHeight;

};

} // namespace

#endif
