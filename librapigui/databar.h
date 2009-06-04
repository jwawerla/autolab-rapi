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
#ifndef RAPIDATABAR_H
#define RAPIDATABAR_H

#include <QProgressBar>
#include <QLabel>

namespace Rapi {

/**
 * Displays data as a bar
 * @author Jens Wawerla
 */
class CDataBar : public QWidget
{
  public:
    /**
     * Default constructor
     * @param parent widget
     * @param caption of data line
     */
    CDataBar(QWidget* parent = 0, QString caption = "NoName", int min = 0, int max = 100);
     /** Default destructor */
    ~CDataBar();
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
     * @param data (true or false) to display
     */
    void setData ( int data );
    /**
     * Sets the range of the bar display
     * @param min minimum value
     * @param max maximum value
     */
    void setRange(int min, int max);

  protected:
    /** Shows the label */
    QLabel* mLabel;
    /** Displays the data */
    QProgressBar* mDisplay;

};

}

#endif
