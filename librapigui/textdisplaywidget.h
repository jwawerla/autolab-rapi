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
#ifndef RAPITEXTDISPLAYWIDGET_H
#define RAPITEXTDISPLAYWIDGET_H

#include "devicewidget.h"
#include "textdisplay.h"
#include "dataline.h"

namespace Rapi
{

/**
 * Widget to visualize the text display device
 * @author Jens Wawerla
 */
class CTextDisplayWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param text to display
     * @param parent of class
     */
    CTextDisplayWidget ( ATextDisplay* text, QWidget* parent = NULL );
    /** Default destructor */
    ~CTextDisplayWidget();
    /** Update the data of the display */
    void updateData ();

  protected:
    /** Text display device */
    ATextDisplay* mTextDisplay;
    /** Display */
    CDataLine* mText;
};

}

#endif
