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
#ifndef RAPIBINARYSENSORARRAYWIDGET_H
#define RAPIBINARYSENSORARRAYWIDGET_H

#include "devicewidget.h"
#include "dataled.h"
#include "binarysensorarray.h"

namespace Rapi
{

/**
 * A widget to visualize a binary sensor array
 * @author Jens Wawerla
 */
class CBinarySensorArrayWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param sensorArray
     * @param parent
     */
    CBinarySensorArrayWidget ( ABinarySensorArray* sensorArray, QWidget* parent );
    /** Default destructor */
    virtual ~CBinarySensorArrayWidget();
    /** Update the data of the display */
    virtual void updateData ();

  protected:
    /** Sensor array to visualize */
    ABinarySensorArray* mSensorArray;
    /** Led to display the binary data */
    CDataLed** mLed;
};

} // namespace

#endif
