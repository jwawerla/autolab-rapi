/*****************************************************************************
 * Project: Forever
 * File:    dataled.h
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
 * $Log: dataled.h,v $
 * Revision 1.1  2009-03-23 06:24:22  jwawerla
 * RobotVis works now for data, maps still need more work
 *
 *
 *
 *****************************************************************************/

#ifndef _DATA_LED_H_
#define _DATA_LED_H_

#include <QWidget>
#include <QPixmap>
#include <QLabel>

/**
 * This class provides a qt widget in form of a LED
 * @author J.Wawerla (jwawerla@sfu.ca)
 * @version 0.1
 */
class CDataLed : public QWidget
{
    Q_OBJECT

  public:
    /**
     * Default constructor
     * @param parent widget
     * @param caption of data line
     */
    CDataLed ( QWidget* parent = 0, QString caption = "NoName" );
    /** Default destructor */
    ~CDataLed();
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
    /** Type definition for led state */
    typedef enum {GREEN_ON, GREEN_OFF, RED_ON, RED_OFF} tLedState;
    /**
     * Sets the data to be displayed
     * @param data (true or false) to display
     */
    void setData ( tLedState data );

  protected:
    /** Shows the label */
    QLabel* mLabel;
    /** Displays the data */
    QLabel* mDisplay;
    /** Pixmap for led on */
    //QPixmap* m_ledOn;
    /** Pixmap for led off */
    //QPixmap* m_ledOff;
    /* Data to display */
    tLedState mData;
};


#endif
