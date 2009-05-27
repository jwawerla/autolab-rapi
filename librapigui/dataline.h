/*****************************************************************************
 * Project: Forever
 * File:    dataline.h
 * Author:  Jens Wawerla (jwawerla_at_sfu.ca)
 *
 *****************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ******************************************************************************
 * $Log: dataline.h,v $
 * Revision 1.1  2009-03-23 06:24:22  jwawerla
 * RobotVis works now for data, maps still need more work
 *
 *
 *****************************************************************************/

#ifndef _DATA_LINE_H_
#define _DATA_LINE_H_

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QLineEdit>
/**
 * This class combines QLineEdit and QLabel to create an easier to use display
 * widget with a corresponding label.
 * @author J.Wawerla (jwawerla@sfu.ca)
 * @version 0.1
 */
class CDataLine : public QWidget
{
    Q_OBJECT

  public:
    /**
     * Default constructor
     * @param parent widget
     * @param name of widget
     * @param caption of data line
     */
    CDataLine ( QWidget* parent = 0, QString caption = "NoName" );
    /** Default destructor */
    ~CDataLine();
    typedef enum {LEFT, RIGHT, CENTER} t_alignment;

    /** Set caption */
    void setCaption ( QString caption );
    /**
     * Sets the geometry of the widget
     * @param x position
     * @param y position
     * @param w width
     * @param h hight
     */
    void setGeometry ( int x, int y, int w, int h );
    /**
     * Sets the data to be displayed
     * @param data to display
     */
    void setData ( double data );
    /** Overwritten for convenience */
    void setData ( int data );
    /** Overwritten for convenience */
    void setData ( unsigned int data );
    /** Overwritten for convenience */
    void setData ( QString data );
    /**
     * Sets the alignment
     * @param align, LEFT for left alignment, RIGHT for right alignment
     */
    void setAlignment ( t_alignment align );
    /**
     * Sets the precision for displaying floating point numbers, by default
     * the precision is set to 1
     * @param p number of decimals for floating points
     */
    void setFloatPrecision ( unsigned int p = 1 );
    /**
     * Enables color coded output of signed values
     * @param true enable, false disable
     */
    void setColorEnable(bool on);

  protected:
    /** Shows the label */
    QLabel* mLabel;
    /** Displays the data */
    QLineEdit* mLineEdit;
    /** Number of decimals for floating points, default 1 */
    unsigned int mFloatPrecision;
    /** Enable color out put for negative values */
    bool mFgColor;
    /** Qt palette */
    QPalette mPalette;
};

#endif
